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

// parse in format: X:Y
static int parse_gyroscope_data(char *msg,
                                struct motors_controller_data *motor_data) {
  int x = 0;
  int y = 0;

  int parsed = 0;

  parsed = sscanf(msg, "%d:%d", &x, &y);

  if (parsed != 2) {
    print(ERROR, "parsed: %d, must be 2", parsed);
    return -1;
  }

  motor_data->motor_x_angle = x;
  motor_data->motor_y_angle = y;

  print(DEBUG, "parsed gyroscope data: x: %d, y: %d", x, y);

  return 0;
}

int start_receive_gyroscope_data(struct gyroscope_ctx *ctx) {
  int retval = 0;

  ctx->is_working = true;

  retval = create_udp_socket(&ctx->sock, ctx->conn.mp_ip, ctx->conn.gp_ip,
                             ctx->conn.mp_port, ctx->conn.gp_port);

  if (retval != 0) {
    print(ERROR, "cannot create socket");
    return -1;
  }

  char msg[MAX_GYROSCOPE_DATA_SIZE];

  while (ctx->is_working) {
    retval = recv_udp_message(&ctx->sock, msg, MAX_GYROSCOPE_DATA_SIZE);

    if (retval != 0) {
      print(ERROR, "cannot recv message");
      break;
    }

    if (msg == NULL) {
      continue;
    }

    retval = parse_gyroscope_data(msg, &ctx->motor_data);

    if (retval != 0) {
      print(ERROR, "cant parse gyroscope data, msg: %s", msg);
      memset(msg, '\0', MAX_GYROSCOPE_DATA_SIZE);
    }
  }
}

int stop_receive_gyroscope_data(struct gyroscope_ctx *ctx) {
  int retval = 0;

  ctx->is_working = false;

  sleep(1);

  retval = close_udp_socket(&ctx->sock);

  if (retval != 0) {
    print(ERROR, "cannot close socket");
    return -1;
  }

  return 0;
}
