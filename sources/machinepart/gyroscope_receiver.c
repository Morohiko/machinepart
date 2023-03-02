#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <gyroscope_receiver.h>
#include <json_config.h>
#include <log.h>
#include <motor_controller.h>

#define MAX_GYROSCOPE_DATA_SIZE 8

// parse in format: X:Y
static int parse_gyroscope_data(char *msg, motor_controller_t *motor_data)
{
    int x = 0;
    int y = 0;

    int parsed = 0;

    parsed = sscanf(msg, "%d:%d", &x, &y);

    if (parsed != 2) {
        hm_log_error("gyrosope_receiver: parsed: %d, must be 2\n", parsed);
        return -1;
    }

    motor_data->platform_motor->angle_x = x;
    motor_data->platform_motor->angle_y = y;

    hm_log_debug("gyrosope_receiver: parsed gyroscope data: x: %d, y: %d\n", x,
                 y);

    return 0;
}

static void *gyroscope_receiver_thread(void *user_data)
{
    gyroscope_receiver_t *ctx = (gyroscope_receiver_t *)user_data;
    int retval = 0;
    char msg[MAX_GYROSCOPE_DATA_SIZE] = { 0 };

new_connection:
    hm_log_info("gyroscope_receiver: gp connection...\n");
    ctx->gp_connfd = accept(ctx->gp_sockfd, NULL, NULL);
    if (ctx->gp_connfd < 0) {
        hm_log_error("gyroscope_receiver: connection with gp is refused\n");
        return NULL;
    }
    hm_log_info("gyroscope_receiver: connected to gp\n");
    while (ctx->is_working) {
        retval = read(ctx->gp_connfd, msg, sizeof(msg));
        if (retval < 0) {
            hm_log_error("gyrosope_receiver: cannot recv message\n");
            break;
        }
        if (msg == NULL) {
            break;
        }
        if (!strcmp(msg, "exit")) {
            hm_log_info(
                    "gyroscope_receiver: \"exit\" command is received, exit..\n");
        }
        retval = parse_gyroscope_data(msg, &ctx->motor_data);
        if (retval != 0) {
            hm_log_error(
                    "gyrosope_receiver: cant parse gyroscope data, msg: %s\n",
                    msg);
            memset(msg, '\0', MAX_GYROSCOPE_DATA_SIZE);
        }
    }

    close(ctx->gp_connfd);
    if (ctx->is_working) {
        hm_log_info(
                "gyroscope_receiver: some issue happens but gr is still working\n");
        goto new_connection;
    }
    return NULL;
}

gyroscope_receiver_t *
start_gyroscope_receiver(gyroscope_receiver_module_t *gyroscope_receiver_config)
{
    gyroscope_receiver_t *ctx =
            (gyroscope_receiver_t *)malloc(sizeof(gyroscope_receiver_t));

    ctx->gp_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl("0.0.0.0" /*fix me*/),
        .sin_port = htons(gyroscope_receiver_config->gp_port)
    };

    bind(ctx->gp_sockfd, &servaddr, sizeof(servaddr));

    listen(ctx->gp_sockfd, 1);

    ctx->is_working = true;

    pthread_create(&ctx->tid, NULL, gyroscope_receiver_thread, ctx);

    return ctx;
}

int stop_gyroscope_receiver(gyroscope_receiver_t *gyroscope_receiver)
{
    int retval = 0;

    gyroscope_receiver->is_working = false;
    hm_log_info("gyroscope_receiver: waiting while gr is working\n");
    retval = write(gyroscope_receiver->gp_connfd, "exit", 4);
    if (retval != 4) {
        hm_log_error("gyroscope_receiver: can`t send \"exit\" command\n");
    }
    pthread_join(gyroscope_receiver->tid, NULL);

    close(gyroscope_receiver->gp_sockfd);
    free(gyroscope_receiver);
    return 0;
}

