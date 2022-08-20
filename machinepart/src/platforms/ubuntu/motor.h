#ifndef MOTOR_H
#define MOTOR_H

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

int motor_initialize(int pwm_width_mcs);

int motor_pin_mode(int pin, int mode);

int motor_digital_write(int pin, int mode);
int motor_delay_microseconds(int mcs);
int motor_digital_write(int pin, int mode);
int motor_delay(int ms);

int motor_angle_update(int motor_pin_x, int motor_angle_x, int motor_pin_y,
                       int motor_angle_y);

#endif // MOTOR_H