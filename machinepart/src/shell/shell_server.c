#include "assert.h"
#include "errno.h"
#include "netinet/in.h"
#include "pthread.h"
#include "sys/select.h"
#include "unistd.h"

#include "json_config.h"
#include "log.h"
#include "shell_server.h"
#include "stdlib.h"

static pthread_t shell_thread_id;

static int process_commands(char *command, char *responce) {
  if (strcmp(command, "help") == 0) {
    memset(responce, '\0', json_config.shell.buffer_size);
    strcpy(responce, "TODO: help list");
  } else {
    memset(responce, '\0', json_config.shell.buffer_size);
    strcpy(responce, "Undefined command");
  }
}

static void *shell_loop(void *data) {
  int sock_in = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(json_config.shell.port);

  bind(sock_in, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  listen(sock_in, 1);

  char *buffer = (char *)malloc(json_config.shell.buffer_size);
  if (buffer == NULL) {
    print(ERROR, "can`t allocate buffer in shell_server");
    return NULL;
  }

  while (1) {
    fd_set fd_in;
    FD_ZERO(&fd_in);
    FD_SET(sock_in, &fd_in);
    int ret = select(sock_in + 1, &fd_in, NULL, NULL, NULL);
    if (ret == -1) {
      print(ERROR, "error after select") return NULL;
    }
    memset(buffer, '\0', json_config.shell.buffer_size);
    if (FD_ISSET(sock_in, &fd_in)) {
      int new_sock = accept(sock_in, NULL, NULL);
      if (new_sock < 0) {
        print(ERROR, "cannot create new_sock, errno = %s", strerror(errno));
        continue;
      }
      int stat = recv(new_sock, buffer, json_config.shell.buffer_size, 0);
      char *responce;
      process_commands(buffer, buffer);
      send(new_sock, buffer, strlen(buffer), 0);
    }
  }
  free(buffer);
}

int start_shell_server() {
  pthread_create(&shell_thread_id, NULL, shell_loop, NULL);
}

int stop_shell_server() {}
