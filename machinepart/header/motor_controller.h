#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "stdbool.h"

struct motors_controller_data {
    bool is_working;

    int motor_x_gpio_pin;
    int motor_x_angle;

    int motor_y_gpio_pin;
    int motor_y_angle;
};

int start_configure_motors_angle(struct motors_controller_data *data);

int stop_configure_motor_angle(struct motors_controller_data *data);

#endif // MOTOR_CONTROLLER_H
