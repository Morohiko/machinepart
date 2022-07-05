#include "assert.h"
#include "errno.h"
#include "netinet/in.h"
#include "pthread.h"
#include "signal.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "sys/select.h"
#include "sys/socket.h"
#include "unistd.h"

#include "config.h"
#include "json_config.h"
#include "log.h"

int sockfd = 0;

static pthread_t thread_id;

int get_port_by_mgmt_socket(int *port) {
  int ret = 0;
  struct sockaddr_in serv_addr;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(json_config.shell.mgmt_port);
  ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  char buffer[4];
  ret = send(sockfd, "get_port", 8, 0);
  recv(sockfd, buffer, 4, 0);
  *port = atoi(buffer);
  send(sockfd, "ok", 2, 0);
  close(sockfd);
}

void *shell_client_loop(void *data) {
  int port;
  get_port_by_mgmt_socket(&port);
  struct sockaddr_in serv_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);
  printf("Welcome to MP shell: port = %d\n", port);
  sleep(1);
  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  char *buffer = (char *)malloc(json_config.shell.buffer_size);
  memset(buffer, '\0', json_config.shell.buffer_size);
  while (1) {
    printf("shell> ");
    fflush(stdin);
    memset(buffer, '\0', json_config.shell.buffer_size);
    fgets(buffer, json_config.shell.buffer_size, stdin);
    if (strlen(buffer) > 1 && buffer[strlen(buffer) - 1] == 10) {
      buffer[strlen(buffer) - 1] = '\0';
    }
    for (int i = 0; i < json_config.shell.buffer_size; i++) {
      if (buffer[i] == '\0') {
        break;
      }
    }
    if (buffer[0] == '\0' || buffer[0] == '\n') {
      continue;
    }
    send(sockfd, buffer, strlen(buffer), 0);
    memset(buffer, '\0', json_config.shell.buffer_size);
    recv(sockfd, buffer, json_config.shell.buffer_size, 0);
    if (strcmp(buffer, "exit") == 0) {
      printf("Exit from shell\n");
      break;
    } else {
      printf("%s\n", buffer);
    }
  }
  close(sockfd);
  return 0;
}

int start_shell_client() {
  pthread_create(&thread_id, NULL, shell_client_loop, NULL);
}

int stop_shell_client() {}

void shell_sig_handler(int signum) {
  if (signum == SIGINT) {
    printf("\nshell> ");
    fflush(stdout);
  } else if (signum == SIGQUIT) {
    printf("\nreceived SIGQUIT\n");
    printf("exit from shell\n");
    // fflush(stdin);
    close(sockfd);
    exit(0);
  } else {
    printf("can`t process signal %d (%s)\n", signum, strsignal(signum));
  }
}
