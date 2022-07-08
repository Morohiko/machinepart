#ifndef GYROSCOPE_RECEIVER_H
#define GYROSCOPE_RECEIVER_H

#include "stdbool.h"

#include "core.h"
#include "motor_controller.h"
#include "wifi.h"

struct gyroscope_ctx {
  bool is_working;
  struct connection_info conn;
  struct udp_socket sock;
  struct motors_controller_data motor_data;
};

int start_receive_gyroscope_data();

int stop_receive_gyroscope_data();

#endif // GYROSCOPE_RECEIVER_H
