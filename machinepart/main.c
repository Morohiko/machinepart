#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "controller.h"
#include "core.h"
#include "json_config.h"
#include "log.h"
#include "shell_server.h"
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
  memcpy(ip_addr, json_config.modules.main_module.gp_ip, 16);
#else  // get ip from /proc/net/arp
  retval = get_gp_ip_addr(ip_addr);
  if (retval != 0) {
    print(ERROR, "cannot get gp ip addr, retval: %d", retval);
    return retval;
  }
#endif // GET_CUSTOM_IP_FROM_CONFIG
#endif
  // // #ifdef REMOTE_CONTROLLER
  //     memcpy(conn_info_controller->mp_ip,
  //     json_config.modules.main_module.mp_ip, 16);
  //     memcpy(conn_info_controller->gp_ip, ip_addr, 16);

  //     assert(conn_info_controller->mp_ip);
  //     assert(conn_info_controller->gp_ip);

  //     conn_info_controller->mp_port = LOCAL_CONTROLLER_PORT;
  //     conn_info_controller->gp_port = TARGET_CONTROLLER_PORT;

  //     print(INFO, "selected controller connection:\n mp ip: %s, gp ip:
  //     %s, mp port: %d, gp port: %d",
  //                 conn_info_controller->mp_ip,
  //                 conn_info_controller->gp_ip,
  //                 conn_info_controller->mp_port,
  //                 conn_info_controller->gp_port);
  // // #endif // REMOTE_CONTROLLER

#ifdef ENABLE_GYROSCOPE_RECEIVER
  memcpy(conn_info_gyroscope->mp_ip, json_config.modules.main_module.mp_ip, 16);
  memcpy(conn_info_gyroscope->gp_ip, json_config.modules.main_module.gp_ip, 16);

  assert(conn_info_gyroscope->mp_ip);
  assert(conn_info_gyroscope->gp_ip);

  conn_info_gyroscope->mp_port =
      json_config.modules.gyroscope_receiver_module.mp_port;
  conn_info_gyroscope->gp_port =
      json_config.modules.gyroscope_receiver_module.gp_port;
  print(INFO,
        "selected gyroscope connection:\n mp ip: %s, gp ip: %s, local "
        "port: %d, gp port: %d",
        conn_info_gyroscope->mp_ip, conn_info_gyroscope->gp_ip,
        conn_info_gyroscope->mp_port, conn_info_gyroscope->gp_port);
#endif // ENABLE_GYROSCOPE_RECEIVER

#ifdef ENABLE_CAMERA
  // install connection info for camera frame
  memcpy(conn_info_camera->mp_ip, json_config.modules.main_module.mp_ip, 16);
  memcpy(conn_info_camera->gp_ip, json_config.modules.main_module.gp_ip, 16);

  assert(conn_info_camera->mp_ip);
  assert(conn_info_camera->gp_ip);

  // configure camera frame port
  conn_info_camera->frame_mp_port =
      json_config.modules.camera_transmitter_module.mp_port;
  conn_info_camera->frame_gp_port =
      json_config.modules.camera_transmitter_module.gp_port;
  // configure camera ack port
  conn_info_camera->ack_mp_port =
      json_config.modules.camera_transmitter_module.local_ack_port;
  conn_info_camera->ack_gp_port =
      json_config.modules.camera_transmitter_module.target_ack_port;

  print(INFO,
        "selected camera frame connection:\n mp ip: %s, gp ip: %s, "
        "frame mp port: %d, frame gp port: %d, ack mp port: %d, ack "
        "gp port: %d",
        conn_info_camera->mp_ip, conn_info_camera->gp_ip,
        conn_info_camera->frame_mp_port, conn_info_camera->frame_gp_port,
        conn_info_camera->ack_mp_port, conn_info_camera->ack_gp_port);
#endif // ENABLE_CAMERA
  return 0;
}

static int init_shell_server_commands() {
  add_command(print_json_config, "config_show", "show json config");
  add_command(start_gyroscope_receiver, "start_gyroscope_receiver",
              "Start gyroscope receiver module");
  add_command(stop_gyroscope_receiver, "stop_gyroscope_receiver",
              "Stop gyroscope receiver module");
  add_command(start_motor_controller, "start_motor_controller",
              "Start motor controller module");
  add_command(stop_motor_controller, "stop_motor_controller",
              "Stop motor controller module");
  add_command(start_camera_module, "start_camera_module",
              "Start camera module");
  add_command(stop_camera_module, "stop_camera_module", "Stop camera module");
  add_command(start_camera_transmitter, "start_camera_transmitter",
              "Start camera transmitter module");
  return 0;
}

int main() {
  int retval = 0;
  set_log_level(DEBUG);
  retval = init_json_config(JSON_CONFIG_FILE);
  if (retval != 0) {
    print(ERROR, "cant init json config path: %s", JSON_CONFIG_FILE);
    return 0;
  }
  enable_log_with_module_names();
  register_log_module(json_config.modules.main_module.name, pthread_self());
  init_signals();
  init_shell_server_commands();
  print(INFO, "======== Start Machinepart =======");

  print(INFO, "======== configure network =======");

  struct connection_info conn_info_controller;
  struct connection_info conn_info_gyroscope;
  struct connection_info_cam conn_info_camera;

  configure_network(&conn_info_controller, &conn_info_gyroscope,
                    &conn_info_camera);

  print(DEBUG, "start remote controller");

  init_modules();

  machine_controller controller;

  init_machine_controller_states(&controller);
#ifdef REMOTE_CONTROLLER
  controller.conn = conn_info_controller;
#endif // REMOTE_CONTROLLER
  start_remote_controller(&controller);

  core_loop(&controller, &conn_info_gyroscope, &conn_info_camera);

  return 0;
}
