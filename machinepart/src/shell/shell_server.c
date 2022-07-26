#include "assert.h"
#include "errno.h"
#include "netinet/in.h"
#include "pthread.h"
#include "signal.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

#include "json_config.h"
#include "log.h"
#include "shell_server.h"

#define NAME_SIZE 30
#define HELP_SIZE 100

#define MAX_SHELL_CONNECTIONS 10

struct command_t {
  char name[NAME_SIZE];
  void (*function)(void *, void *);
  char help[HELP_SIZE];
};

int count_of_commands = 0;
struct command_t commands[10];

int shell_connection_fds[MAX_SHELL_CONNECTIONS];

static pthread_t mgmt_thread_id;

int mgmt_sock;
static void help_command(void *arg, void *resp) {
  char *buffer = (char *)resp;
  sprintf(&buffer[0], "Execute shell command:\n");
  int offset = strlen(buffer);
  for (int i = 0; i < count_of_commands; i++) {
    sprintf(&buffer[offset], "  %s - %s\n", commands[i].name, commands[i].help);
    offset = strlen(buffer);
  }
}

static int create_command(struct command_t *command,
                          void (*function)(void *, void *), char *name,
                          char *help) {
  assert(command);
  if (strlen(name) > NAME_SIZE) {
    print(
        ERROR,
        "cant create command(%s), size of name(%lu) biggest then max size(%d)",
        name, strlen(name), NAME_SIZE);
    return -1;
  }
  if (strlen(help) > HELP_SIZE) {
    print(
        ERROR,
        "cant create command(%s), size of help(%lu) biggest then max size(%d)",
        name, strlen(help), HELP_SIZE);
    return -1;
  }
  memcpy(command->name, name, strlen(name));
  memcpy(command->help, help, strlen(help));
  command->function = function;
  count_of_commands++;
  return 0;
}

static int init_commands() {
  int ret = 0;
  ret = create_command(&commands[count_of_commands], *help_command, "help",
                       "print avaliable commands");
  ret = create_command(&commands[count_of_commands], NULL, "exit",
                       "Exit from shell");

  if (ret != 0) {
    print(ERROR, "cant initialize commands in shell server");
    return -1;
  }
  return 0;
}

static int process_commands(char *command_name, char *responce) {
  char *args = command_name;
  for (int i = 0; i < command_name[i]; i++) {
    if (args[i] == ' ') {
      args[i] = '\0';
      args = &command_name[++i];
      break;
    }
  }

  for (int i = 0; i < count_of_commands; i++) {
    if (strcmp(commands[i].name, command_name) == 0) {
      commands[i].function(args, responce);
      if (strlen(responce) == 0) {
        sprintf(&responce[0], "done\n");
      }
      return 0;
    }
  }
  sprintf(&responce[0], "Unknown command\n");
  return 0;
}

void *shell_server_loop(void *conn) {
  int client_sock = *(int *)conn;

  char *buffer = (char *)malloc(json_config.shell.buffer_size);
  if (buffer == NULL) {
    print(ERROR, "can`t allocate buffer in shell_server");
    return NULL;
  }

  char *responce_buffer = (char *)malloc(json_config.shell.buffer_size);

  while (1) {
    memset(buffer, '\0', json_config.shell.buffer_size);
    recv(client_sock, buffer, json_config.shell.buffer_size, 0);
    if (strcmp(buffer, "exit") == 0 || strlen(buffer) == 0) {
      send(client_sock, "exit", 4, 0);
      *(int *)conn = 0;
      break;
    } else {
      memset(responce_buffer, '\0', json_config.shell.buffer_size);
      process_commands(buffer, responce_buffer);
      send(client_sock, responce_buffer, strlen(responce_buffer), 0);
    }
  }
  close(client_sock);
}

void *mgmt_server_loop(void *data) {
  int connfd, retval;
  struct sockaddr_in servaddr;

  mgmt_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (mgmt_sock == -1) {
    print(ERROR, "socket creation failed...\n");
    return NULL;
  }

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(json_config.shell.mgmt_port);

  retval = bind(mgmt_sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if (retval != 0) {
    print(ERROR, "socket bind failed...\n");
    close(mgmt_sock);
    return NULL;
  }

  retval = listen(mgmt_sock, 5);
  if ((retval) != 0) {
    print(ERROR, "Listen failed...\n");
    close(mgmt_sock);
    return NULL;
  }

  while (1) {
    connfd = accept(mgmt_sock, NULL, NULL);
    if (connfd < 0) {
      print(ERROR, "server accept failed...\n");
      break;
    }
    pthread_t thread_id;
    int i;
    for (i = 0; i < MAX_SHELL_CONNECTIONS; i++) {
      if (shell_connection_fds[i] == 0) {
        shell_connection_fds[i] = connfd;
        break;
      }
    }
    pthread_create(&thread_id, NULL, shell_server_loop,
                   (void *)&shell_connection_fds[i]);
    sleep(1);
  }

  close(mgmt_sock);
}

int start_shell_server() {
  print(INFO, "start_shell_server");
  init_commands();
  for (int i = 0; i < MAX_SHELL_CONNECTIONS; i++) {
    shell_connection_fds[i] = 0;
  }
  pthread_create(&mgmt_thread_id, NULL, mgmt_server_loop, NULL);
  return 0;
}

int stop_shell_server() {
  print(INFO, "stop_shell_server");
  for (int i = 0; i < MAX_SHELL_CONNECTIONS; i++) {
    if (shell_connection_fds[i] != 0) {
      send(shell_connection_fds[i], "exit", 4, 0);
      shell_connection_fds[i] = 0;
    }
  }
  close(mgmt_sock);
  return 0;
}

int add_command(void(*func), char *name, char *description) {
  create_command(&commands[count_of_commands], func, name, description);
}
