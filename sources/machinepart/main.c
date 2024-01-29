#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <camera.h>
#include <camera_transmitter.h>
#include <shell_client.h>
#include <gyroscope_receiver.h>
#include <json_config.h>
#include <log.h>
#include <shell_server.h>

#define CONFIG_FILEPATH 128u

typedef struct {
    char config_file_path[CONFIG_FILEPATH];

    camera_t *camera;
    camera_transmitter_t *camera_transmitter;
    gyroscope_receiver_t *gyroscope_receiver;
    motor_controller_t *motor_controller;

    json_config_t *json_config;
    shell_server_t *shell_server;
    shell_client_t *shell_client;
} machinepart_t;

static machinepart_t *machinepart = NULL;

static int init_shell_server_commands(shell_server_t *shell_server)
{
    shell_server_add_command(shell_server, print_json_config, "config_show",
                             "show json config");
    shell_server_add_command(shell_server, config_start_gyroscope_receiver,
                             "start_gyroscope_receiver",
                             "Start gyroscope receiver module");
    shell_server_add_command(shell_server, config_stop_gyroscope_receiver,
                             "stop_gyroscope_receiver",
                             "Stop gyroscope receiver module");
    shell_server_add_command(shell_server, config_start_motor_controller,
                             "start_motor_controller",
                             "Start motor controller module");
    shell_server_add_command(shell_server, config_stop_motor_controller,
                             "stop_motor_controller",
                             "Stop motor controller module");
    shell_server_add_command(shell_server, config_start_camera_module,
                             "start_camera_module", "Start camera module");
    shell_server_add_command(shell_server, config_stop_camera_module,
                             "stop_camera_module", "Stop camera module");
    shell_server_add_command(shell_server, config_start_camera_transmitter,
                             "start_camera_transmitter",
                             "Start camera transmitter module");
    return 0;
}

static int core_loop(machinepart_t *machinepart)
{
    int ret = 0;
    // fix me
    machinepart->camera = NULL;
    machinepart->camera_transmitter = NULL;
    machinepart->gyroscope_receiver = NULL;
    machinepart->motor_controller = NULL;
    machinepart->shell_client = NULL;
    //
    while (1) {
        sleep(1);

        if (machinepart->json_config->modules.camera_module.state &&
            !machinepart->camera) {
            hm_log_info("core: start camera\n");
            machinepart->camera = init_camera(
                    &machinepart->json_config->modules.camera_module);
        } else if (!machinepart->json_config->modules.camera_module.state &&
                   machinepart->camera) {
            hm_log_info("core: stop camera\n");
            ret = destroy_camera(machinepart->camera);
            if (ret) {
                hm_log_error("core: can`t destroy camera\n");
                return ret;
            }
            machinepart->camera = NULL;
        }

        if (machinepart->json_config->modules.camera_transmitter_module.state &&
            !machinepart->camera_transmitter) {
            hm_log_info("core: start camera_transmitter\n");
            machinepart->camera_transmitter = camera_transmitter_init();
        } else if (!machinepart->json_config->modules.camera_transmitter_module
                            .state &&
                   machinepart->camera_transmitter) {
            hm_log_info("core: stop camera_transmitter\n");
            ret = camera_transmitter_destroy(machinepart->camera_transmitter);
            if (ret) {
                hm_log_error("core: can`t destroy camera_transmitter\n");
                return ret;
            }
            machinepart->camera_transmitter = NULL;
        }

        if (machinepart->json_config->modules.gyroscope_receiver_module.state &&
            !machinepart->gyroscope_receiver) {
            hm_log_info("core: start gyroscope_receiver\n");
            machinepart->gyroscope_receiver = start_gyroscope_receiver(
                    &machinepart->json_config->modules
                             .gyroscope_receiver_module);
        } else if (!machinepart->json_config->modules.gyroscope_receiver_module
                            .state &&
                   machinepart->gyroscope_receiver) {
            hm_log_info("core: stop gyroscope_receiver\n");
            ret = stop_gyroscope_receiver(machinepart->gyroscope_receiver);
            if (ret) {
                hm_log_error("core: can`t destroy gyroscope_receiver\n");
                return ret;
            }
            machinepart->gyroscope_receiver = NULL;
        }

        if (machinepart->json_config->modules.motor_module.state &&
            !machinepart->motor_controller) {
            hm_log_info("core: start motor_controller\n");
            machinepart->motor_controller = start_motor_controller(
                    &machinepart->json_config->modules.motor_module);
        } else if (!machinepart->json_config->modules.motor_module.state &&
                   machinepart->motor_controller) {
            hm_log_info("core: stop motor_controller\n");
            ret = stop_motor_controller(machinepart->motor_controller);
            if (ret) {
                hm_log_error("core: can`t destroy motor_controller\n");
                return ret;
            }
            machinepart->motor_controller = NULL;
        }

        if (machinepart->json_config->shell.local_shell_client &&
            !machinepart->shell_client) {
            machinepart->shell_client = start_shell_client(
                    "0.0.0.0", machinepart->json_config->shell.mgmt_port,
                    false);
        }
    }
}

static void sig_handler(int signum)
{
    if (signum == SIGQUIT) {
        stop_shell_server();
    }
    shell_sig_handler(signum, machinepart->shell_client->sockfd);
}

static void init_signals()
{
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);
}

int main(int argc, char **argv)
{
    hm_log_info("main: ======== Start Machinepart =======\n");

    // Process command-line options
    int option;
    char *config = NULL;
    while ((option = getopt(argc, argv, "cdf:")) != -1) {
        switch (option) {
        case 'd':
            hm_log_info("main: run as a daemon\n");
            break;
        case 'f':
            config = optarg;
            break;
        default:
            hm_log_error("main: invalid args\n");
            return -1;
        }
    }

    if (config == NULL) {
        config = JSON_CONFIG_FILE;
    }

    if (strlen(config) > CONFIG_FILEPATH) {
        hm_log_error("main: path to config file is too long\n");
        return -1;
    }

    hm_log_info("main: current config filepath: %s\n", config);

    init_signals();

    // init machinepart
    machinepart = (machinepart_t *)malloc(sizeof(machinepart_t));
    memset(machinepart, '\0', sizeof(machinepart_t));

    // init json_config
    sprintf(machinepart->config_file_path, "%s", config);
    machinepart->json_config = init_json_config(machinepart->config_file_path);
    if (!machinepart->json_config) {
        hm_log_error("main: can`t init json_config\n");
        goto exit;
    }

    // init shell_server
    machinepart->shell_server =
            init_shell_server(&machinepart->json_config->shell);
    if (!machinepart->shell_server) {
        hm_log_error("main: can`t init shell_server\n");
        goto exit;
    }
    int ret = init_shell_server_commands(machinepart->shell_server);
    if (ret) {
        hm_log_error("main: can`t init shell commands\n");
        goto exit;
    }

    core_loop(machinepart);

exit:
    if (!machinepart) {
        return 0;
    }

    if (machinepart->shell_server) {
        destroy_shell_server(machinepart->shell_server);
    }
    if (machinepart->json_config) {
        destroy_json_config(machinepart->json_config);
    }

    return 0;
}
