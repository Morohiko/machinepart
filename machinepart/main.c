#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "controller.h"
#include "core.h"
#include "json_config.h"
#include "log.h"
#include "utils.h"
#include "wifi.h"

static int configure_network(struct connection_info *conn_info_controller,
                             struct connection_info *conn_info_gyroscope,
                             struct connection_info_cam *conn_info_camera) {
  assert(conn_info_gyroscope);
  assert(conn_info_camera);

  int retval = 0;
  char ip_addr[16];

#if 0 // TODO: some issue
#ifdef GET_CUSTOM_IP_FROM_CONFIG
  memcpy(ip_addr, json_config.modules.main_module.target_ip, 16);
#else  // get ip from /proc/net/arp
  retval = get_target_ip_addr(ip_addr);
  if (retval != 0) {
    print(ERROR, "cannot get target ip addr, retval: %d", retval);
    return retval;
  }
#endif // GET_CUSTOM_IP_FROM_CONFIG
#endif
  // // #ifdef REMOTE_CONTROLLER
  //     memcpy(conn_info_controller->local_ip,
  //     json_config.modules.main_module.local_ip, 16);
  //     memcpy(conn_info_controller->target_ip, ip_addr, 16);

  //     assert(conn_info_controller->local_ip);
  //     assert(conn_info_controller->target_ip);

  //     conn_info_controller->local_port = LOCAL_CONTROLLER_PORT;
  //     conn_info_controller->target_port = TARGET_CONTROLLER_PORT;

  //     print(INFO, "selected controller connection:\n local ip: %s, target ip:
  //     %s, local port: %d, target port: %d",
  //                 conn_info_controller->local_ip,
  //                 conn_info_controller->target_ip,
  //                 conn_info_controller->local_port,
  //                 conn_info_controller->target_port);
  // // #endif // REMOTE_CONTROLLER

#ifdef ENABLE_GYROSCOPE_RECEIVER
  memcpy(conn_info_gyroscope->local_ip,
         json_config.modules.main_module.local_ip, 16);
  memcpy(conn_info_gyroscope->target_ip, json_config.modules.main_module.target_ip, 16);

  assert(conn_info_gyroscope->local_ip);
  assert(conn_info_gyroscope->target_ip);

  conn_info_gyroscope->local_port =
      json_config.modules.gyroscope_receiver_module.local_port;
  conn_info_gyroscope->target_port =
      json_config.modules.gyroscope_receiver_module.target_port;
  print(INFO,
        "selected gyroscope connection:\n local ip: %s, target ip: %s, local "
        "port: %d, target port: %d",
        conn_info_gyroscope->local_ip, conn_info_gyroscope->target_ip,
        conn_info_gyroscope->local_port, conn_info_gyroscope->target_port);
#endif // ENABLE_GYROSCOPE_RECEIVER

#ifdef ENABLE_CAMERA
  // install connection info for camera frame
  memcpy(conn_info_camera->local_ip, json_config.modules.main_module.local_ip,
         16);
  memcpy(conn_info_camera->target_ip, ip_addr, 16);

  assert(conn_info_camera->local_ip);
  assert(conn_info_camera->target_ip);

  // configure camera frame port
  conn_info_camera->frame_local_port =
      json_config.modules.camera_transmitter_module.local_port;
  conn_info_camera->frame_target_port =
      json_config.modules.camera_transmitter_module.target_port;
  // configure camera ack port
  conn_info_camera->ack_local_port =
      json_config.modules.camera_transmitter_module.local_ack_port;
  conn_info_camera->ack_target_port =
      json_config.modules.camera_transmitter_module.target_ack_port;

  print(INFO,
        "selected camera frame connection:\n local ip: %s, target ip: %s, "
        "frame local port: %d, frame target port: %d, ack local port: %d, ack "
        "target port: %d",
        conn_info_camera->local_ip, conn_info_camera->target_ip,
        conn_info_camera->frame_local_port, conn_info_camera->frame_target_port,
        conn_info_camera->ack_local_port, conn_info_camera->ack_target_port);
#endif // ENABLE_CAMERA
  return 0;
}

int main() {
  set_log_level(DEBUG);
  init_json_config(JSON_CONFIG_FILE);
  add_command(print_json_config, "config_show", "show json config");
  add_command(set_modules_camera_state, "set_modules_camera_state",
              "Set module camera state");
  enable_log_with_module_names();
  register_log_module(json_config.modules.main_module.name, pthread_self());
  init_signals();
  print(INFO, "======== Start Machinepart =======");

  print(INFO, "======== configure network =======");

  struct connection_info conn_info_controller;
  struct connection_info conn_info_gyroscope;
  struct connection_info_cam conn_info_camera;

  configure_network(&conn_info_controller, &conn_info_gyroscope,
                    &conn_info_camera);

  print(DEBUG, "start remote controller");

  machine_controller controller;

  init_machine_controller_states(&controller);
#ifdef REMOTE_CONTROLLER
  controller.conn = conn_info_controller;
#endif // REMOTE_CONTROLLER
  start_remote_controller(&controller);

  core_loop(&controller, &conn_info_gyroscope, &conn_info_camera);

  return 0;
}
