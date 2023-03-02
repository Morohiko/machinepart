#ifndef HM_MOTOR_CONTROLLER_H
#define HM_MOTOR_CONTROLLER_H

#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>

#include <motor_shm.h>
#include <json_config.h>

typedef struct {
    bool is_working;
    pthread_t tid;
    platform_motor_t *platform_motor;
} motor_controller_t;

motor_controller_t *start_motor_controller(motor_module_t *motor_config);

int stop_motor_controller(motor_controller_t *data);

#endif // HM_MOTOR_CONTROLLER_H
