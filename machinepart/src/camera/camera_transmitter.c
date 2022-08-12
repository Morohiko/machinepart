#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "camera/camera_transmitter.h"
#include "core.h"
#include "fps_checker.h"
#include "json_config.h"
#include "log.h"
#include "wifi.h"

#define ACK_MESSAGE_SIZE 4

#ifdef SEND_CAMERA_DATA_OVER_UDP
int start_send_camera_data_through_udp(struct camera_ctx *cam_ctx) {
#if 1
  print(DEBUG, "=============== create socket ==============");
  print(DEBUG, "local ip = %s", cam_ctx->conn.local_ip);
  print(DEBUG, "target ip = %s", cam_ctx->conn.target_ip);
  print(DEBUG, "local port = %d", cam_ctx->conn.local_port);
  print(DEBUG, "target port = %d", cam_ctx->conn.target_port);
#endif
  print(DEBUG, "start send camera data through udp");
  int retval = 0;

  if (create_udp_socket(&cam_ctx->sock, cam_ctx->conn.local_ip,
                        cam_ctx->conn.target_ip, cam_ctx->conn.local_port,
                        cam_ctx->conn.target_port) != 0) {
    print(ERROR, "cannot create socket");
    return -1;
  }

  int *data = (int *)malloc(cam_ctx->data.size);
  assert(ctx);
  assert(cam_ctx->data.data);
  while (1) {
    if (!cam_ctx->isNewData) {
      print(DEBUG, "it is not new data, skip");
      continue;
    }

    pthread_mutex_lock(&lock);

    memcpy((int *)data, &cam_ctx->data.data[0], cam_ctx->data.size);

    retval = send_udp_message(&cam_ctx->sock, data, cam_ctx->data.size);

    pthread_mutex_unlock(&lock);
    if (retval != 0) {
      print(ERROR, "cannot send message");
      break;
    }
  }
  return 0;
}

int stop_send_camera_data_through_udp(struct camera_ctx *cam_ctx) { return 0; }

int start_recv_camera_data_through_udp(struct camera_ctx *cam_ctx) {

  if (create_udp_socket(&cam_ctx->sock, cam_ctx->conn.local_ip,
                        cam_ctx->conn.target_ip, cam_ctx->conn.local_port,
                        cam_ctx->conn.target_port) != 0) {
    print(ERROR, "cannot create socket");
    return -1;
  }

  int size = json_config.modules.camera_module.frame_width *
             json_config.modules.camera_module.frame_height *
             json_config.modules.camera_module.frame_elem_size;

  char msg[size];
  while (1) {
    if (recv_udp_message(&cam_ctx->sock, msg, size) < 0) {
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

int stop_recv_camera_data_through_udp(struct camera_ctx *ctx) { return 0; }
#endif // SEND_CAMERA_DATA_OVER_UDP

int start_send_camera_data_through_tcp(struct camera_ctx *cam_ctx) {
  int retval;
#if 1
  print(DEBUG, "\n=============== camera create socket ==============");

  print(DEBUG, "local ip = %s", cam_ctx->conn.local_ip);
  print(DEBUG, "target ip = %s", cam_ctx->conn.target_ip);

  print(DEBUG, "frame local port = %d", cam_ctx->conn.frame_local_port);
  print(DEBUG, "frame target port = %d", cam_ctx->conn.frame_target_port);

  print(DEBUG, "ack local port = %d", cam_ctx->conn.ack_local_port);
  print(DEBUG, "ack target port = %d", cam_ctx->conn.ack_target_port);
#endif
  print(DEBUG, "start send camera data through tcp");

  // configure camera frame socket
  print(DEBUG, "start configure camera frame socket");

  retval = create_tcp_socket(
      &cam_ctx->camera_tcp_sock_frame,
      json_config.modules.camera_transmitter_module.local_port);
  if (retval != 0) {
    print(ERROR, "cannot create socket");
    return retval;
  }

  print(DEBUG, "socket created");
  retval = listen_tcp_connection(&cam_ctx->camera_tcp_sock_frame,
                                 MAX_TCP_CONNECTION);
  if (retval != 0) {
    print(ERROR, "cannot start tcp listen connection");
    return retval;
  }

  print(DEBUG, "listen tcp connection");
  retval = accept_tcp_connection(&cam_ctx->camera_tcp_sock_frame);
  if (retval != 0) {
    print(ERROR, "cannot accept connection");
    return -1;
  }

  // configure camera ack socket
  print(DEBUG, "start configure camera ack socket");

  print(DEBUG, "connection accepted");

#ifdef WITH_FPS_CHECKER
  struct fps_check_t fps_checker;
  init_fps_checker(&fps_checker);
#endif // WITH_FPS_CHECKER

  print(DEBUG, "start sending messages");
  char ack_message[ACK_MESSAGE_SIZE];
  for (;;) {

#ifdef WITH_FPS_CHECKER
    update_fps_value(&fps_checker, "send camera messages");
#endif // WITH_FPS_CHECKER

    if (cam_ctx->data.data == NULL) {
      continue;
    }

    pthread_mutex_lock(&cam_ctx->lock);
    retval = send_tcp_message(&cam_ctx->camera_tcp_sock_frame,
                              cam_ctx->data.data, cam_ctx->data.size);
    pthread_mutex_unlock(&cam_ctx->lock);
    if (retval != 0) {
      print(ERROR, "cannot send message");
      break;
    }
  }
  return 0;
}

int camera_init(struct camera_ctx *cam_ctx) {
  pthread_mutex_init(&cam_ctx->lock, NULL);
  return 0;
}