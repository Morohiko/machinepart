#include "camera/camera.h"
#include "json_config.h"
#include "log.h"
#include "wifi.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

static void *camera_receiver_thread(void *user_data) {
  struct camera_ctx *cam_ctx = (struct camera_ctx *)user_data;

  start_recv_camera_data_through_udp(cam_ctx);
}

int config_network(struct connection_info *conn_info) {
  memcpy(conn_info->mp_ip, json_config.modules.main_module.mp_ip, 16);
  memcpy(conn_info->gp_ip, json_config.modules.main_module.mp_ip, 16);

  assert(conn_info->mp_ip);
  assert(conn_info->gp_ip);

  // inverted cause tested on local machine
  conn_info->mp_port = json_config.modules.gyroscope_receiver_module.gp_port;
  conn_info->gp_port = json_config.modules.gyroscope_receiver_module.mp_port;
  print(INFO,
        "selected connection:\n mp ip: %s, gp ip: %s, mp port: %d, "
        "gp port: %d",
        conn_info->mp_ip, conn_info->gp_ip, conn_info->mp_port,
        conn_info->gp_port);

  return 0;
}

int main(int argc, char **argv) {
  set_log_level(DEBUG);

  struct camera_ctx cam_ctx;

  config_network(&cam_ctx.conn);

  pthread_t cameraReceiverThreadID;
  pthread_create(&cameraReceiverThreadID, NULL, camera_receiver_thread,
                 &cam_ctx);

  sleep(99999);

  return 0;
}
