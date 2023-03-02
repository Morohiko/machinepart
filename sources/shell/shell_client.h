#ifndef HM_SHELL_CLIENT_H
#define HM_SHELL_CLIENT_H

#include <stdbool.h>

typedef struct {
    char ipv4_addr[16u];
    bool is_daemon;
    pthread_t thread_id;
    int sockfd;
    int mgmt_port;
} shell_client_t;

shell_client_t *start_shell_client(char *ip_addr, int port, bool is_daemon);

int stop_shell_client(shell_client_t *data);

void shell_wait_until_run(shell_client_t *shell_client);

int shell_sig_handler(int signum, int sockfd);

#endif // HM_SHELL_CLIENT_H
