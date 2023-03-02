#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <json_config.h>
#include <log.h>
#include <shell_client.h>

#define BUFFER_SIZE 1000u

static void shell_client_handle_msgs(int sockfd)
{
    char *buffer = (char *)malloc(BUFFER_SIZE);
    while (1) {
        printf("shell> ");
        fflush(stdout);
        memset(buffer, '\0', BUFFER_SIZE);
        fd_set fd_in;
        FD_ZERO(&fd_in);
        FD_SET(sockfd, &fd_in);
        FD_SET(0, &fd_in);

        int ret = select(sockfd + 1, &fd_in, NULL, NULL, NULL);
        if (ret == -1) {
            hm_log_error("shell_client: error after select\n");
            continue;
        }
        if (FD_ISSET(sockfd, &fd_in)) {
            recv(sockfd, buffer, BUFFER_SIZE, 0);
            if (strcmp(buffer, "exit") == 0) {
                send(sockfd, "exit", 4, 0);
                printf("Exit from shell\n");
                break;
            } else {
                printf("%s\n", buffer);
            }
            continue;
        }
        if (FD_ISSET(0, &fd_in)) {
            char *buff = fgets(buffer, BUFFER_SIZE, stdin);
            if (strlen(buff) < 1) {
                hm_log_error("shell_client: empty string received\n");
                continue;
            }
            if (strlen(buffer) > 1 && buffer[strlen(buffer) - 1] == 10) {
                buffer[strlen(buffer) - 1] = '\0';
            }
            for (int i = 0; i < BUFFER_SIZE; i++) {
                if (buffer[i] == '\0') {
                    break;
                }
            }
            if (buffer[0] == '\0' || buffer[0] == '\n') {
                continue;
            }
            send(sockfd, buffer, strlen(buffer), 0);
            memset(buffer, '\0', BUFFER_SIZE);
            recv(sockfd, buffer, BUFFER_SIZE, 0);
            if (strcmp(buffer, "exit") == 0) {
                printf("Exit from shell\n");
                send(sockfd, "exit", 4, 0);
                break;
            } else {
                printf("%s\n", buffer);
            }
        }
    }
}

static void *shell_client_loop(void *data)
{
    shell_client_t *shell_client = (shell_client_t *)data;
    shell_client->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (shell_client->sockfd == -1) {
        hm_log_error("shell_client: socket creation failed...\n");
        free(shell_client);
        return NULL;
    }

    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(shell_client->ipv4_addr),
        .sin_port = htons(shell_client->mgmt_port)
    };

    int retval = connect(shell_client->sockfd, (struct sockaddr *)&servaddr,
                         sizeof(servaddr));
    if (retval != 0) {
        hm_log_error("shell_client: connection with the server failed... %s\n",
                     strerror(errno));
        free(shell_client);
        return NULL;
    }

    shell_client_handle_msgs(shell_client->sockfd);
    close(shell_client->sockfd);
    return NULL;
}

void shell_wait_until_run(shell_client_t *shell_client)
{
    pthread_join(shell_client->thread_id, NULL);
}

shell_client_t *start_shell_client(char *ip_addr, int port, bool is_daemon)
{
    shell_client_t *shell_client =
            (shell_client_t *)malloc(sizeof(shell_client_t));
    if (!shell_client) {
        hm_log_error("shell_client: can`t allocate memory for shell client\n");
        return NULL;
    };
    shell_client->mgmt_port = port;
    strcpy(shell_client->ipv4_addr, ip_addr);
    pthread_create(&shell_client->thread_id, NULL, shell_client_loop,
                   shell_client);
    return shell_client;
}

// TODO: implement
int stop_shell_client(shell_client_t *data)
{
    free(data);
    return 0;
}

int shell_sig_handler(int signum, int sockfd)
{
    switch (signum) {
    case SIGINT:
        printf("\nshell> ");
        fflush(stdout);
        return 0;
    case SIGQUIT:
        printf("\nreceived SIGQUIT\n");
        printf("exit from shell\n");
        // fflush(stdin);
        close(sockfd);
        exit(0);
    default:
        printf("can`t process signal %d (%s)\n", signum, strsignal(signum));
        return -1;
    }
}
