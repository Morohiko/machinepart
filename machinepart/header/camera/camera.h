#include <stdbool.h>

#include "config.h"
#include "core.h"

struct camera_data {
    int size;
    int data[307200];
};

struct camera_ctx {
    bool isWorking;
    bool isBusy;
    bool isNewData;
    struct connection_info conn;
    struct udp_socket sock;
    struct camera_data data;
};


int start_send_camera_data_through_udp(struct camera_ctx *ctx);
int stop_send_camera_data_through_udp(struct camera_ctx *ctx);

int start_recv_camera_data_through_udp(struct camera_ctx *ctx);
int stop_recv_camera_data_through_udp(struct camera_ctx *ctx);
