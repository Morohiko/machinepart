#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "wifi.h"
#include "core.h"
#include "log.h"
#include "camera/camera.h"

int start_send_camera_data_through_udp(struct camera_ctx *ctx) {
#if 1
    print(DEBUG, "=============== create socket ==============");
    print(DEBUG, "local ip = %s", ctx->conn.local_ip);
    print(DEBUG, "target ip = %s", ctx->conn.target_ip);
    print(DEBUG, "local port = %d", ctx->conn.local_port);
    print(DEBUG, "target port = %d", ctx->conn.target_port);
#endif
    print(DEBUG, "start send camera data through udp");
    ctx->isWorking = true;

    if (create_udp_socket(&ctx->sock,
                          ctx->conn.local_ip, ctx->conn.target_ip,
                          ctx->conn.local_port, ctx->conn.target_port) != 0) {
        print(ERROR, "cannot create socket");
        return -1;
    }

    int *data = (int *) malloc(ctx->data.size);
assert(ctx);
//assert(ctx->data);
assert(ctx->data.data);
    while(ctx->isWorking) {
//print(DEBUG, "ctx is busy = %d", ctx->isBusy);

        if (!ctx->isNewData) {
            print(DEBUG, "it is not new data, skip");
sleep(1);

            continue;
        }

        if (ctx->isBusy) {
            print(DEBUG, "cannot send data, camera is busy");

            continue;
        }
        ctx->isBusy = true;

//print(DEBUG, "befor memcpu");
        memcpy((int *) data, &ctx->data.data[0], ctx->data.size);
print(DEBUG, "data = %d", *data);
//print(DEBUG, "after memcpu");

//print(DEBUG, "before send");
sleep(1);

        if (send_udp_message(&ctx->sock, data, ctx->data.size) < 0) {
            print(ERROR, "cannot send message");
            break;
        }
ctx->isNewData = false;
        ctx->isBusy = false;

print(DEBUG, "sended message through udp send");
    }

    return 0;
}

int stop_send_camera_data_through_udp(struct camera_ctx *ctx) {

    return 0;
}

int start_recv_camera_data_through_udp(struct camera_ctx *ctx) {
    ctx->isWorking = true;

    if (create_udp_socket(&ctx->sock,
                          ctx->conn.local_ip, ctx->conn.target_ip,
                          ctx->conn.local_port, ctx->conn.target_port) != 0) {
        print(ERROR, "cannot create socket");
        return -1;
    }

    int size = CAMERA_FRAME_WIDTH*CAMERA_FRAME_HEIGHT*CAMERA_FRAME_ELEM_SIZE;

    char msg[size];
    while(ctx->isWorking) {
        if (recv_udp_message(&ctx->sock, msg, size) < 0) {
            print(ERROR, "cannot recv message");
            break;
        }

        if (msg == NULL) {
            print(DEBUG, "received udp message is empty");
            continue;
        }
        print(DEBUG, "received some data through udp, camera receiver");
    }

    return 0;
}

int stop_recv_camera_data_through_udp(struct camera_ctx *ctx) {

    return 0;
}
