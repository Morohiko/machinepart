#include <assert.h>
#include <unistd.h>

#include "config.h"
#include "gyroscope_receiver.h"
#include "json_config.h"
#include "log.h"
#include "motor_controller.h"
#include "string.h"
#include "wifi.h"

#define MAX_GYROSCOPE_DATA_SIZE 8

// parse in format: X:Y:Z
static int parse_gyroscope_data(char *msg, struct gyroscope_data *data,
                                struct motors_controller_data *motor_data) {
  int x = 0;
  int y = 0;

  int parsed = 0;

  parsed = sscanf(msg, "%d:%d", &x, &y);

  if (parsed != 2) {
    print(ERROR, "parsed: %d, must be 2", parsed);
    return -1;
  }

  // TODO: remove functionality, deprecated
  data->x = x;
  data->y = y;
  //

  motor_data->motor_x_angle = x;
  motor_data->motor_y_angle = y;

  print(DEBUG, "parsed gyroscope data: x: %d, y: %d", data->x, data->y);

  return 0;
}

int start_receive_gyroscope_data(struct gyroscope_ctx *ctx) {
#if 0
camera("=============== create socket ==============");
    print("local ip = %s", ctx->conn.local_ip);
    print("target ip = %s", ctx->conn.target_ip);
    print("local port = %d", ctx->conn.local_port_gyroscope);
    print("target port = %d", ctx->conn.target_port_gyroscope);
#endif
  ctx->is_working = true;

  if (create_udp_socket(&ctx->sock, ctx->conn.local_ip, ctx->conn.target_ip,
                        ctx->conn.local_port, ctx->conn.target_port) != 0) {
    print(ERROR, "cannot create socket");
    return -1;
  }

  char msg[MAX_GYROSCOPE_DATA_SIZE];
  int try
    = 0;
  while (ctx->is_working) {
    if (recv_udp_message(&ctx->sock, msg, MAX_GYROSCOPE_DATA_SIZE) < 0) {
      print(ERROR, "cannot recv message");
      break;
    }

    if (msg == NULL) {
      print(DEBUG, "received udp message is empty");
      print(DEBUG, "try = %d", try);

      if (try ++ == 10) {
        return -1;
      }

      continue;
    }

    //        print(DEBUG, "received udp msg: %s", msg);

    if (parse_gyroscope_data(msg, &ctx->data, &ctx->motor_data)) {
      print(ERROR, "cant parse gyroscope data, msg: %s", msg);
      memset(msg, '\0', MAX_GYROSCOPE_DATA_SIZE);
      break;
    }
  }
}

int stop_receive_gyroscope_data(struct gyroscope_ctx *ctx) {
  ctx->is_working = false;

  sleep(5);

  if (close_udp_socket(&ctx->sock)) {
    print(ERROR, "cannot close socket");
    return -1;
  }
}

// deprecated
int recv_gyroscope_data(struct gyroscope_ctx *ctx) {
  char msg[MAX_GYROSCOPE_DATA_SIZE];

  assert(msg);
  if (create_udp_socket(
          &ctx->sock, json_config.modules.main_module.local_ip,
          json_config.modules.main_module.target_ip,
          json_config.modules.gyroscope_receiver_module.local_port,
          json_config.modules.gyroscope_receiver_module.target_port) != 0) {
    print(ERROR, "cannot create socket");
    return -1;
  }

  print(DEBUG, "socket created");

  while (1) {
    //        memset(msg, '\0', MAX_GYROSCOPE_DATA_SIZE);
    if (recv_udp_message(&ctx->sock, msg, MAX_GYROSCOPE_DATA_SIZE) < 0) {
      print(ERROR, "cannot send message");
      break;
    }

    if (msg != NULL) {
      print(DEBUG, "received udp msg: %s", msg);
    }

    if (parse_gyroscope_data(msg, &ctx->data, &ctx->motor_data)) {
      print(ERROR, "cant parse gyroscope data, msg: %s", msg);
      memset(msg, '\0', MAX_GYROSCOPE_DATA_SIZE);
      break;
    }
    //        sleep(1);
  }

  if (close_udp_socket(&ctx->sock)) {
    print(ERROR, "cannot close socket");
    return -1;
  }

  print(DEBUG, "look like recv msg through udp is good working");

  return 0;
}
