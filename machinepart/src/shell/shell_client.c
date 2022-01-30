#include "errno.h"
#include "netinet/in.h"
#include "signal.h"
#include "stdlib.h"
#include "string.h"
#include "sys/socket.h"
#include "unistd.h"

#include "json_config.h"
#include "log.h"

#define JSON_CONFIG_FILE                                                       \
  "/home/user/work/headmachine/machinepart/machinepart/src/config/config.json"

int sockfd = 0;
char *buffer = NULL;

void sig_handler(int signum) {
  if (signum == SIGINT) {
    printf("\nshell> ");
    fflush(stdout);
  } else if (signum == SIGQUIT) {
    printf("\nreceived SIGQUIT\n");
    printf("exit from shell\n");
    close(sockfd);
    exit(0);
  } else {
    printf("can`t process signal %d (%s)\n", signum, strsignal(signum));
  }
}

int connect_to_host() {
  int ret = 0;
  struct sockaddr_in serv_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(json_config.shell.port);
  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  return sockfd;
}

static void process_command(char *command) {
  if (strcmp(command, "exit") == 0) {
    free(buffer);
    close(sockfd);
    exit(0);
  }
}

int main() {
  signal(SIGINT, sig_handler);
  signal(SIGQUIT, sig_handler);
  set_log_level(DEBUG);
  init_json_config(JSON_CONFIG_FILE);
  if (json_config.shell.state == 0) {
    print(ERROR, "shell is disabled");
    return 0;
  }
  buffer = (char *)malloc(json_config.shell.buffer_size);
  if (buffer == NULL) {
    print(ERROR, "can`t allocate memory");
    return 0;
  }
  while (1) {
    sockfd = connect_to_host();
    printf("shell> ");
    memset(buffer, '\0', json_config.shell.buffer_size);
    fgets(buffer, json_config.shell.buffer_size, stdin);
    // remove line feed
    buffer[strlen(buffer) - 1] = '\0';
    process_command(buffer);
    send(sockfd, buffer, strlen(buffer), 0);
    memset(buffer, '\0', json_config.shell.buffer_size);
    recv(sockfd, buffer, json_config.shell.buffer_size, 0);
    printf("GP: %s\n", buffer);
    close(sockfd);
  }
  return 0;
}
