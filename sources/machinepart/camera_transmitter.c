#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <camera_transmitter.h>
#include <fps_checker.h>
#include <json_config.h>
#include <log.h>

#define ACK_MESSAGE_SIZE 4

void *start_send_camera_data_through_tcp(void *data)
{
    camera_transmitter_t *cam_ctx = (camera_transmitter_t *)data;
#ifdef WITH_FPS_CHECKER
    fps_checker_t fps_checker;
    init_fps_checker(&fps_checker);
#endif // WITH_FPS_CHECKER

    hm_log_debug("camera_transmitter: start sending messages\n");
    while (cam_ctx->is_working) {
#ifdef WITH_FPS_CHECKER
        update_fps_value(&fps_checker, "send camera messages");
#endif // WITH_FPS_CHECKER

        if (cam_ctx->data.data == NULL) {
            continue;
        }

        pthread_mutex_lock(&cam_ctx->lock);
        size_t sended = send(cam_ctx->connfd, cam_ctx->data.data,
                             cam_ctx->data.size, 0);
        if (cam_ctx->data.size != sended) {
            hm_log_error(
                    "camera_transmitter: in some reason cant send full message\n");
        }
        pthread_mutex_unlock(&cam_ctx->lock);
    }
    return 0;
}

camera_transmitter_t *
camera_transmitter_init(camera_transmitter_module_t *camera_transmitter_config)
{
    camera_transmitter_t *camera_transmitter =
            (camera_transmitter_t *)malloc(sizeof(camera_transmitter_t));
    if (!camera_transmitter) {
        hm_log_error("camera_transmitter: can`t init memory for module\n");
        return NULL;
    }

#if 1
    hm_log_debug(
            "camera_transmitter: \n=============== camera create socket ==============\n");

    hm_log_debug("camera_transmitter: mp ip = %s\n",
                 camera_transmitter->conn.mp_ip);
    hm_log_debug("camera_transmitter: gp ip = %s\n",
                 camera_transmitter->conn.gp_ip);

    hm_log_debug("camera_transmitter: frame mp port = %d",
                 camera_transmitter->conn.frame_mp_port);
    hm_log_debug("camera_transmitter: frame gp port = %d",
                 camera_transmitter->conn.frame_gp_port);

    hm_log_debug("camera_transmitter: ack mp port = %d",
                 camera_transmitter->conn.ack_mp_port);
    hm_log_debug("camera_transmitter: ack gp port = %d",
                 camera_transmitter->conn.ack_gp_port);
#endif
    hm_log_debug("camera_transmitter: start send camera data through tcp\n");

    // configure camera frame socket
    hm_log_debug("camera_transmitter: start configure camera frame socket\n");

    camera_transmitter->listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(INADDR_ANY),
        .sin_port = htons(camera_transmitter_config->mp_port)
    };

    bind(camera_transmitter->listenfd, (struct sockaddr *)&servaddr,
         sizeof(servaddr));

    listen(camera_transmitter->listenfd, 1);

    camera_transmitter->connfd =
            accept(camera_transmitter->listenfd, (struct sockaddr *)NULL, NULL);

    // configure camera ack socket
    hm_log_debug("camera_transmitter: start configure camera ack socket\n");

    hm_log_debug("camera_transmitter: connection accepted\n");

    pthread_create(&camera_transmitter->tid, NULL,
                   start_send_camera_data_through_tcp, camera_transmitter);
    return camera_transmitter;
}

int camera_transmitter_destroy(camera_transmitter_t *camera_transmitter)
{
    if (!camera_transmitter) {
        hm_log_error("camera_transmitter: can`t destroy, null\n");
        return -1;
    }
    camera_transmitter->is_working = false;
    pthread_join(camera_transmitter->tid, NULL);
    free(camera_transmitter);
    return 0;
}
