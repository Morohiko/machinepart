#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

struct main_module_t {
  int id;
  char *name;
};

struct camera_module_t {
  int id;
  char *name;
  int state;
};

struct camera_transmitter_module_t {
  int id;
  char *name;
  int state;
};

struct gyroscope_receiver_module_t {
  int id;
  char *name;
  int state;
};

struct motor_module_t {
  int id;
  char *name;
  int state;
};

struct remote_controller_module_t {
  int id;
  char *name;
};

struct modules_t {
  struct main_module_t main_module;
  struct camera_module_t camera_module;
  struct camera_transmitter_module_t camera_transmitter_module;
  struct gyroscope_receiver_module_t gyroscope_receiver_module;
  struct motor_module_t motor_module;
  struct remote_controller_module_t remote_controller_module;
};

struct fps_checker_t {
  int time_to_update;
};

struct shell_t {
    int state;
    int mgmt_port;
    int port;
    int buffer_size;
    int local_shell_client;
};

struct json_config_t {
  struct modules_t modules;
  struct fps_checker_t fps_checker;
  struct shell_t shell;
};

struct json_config_t json_config;

void init_json_config(char *json_filepath);

#endif // JSON_CONFIG_H