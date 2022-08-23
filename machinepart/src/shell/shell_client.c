#include "assert.h"
#include "errno.h"
#include "netinet/in.h"
#include "pthread.h"
#include "signal.h"
#include "stdbool.h"
#include "sys/select.h"
#include "unistd.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "config.h"
#include "json_config.h"
#include "log.h"
#include "shell_client.h"

int sockfd = 0;

pthread_t thread_id;

void shell_client_handle_msgs(int sockfd) {
  char *buffer = (char *)malloc(json_config.shell.buffer_size);
  memset(buffer, '\0', json_config.shell.buffer_size);
  while (1) {
    printf("shell> ");
    fflush(stdout);
    memset(buffer, '\0', json_config.shell.buffer_size);
    fd_set fd_in;
    FD_ZERO(&fd_in);
    FD_SET(sockfd, &fd_in);
    FD_SET(0, &fd_in);

    int ret = select(sockfd + 1, &fd_in, NULL, NULL, NULL);
    if (ret == -1) {
      print(ERROR, "error after select");
      continue;
    }
    if (FD_ISSET(sockfd, &fd_in)) {
      recv(sockfd, buffer, json_config.shell.buffer_size, 0);
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
        send(sockfd, "exit", 4, 0);
        break;
      } else {
        printf("%s\n", buffer);
      }
    }
  }
}

void *shell_client_loop(void *data) {
  int retval;
  int is_remote = *(int *)data;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    print(ERROR, "socket creation failed...\n");
    exit(0);
  }

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr =
      is_remote ? inet_addr(json_config.modules.main_module.gp_ip)
                : inet_addr("127.0.0.1");
  servaddr.sin_port = htons(json_config.shell.mgmt_port);

  retval = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  if (retval != 0) {
    print(ERROR, "connection with the server failed...\n");
    exit(0);
  }

  shell_client_handle_msgs(sockfd);
  close(sockfd);
}

void shell_wait_until_run() { pthread_join(thread_id, NULL); }

int start_shell_client(int is_remote) {
  pthread_create(&thread_id, NULL, shell_client_loop, &is_remote);
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
