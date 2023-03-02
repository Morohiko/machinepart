#ifndef HM_SHELL_SERVER_H
#define HM_SHELL_SERVER_H

#define NAME_SIZE 30
#define HELP_SIZE 100

#define MAX_SHELL_COMMANDS 20
#define MAX_SHELL_CONNECTIONS 10

#include <json_config.h>

typedef struct {
    char name[NAME_SIZE];
    void (*function)(void *, void *);
    char help[HELP_SIZE];
} shell_command_t;

typedef struct {
    pthread_t mgmt_thread_id;
    int count_of_command;
    int mgmt_sock;
    int current_fd_id;
    int mgmt_port;
    int buffer_size;
    int shell_connection_fds[MAX_SHELL_CONNECTIONS];
    shell_command_t shell_command[MAX_SHELL_COMMANDS];
} shell_server_t;

shell_server_t *init_shell_server(shell_t *shell_config);
int destroy_shell_server(shell_server_t *shell_server);

int stop_shell_server();

int shell_server_add_command(shell_server_t *shell_server, void(*func),
                             char *name, char *description);

#endif // HM_SHELL_SERVER_H
