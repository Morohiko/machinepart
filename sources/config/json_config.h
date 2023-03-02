#ifndef HM_JSON_CONFIG_H
#define HM_JSON_CONFIG_H

#include <stddef.h>

#define MODULES_MAIN_NAME_STR_SIZE 32u
#define MODULES_MAIN_MP_IP_STR_SIZE 32u
#define MODULES_MAIN_GP_IP_STR_SIZE 32u

#define MODULES_CAMERA_NAME_STR_SIZE 32u

#define MODULES_CAMERA_TRANSMITTER_NAME_STR_SIZE 32u

#define MODULES_GYROSCOPE_RECEIVER_NAME_STR_SIZE 32u

#define MODULES_MOTOR_NAME_STR_SIZE 32u

#define MODULES_REMOTE_CONTROLLER_NAME_STR_SIZE 32u

typedef struct {
    int id;
    char name[MODULES_MAIN_NAME_STR_SIZE];
    char mp_ip[MODULES_MAIN_MP_IP_STR_SIZE];
    char gp_ip[MODULES_MAIN_GP_IP_STR_SIZE];
} main_module_t;

typedef struct {
    int id;
    char name[MODULES_CAMERA_NAME_STR_SIZE];
    int state;
    int frame_width;
    int frame_height;
    int frame_elem_size;
} camera_module_t;

typedef struct {
    int id;
    char name[MODULES_CAMERA_TRANSMITTER_NAME_STR_SIZE];
    int state;
    int mp_port;
    int gp_port;
    int local_ack_port;
    int target_ack_port;
} camera_transmitter_module_t;

typedef struct {
    int id;
    char name[MODULES_GYROSCOPE_RECEIVER_NAME_STR_SIZE];
    int state;
    int mp_port;
    int gp_port;
} gyroscope_receiver_module_t;

typedef struct {
    int id;
    char name[MODULES_MOTOR_NAME_STR_SIZE];
    int state;
    int motor_delay_ms;
    int motor_x_pin;
    int motor_y_pin;
} motor_module_t;

typedef struct {
    int id;
    char name[MODULES_REMOTE_CONTROLLER_NAME_STR_SIZE];
} remote_controller_module_t;

typedef struct {
    main_module_t main_module;
    camera_module_t camera_module;
    camera_transmitter_module_t camera_transmitter_module;
    gyroscope_receiver_module_t gyroscope_receiver_module;
    motor_module_t motor_module;
    remote_controller_module_t remote_controller_module;
} modules_t;

typedef struct {
    int state;
    int time_to_update;
} fps_check_t;

typedef struct {
    int state;
    int mgmt_port;
    int port;
    int buffer_size;
    int local_shell_client;
} shell_t;

typedef struct {
    modules_t modules;
    fps_check_t fps_check;
    shell_t shell;
} json_config_t;

json_config_t *init_json_config(char *json_filepath);

int destroy_json_config(json_config_t *json_config);

void print_json_config();

void config_start_gyroscope_receiver(void *data);
void config_stop_gyroscope_receiver(void *data);

void config_start_motor_controller(void *data);
void config_stop_motor_controller(void *data);

void config_start_camera_module(void *data);
void config_stop_camera_module(void *data);

void config_start_camera_transmitter(void *data);

#endif // HM_JSON_CONFIG_H
