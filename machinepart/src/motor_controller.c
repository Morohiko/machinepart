#include "assert.h"
#include "log.h"
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#ifdef BUILD_ON_ARM

#include "wiringPi.h"
#endif
#include "config.h"
#include "motor_controller.h"

static int motor_controller_init(struct motors_controller_data *data) {
#ifdef BUILD_ON_ARM
  if (wiringPiSetup()) {
    print(ERROR, "cannot setup wiring pi");
    return -1;
  }

  if (wiringPiSetupGpio()) {
    print(ERROR, "cannot setup wiring pi gpio");
    return -1;
  }
  if (wiringPiSetupSys()) {
    print(ERROR, "cannot setup wiring pi sys");
    return -1;
  }
#endif
  // TODO: move config to main()
  data->motor_x_gpio_pin = MOTOR_X_GPIO_PIN;
  data->motor_y_gpio_pin = MOTOR_Y_GPIO_PIN;
  //
#ifdef BUILD_ON_ARM
  pinMode(data->motor_x_gpio_pin, OUTPUT);
  pinMode(data->motor_y_gpio_pin, OUTPUT);

  print(INFO, "wiring pi installed");
#endif
  return 0;
}

static int angle_to_delay_microseconds(int angle, int *dest_mcs) {
  assert(dest_mcs);

  if (!angle)
    return -1;

  *dest_mcs = 2000 / 180 * angle + 500;
}

static int set_motor_angle(int motor_pin, int motor_angle) {
  int mcs;

  // invert if motor pin X
  if (motor_pin == MOTOR_Y_GPIO_PIN) {
    motor_angle = (motor_angle - 180) * -1;
  }

  angle_to_delay_microseconds(motor_angle, &mcs);

  print(DEBUG, "motor_pin = %d mcs = %d", motor_pin, mcs);
#ifdef BUILD_ON_ARM
  digitalWrite(motor_pin, HIGH);
  delayMicroseconds(mcs);
  digitalWrite(motor_pin, LOW);
  delay(MOTORS_DELAY_MS);
#endif
}

int start_configure_motors_angle(struct motors_controller_data *data) {
  assert(data);
  int retval = 0;
  retval = motor_controller_init(data);
  assert(!retval);
  data->is_working = true;
  data->motor_x_angle = 90;
  data->motor_y_angle = 90;

  while (data->is_working) {
    set_motor_angle(data->motor_x_gpio_pin, data->motor_x_angle);

    set_motor_angle(data->motor_y_gpio_pin, data->motor_y_angle);
  }
  return 0;
}

int stop_configure_motor_angle(struct motors_controller_data *data) {
  assert(data);
  data->is_working = false;
  return 0;
}
