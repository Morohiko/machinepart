#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "fps_checker.h"
#include "wifi.h"
#include "core.h"
#include "log.h"
#include "camera/camera_transmitter.h"

#define ACK_MESSAGE_SIZE 4

#ifdef SEND_CAMERA_DATA_OVER_UDP
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
    assert(ctx); assert(ctx->data.data);
    while(ctx->isWorking) {
        if (!ctx->isNewData) {
            print(DEBUG, "it is not new data, skip");
            continue;
        }

        if (ctx->isBusy) {
            print(DEBUG, "cannot send data, camera is busy");

            continue;
        }
        ctx->isBusy = true;

        memcpy((int *) data, &ctx->data.data[0], ctx->data.size);

        if (send_udp_message(&ctx->sock, data, ctx->data.size) < 0) {
            print(ERROR, "cannot send message");
            break;
        }
        ctx->isBusy = false;
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
#endif // SEND_CAMERA_DATA_OVER_UDP

int start_send_camera_data_through_tcp(struct camera_ctx *ctx) {
#if 1
    print(DEBUG, "\n=============== camera create socket ==============");

    print(DEBUG, "local ip = %s", ctx->conn.local_ip);
    print(DEBUG, "target ip = %s", ctx->conn.target_ip);

    print(DEBUG, "frame local port = %d", ctx->conn.frame_local_port);
    print(DEBUG, "frame target port = %d", ctx->conn.frame_target_port);

    print(DEBUG, "ack local port = %d", ctx->conn.ack_local_port);
    print(DEBUG, "ack target port = %d", ctx->conn.ack_target_port);
#endif
    print(DEBUG, "start send camera data through tcp");
    ctx->isWorking = true;

// configure camera frame socket
    print(DEBUG, "start configure camera frame socket");

    if (create_tcp_socket(&ctx->camera_tcp_sock_frame,
               LOCAL_CAMERA_FRAME_PORT) != 0) {
        print(ERROR, "cannot create socket");
        return -1;
    }

    print(DEBUG, "socket created");
    if (listen_tcp_connection(&ctx->camera_tcp_sock_frame, MAX_TCP_CONNECTION) != 0) {
        print(ERROR, "cannot start tcp listen connection");
        return -1;
    }

    print(DEBUG, "listen tcp connection");
    if (accept_tcp_connection(&ctx->camera_tcp_sock_frame) != 0) {
        print(ERROR, "cannot accept connection");
        return -1;
    }

// configure camera ack socket
    print(DEBUG, "start configure camera ack socket");

    if (create_tcp_socket(&ctx->camera_tcp_sock_ack,
               LOCAL_CAMERA_ACK_PORT) != 0) {
        print(ERROR, "cannot create socket");
        return -1;
    }

    print(DEBUG, "socket created");
    if (listen_tcp_connection(&ctx->camera_tcp_sock_ack, MAX_TCP_CONNECTION) != 0) {
        print(ERROR, "cannot start tcp listen connection");
        return -1;
    }

    print(DEBUG, "listen tcp connection");
    if (accept_tcp_connection(&ctx->camera_tcp_sock_ack) != 0) {
        print(ERROR, "cannot accept connection");
        return -1;
    }


    print(DEBUG, "connection accepted");

#ifdef WITH_FPS_CHECKER
    struct fps_checker_t fps_checker;
    init_fps_checker(&fps_checker);
#endif // WITH_FPS_CHECKER

    print(DEBUG, "start sending messages");
    char ack_message[ACK_MESSAGE_SIZE];
    for(;;) {

#ifdef WITH_FPS_CHECKER
        update_fps_value(&fps_checker, "send camera messages");
#endif // WITH_FPS_CHECKER

        if (ctx->data.data == NULL) {
            print(ERROR, "msg is empty, size = %zu", ctx->data.size);
        sleep(1);
        continue;
        }
#ifdef SEND_MTU_SIZE_MESSAGES
        if (send_tcp_message(&ctx->camera_tcp_sock, ctx->data.data, MTU_SIZE_MESSAGE) < 0) {

#else
        if (send_tcp_message(&ctx->camera_tcp_sock_frame, ctx->data.data, ctx->data.size) < 0) {
#endif
            print(ERROR, "cannot send message");
            break;
        }

//    print(DEBUG, "sended message");

// receive acknowledge
        while (recv_tcp_message(&ctx->camera_tcp_sock_ack, ack_message, ACK_MESSAGE_SIZE)) {
            print(DEBUG, "ack received, message: %s", ack_message);
            sleep(1);

        }
//        print(DEBUG, "ack received, message: %s", ack_message);
    }

    return 0;
}

