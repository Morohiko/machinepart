#ifndef HM_MOTOR_SHM_H
#define HM_MOTOR_SHM_H

#include <stdint.h>

typedef struct {
    uint16_t angle_x;
    uint16_t angle_y;
    uint8_t pin_x;
    uint8_t pin_y;
} platform_motor_t;

platform_motor_t *motor_init(uint8_t pin_x, uint8_t pin_y);
int motor_deinit(platform_motor_t *motor);
int motor_set_angle(platform_motor_t *motor, uint16_t angle_x,
                    uint16_t angle_y);

#endif // HM_MOTOR_SHM_H