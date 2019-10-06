#ifndef CAMERA_TRANSMITTER_H
#define CAMERA_TRANSMITTER_H

#include <stdbool.h>

#include "config.h"
#include "core.h"

struct camera_data {
    void *data;
    size_t size;
};

struct camera_ctx {
    bool isWorking;
    bool isBusy;
    bool isNewData;
    struct connection_info_cam conn;
    struct udp_socket sock;
    struct tcp_socket camera_tcp_sock_frame;
    struct tcp_socket camera_tcp_sock_ack;
    struct camera_data data;
};

int start_send_camera_data_through_udp(struct camera_ctx *ctx);
int stop_send_camera_data_through_udp(struct camera_ctx *ctx);

int start_recv_camera_data_through_udp(struct camera_ctx *ctx);
int stop_recv_camera_data_through_udp(struct camera_ctx *ctx);

#endif // CAMERA_TRANSMITTER_H
