#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

struct main_module_t {
  int id;
  char *name;
  char *local_ip;
  char *target_ip;
};

struct camera_module_t {
  int id;
  char *name;
  int state;
  int frame_width;
  int frame_height;
  int frame_elem_size;
};

struct camera_transmitter_module_t {
  int id;
  char *name;
  int state;
  int local_port;
  int target_port;
  int local_ack_port;
  int target_ack_port;
};

struct gyroscope_receiver_module_t {
  int id;
  char *name;
  int state;
  int local_port;
  int target_port;
};

struct motor_module_t {
  int id;
  char *name;
  int state;
  int motor_delay_ms;
  int motor_x_gpio_pin;
  int motor_y_gpio_pin;
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
  int state;
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

extern struct json_config_t json_config;

int init_json_config(char *json_filepath);

void print_json_config();

void start_gyroscope_receiver(void *data);
void stop_gyroscope_receiver(void *data);

void start_motor_controller(void *data);
void stop_motor_controller(void *data);

void start_camera_module(void *data);
void stop_camera_module(void *data);

void start_camera_transmitter(void *data);

#endif // JSON_CONFIG_H