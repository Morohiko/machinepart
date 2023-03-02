#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <json_config.h>
#include <log.h>

static int gyroscope_receiver_logic(json_config_t *json_config)
{
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock_fd <= 0) {
        hm_log_error("gyroscope_emulator: cannot create socket\n");
        return -1;
    }

    struct sockaddr_in local_sock = {
        .sin_family = AF_INET,
        .sin_port =
                htons(json_config->modules.gyroscope_receiver_module.mp_port)
    };

    if (!inet_aton(json_config->modules.main_module.mp_ip,
                   &local_sock.sin_addr)) {
        hm_log_error("gyroscope_emulator: cannot do inet_aton, mp_ip = %s\n",
                     json_config->modules.main_module.mp_ip);
        return -1;
    }

    bind(sock_fd, (struct sockaddr *)&local_sock, sizeof(local_sock));

    hm_log_debug("gyroscope_emulator: local socket created with port = %d\n",
                 json_config->modules.gyroscope_receiver_module.mp_port);

    struct sockaddr_in target_sock = {
        .sin_family = AF_INET,
        .sin_port =
                htons(json_config->modules.gyroscope_receiver_module.gp_port)
    };

    if (!inet_aton(json_config->modules.main_module.gp_ip,
                   &target_sock.sin_addr)) {
        hm_log_error("gyroscope_emulator: cannot do inet_aton, gp_ip = %s\n",
                     json_config->modules.main_module.gp_ip);
        return -1;
    }

    hm_log_debug("gyroscope_emulator: target sock is on port = %d",
                 json_config->modules.gyroscope_receiver_module.gp_port);

    while (1) {
        sendto(sock_fd, "1:2", 10, 0, (struct sockaddr *)&target_sock,
               sizeof(target_sock));
        sleep(1);
    }

    return 0;
}

int main()
{
    json_config_t *json_config = init_json_config(
            JSON_CONFIG_FILE /*maybe create another json_config?*/);
    if (!json_config) {
        hm_log_error("gyroscope_emulator: can`t init json_config\n");
        return -1;
    }
    gyroscope_receiver_logic(json_config);
    return 0;
}