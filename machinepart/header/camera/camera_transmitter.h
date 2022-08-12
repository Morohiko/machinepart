#ifndef CAMERA_TRANSMITTER_H
#define CAMERA_TRANSMITTER_H

#include "pthread.h"
#include <stdbool.h>

#include "config.h"
#include "core.h"

struct camera_data {
  void *data;
  size_t size;
  size_t max_size;
};

struct camera_ctx {
  pthread_mutex_t lock;
  bool isNewData;
  struct connection_info_cam conn;
  struct tcp_socket camera_tcp_sock_frame;
  struct camera_data data;
};

#ifdef SEND_CAMERA_DATA_OVER_UDP
int start_send_camera_data_through_udp(struct camera_ctx *cam_ctx);
int stop_send_camera_data_through_udp(struct camera_ctx *cam_ctx);

int start_recv_camera_data_through_udp(struct camera_ctx *cam_ctx);
int stop_recv_camera_data_through_udp(struct camera_ctx *cam_ctx);
#endif
int camera_init(struct camera_ctx *cam_ctx);

int start_send_camera_data_through_tcp(struct camera_ctx *cam_ctx);

#endif // CAMERA_TRANSMITTER_H
