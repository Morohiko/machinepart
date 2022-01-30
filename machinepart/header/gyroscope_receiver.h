#ifndef GYROSCOPE_RECEIVER_H
#define GYROSCOPE_RECEIVER_H

#include "stdbool.h"

#include "core.h"
#include "motor_controller.h"
#include "wifi.h"

struct gyroscope_data {
  int x;
  int y;
  int z;
};

struct gyroscope_ctx {
  bool is_working;
  struct connection_info conn;
  struct udp_socket sock;
  struct gyroscope_data data;
  struct motors_controller_data motor_data;
};

int start_receive_gyroscope_data();

int stop_receive_gyroscope_data();

// deprecated
int recv_gyroscope_data(struct gyroscope_ctx *ctx);

#endif // GYROSCOPE_RECEIVER_H
