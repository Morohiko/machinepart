#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <netdb.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 

#include <json_config.h>
#include <log.h>
#include <shell_server.h>

static void help_command(void *arg, void *resp)
{
    shell_server_t *shell_server = (shell_server_t *)arg;
    char *buffer = (char *)resp;
    sprintf(&buffer[0], "Execute shell command:\n");
    int offset = strlen(buffer);
    for (int i = 0; i < shell_server->count_of_command; i++) {
        sprintf(&buffer[offset], "  %s - %s\n",
                shell_server->shell_command[i].name,
                shell_server->shell_command[i].help);
        offset = strlen(buffer);
    }
}

static int create_command(shell_server_t *shell_server,
                          void (*function)(void *, void *), char *name,
                          char *help)
{
    if (strlen(name) > NAME_SIZE) {
        hm_log_error(
                "shell_server: cant create command(%s), size of name(%zu) biggest then max size(%d)",
                name, strlen(name), NAME_SIZE);
        return -1;
    }
    if (strlen(help) > HELP_SIZE) {
        hm_log_error(
                "shell_server: cant create command(%s), size of help(%zu) biggest then max size(%d)",
                name, strlen(help), HELP_SIZE);
        return -1;
    }
    sprintf(shell_server->shell_command[shell_server->count_of_command].name,
            "%s", name);
    sprintf(shell_server->shell_command[shell_server->count_of_command].help,
            "%s", help);
    shell_server->shell_command[shell_server->count_of_command].function =
            function;
    shell_server->count_of_command++;
    return 0;
}

static int init_default_commands(shell_server_t *shell_server)
{
    int ret = create_command(shell_server, *help_command, "help",
                             "print avaliable commands");
    if (ret) {
        hm_log_error(
                "shell_server: can`t init command \"help\" in shell server\n");
        return ret;
    }
    ret = create_command(shell_server, NULL, "exit", "Exit from shell");
    if (ret) {
        hm_log_error(
                "shell_server: can`t init command \"exit\" in shell server\n");
        return ret;
    }
    return 0;
}

static int process_commands(shell_server_t *shell_server, char *command_name,
                            char *responce)
{
    for (int i = 0; i < shell_server->count_of_command; i++) {
        if (strcmp(shell_server->shell_command[i].name, command_name)) {
            continue;
        }
        if (strcmp(command_name, "help") == 0) {
            shell_server->shell_command[i].function(shell_server, responce);
        } else {
            shell_server->shell_command[i].function(command_name, responce);
        }
        if (strlen(responce) == 0) {
            sprintf(&responce[0], "done\n");
        }
        return 0;
    }
    sprintf(&responce[0], "Unknown command\n");
    return 0;
}

static void *shell_server_loop(void *arg)
{
    shell_server_t *shell_server = (shell_server_t *)arg;
    int *client_sock =
            &shell_server->shell_connection_fds[shell_server->current_fd_id];

    char *buffer = (char *)malloc(shell_server->buffer_size);
    if (buffer == NULL) {
        hm_log_error("shell_server: can`t allocate buffer in shell_server\n");
        return NULL;
    }

    char *responce_buffer = (char *)malloc(shell_server->buffer_size);

    while (1) {
        memset(buffer, '\0', shell_server->buffer_size);
        recv(*client_sock, buffer, shell_server->buffer_size, 0);
        if (strcmp(buffer, "exit") == 0 || strlen(buffer) == 0) {
            send(*client_sock, "exit", 4, 0);
            *(int *)client_sock = 0;
            break;
        } else {
            memset(responce_buffer, '\0', shell_server->buffer_size);
            process_commands(shell_server, buffer, responce_buffer);
            send(*client_sock, responce_buffer, strlen(responce_buffer), 0);
        }
    }
    close(*client_sock);
    return NULL;
}

static void *mgmt_server_loop(void *data)
{
    shell_server_t *shell_server = (shell_server_t *)data;
    shell_server->mgmt_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (shell_server->mgmt_sock == -1) {
        hm_log_error("shell_server: socket creation failed...\n");
        return NULL;
    }

    struct sockaddr_in servaddr = { .sin_family = AF_INET,
                                    .sin_addr.s_addr = htonl(INADDR_ANY),
                                    .sin_port =
                                            htons(shell_server->mgmt_port) };

    int retval = bind(shell_server->mgmt_sock, (struct sockaddr *)&servaddr,
                  sizeof(servaddr));
    if (retval != 0) {
        hm_log_error("shell_server: socket bind failed...\n");
        close(shell_server->mgmt_sock);
        return NULL;
    }

    retval = listen(shell_server->mgmt_sock, 5);
    if ((retval) != 0) {
        hm_log_error("shell_server: Listen failed...\n");
        close(shell_server->mgmt_sock);
        return NULL;
    }

    hm_log_info("shell_server: listen\n");
    while (1) {
        int connfd = accept(shell_server->mgmt_sock, NULL, NULL);
        if (connfd < 0) {
            hm_log_error("shell_server: server accept failed...\n");
            break;
        }
        pthread_t thread_id;
        int i;
        for (i = 0; i < MAX_SHELL_CONNECTIONS; i++) {
            if (shell_server->shell_connection_fds[i] == 0) {
                shell_server->shell_connection_fds[i] = connfd;
                break;
            }
        }
        shell_server->current_fd_id = i;
        pthread_create(&thread_id, NULL, shell_server_loop,
                       (void *)shell_server);
        sleep(1);
    }

    close(shell_server->mgmt_sock);
    return NULL;
}

static int start_shell_server(shell_server_t *shell_server)
{
    hm_log_info("shell_server: start_shell_server\n");
    init_default_commands(shell_server);
    for (int i = 0; i < MAX_SHELL_CONNECTIONS; i++) {
        shell_server->shell_connection_fds[i] = 0;
    }
    pthread_create(&shell_server->mgmt_thread_id, NULL, mgmt_server_loop,
                   shell_server);
    return 0;
}

int stop_shell_server(shell_server_t *shell_server)
{
    hm_log_info("shell_server: stop_shell_server\n");
    for (int i = 0; i < MAX_SHELL_CONNECTIONS; i++) {
        if (shell_server->shell_connection_fds[i] != 0) {
            send(shell_server->shell_connection_fds[i], "exit", 4, 0);
            shell_server->shell_connection_fds[i] = 0;
        }
    }
    close(shell_server->mgmt_sock);
    return 0;
}

int shell_server_add_command(shell_server_t *shell_server, void(*func),
                             char *name, char *description)
{
    int ret = create_command(shell_server, func, name, description);
    if (ret) {
        hm_log_error("shell_server: can`t create command, name: %s\n", name);
    }
    return ret;
}

shell_server_t *init_shell_server(shell_t *shell_config)
{
    shell_server_t *shell_server =
            (shell_server_t *)malloc(sizeof(shell_server_t));
    if (!shell_server) {
        hm_log_error("shell_server: can`t allocate memory for shell_server\n");
        return NULL;
    }
    shell_server->count_of_command = 0;
    shell_server->mgmt_port = shell_config->mgmt_port;
    shell_server->buffer_size = shell_config->buffer_size;

    // start shell_server
    int ret = start_shell_server(shell_server);
    if (ret) {
        hm_log_error("can`t start shell_server\n");
        free(shell_server);
        return NULL;
    }
    return shell_server;
}

int destroy_shell_server(shell_server_t *shell_server)
{
    if (shell_server == NULL) {
        hm_log_error("shell_server: can`t destroy shell_server, null\n");
        return -1;
    }
    free(shell_server);
    return 0;
}
