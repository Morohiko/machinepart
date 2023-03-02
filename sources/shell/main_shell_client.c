#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include <json_config.h>
#include <log.h>
#include <shell_client.h>

typedef struct {
    shell_client_t *shell_client;
    json_config_t *json_config;
} main_shell_client_t;

static main_shell_client_t *main_shell_client = NULL;

static void sig_handler(int signum)
{
    shell_sig_handler(signum, main_shell_client->shell_client->sockfd);
}

int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);

    main_shell_client =
            (main_shell_client_t *)malloc(sizeof(main_shell_client_t));
    if (!main_shell_client) {
        hm_log_error(
                "main_shell_client: can`t allocate memory for main_shell_client\n");
        goto exit;
    }

    main_shell_client->json_config = init_json_config(JSON_CONFIG_FILE);
    if (main_shell_client->json_config == NULL) {
        hm_log_error("controller: can`t create shell client\n");
        goto exit;
    }

    if (main_shell_client->json_config->shell.state == 0) {
        hm_log_error("shell_client: shell is disabled\n");
        goto exit;
    }

    // TODO: move ip addr to json_config
    main_shell_client->shell_client = start_shell_client(
            "0.0.0.0", main_shell_client->json_config->shell.mgmt_port,
            true);

    shell_wait_until_run(main_shell_client->shell_client);

exit:
    if (main_shell_client->json_config) {
        free(main_shell_client->json_config);
    }
    if (main_shell_client->shell_client) {
        free(main_shell_client->shell_client);
    }
    return 0;
}
