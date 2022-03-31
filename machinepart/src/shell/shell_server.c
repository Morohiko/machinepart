#include "assert.h"
#include "errno.h"
#include "netinet/in.h"
#include "pthread.h"
#include "signal.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "sys/select.h"
#include "unistd.h"

#include "json_config.h"
#include "log.h"
#include "shell_server.h"

#define NAME_SIZE 10
#define HELP_SIZE 100

struct command_t {
  char name[NAME_SIZE];
  void (*function)(void *);
  char help[HELP_SIZE];
};

static int count_of_commands = 0;
static struct command_t commands[10];

static pthread_t mgmt_thread_id;

static void test_help(void *ptr) {
  char *buffer = (char *)ptr;
  sprintf(&buffer[0], "Execute shell command:\n");
  int offset = strlen(buffer);
  for (int i = 0; i < count_of_commands; i++) {
    sprintf(&buffer[offset], "  %s: %s\n", commands[i].name, commands[i].help);
    offset += strlen(buffer);
  }
}

static int create_command(struct command_t *command, void (*function)(void *),
                          char *name, char *help) {
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
  ret = create_command(&commands[0], *test_help, "help",
                       "print avaliable commands");
  ret = create_command(&commands[1], NULL, "exit", "Exit from shell");

  if (ret != 0) {
    print(ERROR, "cant initialize commands in shell server");
    return -1;
  }
  return 0;
}

static int process_commands(char *command, char *responce) {
  for (int i = 0; i < count_of_commands; i++) {
    if (strcmp(commands[i].name, command) == 0) {
      commands[i].function(responce);
      if (strlen(responce) == 0) {
        sprintf(&responce[0], "done\n");
      }
      return 0;
    }
  }
  sprintf(&responce[0], "Unknown command\n");
  return 0;
}

static void *shell_server_loop(void *data) {
  int port = *(int *)data;
  free(data);
  int sock_in = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);
  bind(sock_in, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  listen(sock_in, 1);
  char *buffer = (char *)malloc(json_config.shell.buffer_size);
  if (buffer == NULL) {
    print(ERROR, "can`t allocate buffer in shell_server");
    return NULL;
  }
  int new_sock = accept(sock_in, NULL, NULL);
  char *responce_buffer = (char *)malloc(json_config.shell.buffer_size);
  while (1) {
    memset(buffer, '\0', json_config.shell.buffer_size);
    recv(new_sock, buffer, json_config.shell.buffer_size, 0);
    if (strcmp(buffer, "exit") == 0 || strlen(buffer) == 0) {
      send(new_sock, "exit", 4, 0);
      break;
    } else {
      memset(responce_buffer, '\0', json_config.shell.buffer_size);
      process_commands(buffer, responce_buffer);
      send(new_sock, responce_buffer, strlen(responce_buffer), 0);
    }
  }
  print(INFO, "exit from shell");
  free(buffer);
  close(new_sock);
  close(sock_in);
}

static void *mgmt_server_loop(void *data) {
  int new_sock;
  int sock_in = socket(AF_INET, SOCK_STREAM, 0);
  int start_shell_port = 9020;
  int current_shell_port = start_shell_port;
  int amount_avail_port = 20;
  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(json_config.shell.mgmt_port);
  bind(sock_in, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  listen(sock_in, 5);

  char *buffer = (char *)malloc(json_config.shell.buffer_size);
  if (buffer == NULL) {
    print(ERROR, "can`t allocate buffer in shell_server");
    return NULL;
  }

  struct timeval tv;
  while (1) {
    fd_set fd_in;
    FD_ZERO(&fd_in);
    FD_SET(sock_in, &fd_in);
    int ret = select(sock_in + 1, &fd_in, NULL, NULL, NULL);
    if (ret == -1) {
      print(ERROR, "error after select");
      continue;
    }
    if (FD_ISSET(sock_in, &fd_in)) {
      new_sock = accept(sock_in, NULL, NULL);
      if (new_sock < 0) {
        print(ERROR, "cannot create new_sock, errno = %s", strerror(errno));
        continue;
      }
      memset(buffer, '\0', json_config.shell.buffer_size);
      recv(new_sock, buffer, json_config.shell.buffer_size, 0);
      if (strcmp(buffer, "exit") == 0) {
        send(new_sock, "exit", 4, 0);
        break;
      } else if (strcmp(buffer, "get_port") == 0) {
        // find port for new shell client
        char portstr[4];
        memset(portstr, '\0', 4);
        sprintf(portstr, "%d", current_shell_port);
        send(new_sock, portstr, 4, 0);
        int *port = (int *)malloc(4);
        *port = current_shell_port;
        current_shell_port++;
        if (current_shell_port > (start_shell_port + amount_avail_port)) {
          current_shell_port = start_shell_port;
        }

        memset(buffer, '\0', json_config.shell.buffer_size);
        recv(new_sock, buffer, json_config.shell.buffer_size, 0);
        if (strcmp(buffer, "ok") == 0) {
          pthread_t shell_thread_id;
          pthread_create(&shell_thread_id, NULL, shell_server_loop, port);
        } else {
          continue;
        }
      } else {
        print(ERROR, "unknown command on mgmt_port %s", buffer);
      }
    }
  }
  close(new_sock);
  close(sock_in);
  free(buffer);
}

int start_shell_server() {
  init_commands();
  print(INFO, "start_shell_server");
  pthread_create(&mgmt_thread_id, NULL, mgmt_server_loop, NULL);
  return 0;
}

int stop_shell_server() {
  // TODO: close sockets
  print(INFO, "stop_shell_server");
  return 0;
}
