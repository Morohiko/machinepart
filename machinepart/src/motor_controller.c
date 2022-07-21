#include "assert.h"
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#include "log.h"
#include "motor.h"
#include "config.h"
#include "motor_controller.h"
#include "json_config.h"

static int motor_controller_init(struct motors_controller_data *data) {
  int retval = 0;

  retval = motor_initialize(json_config.modules.motor_module.motor_delay_ms);

  if (retval != 0) {
    print(ERROR, "motor initialization error");
    return -1;
  }

  // TODO: move config to main()
  data->motor_x_gpio_pin = json_config.modules.motor_module.motor_x_gpio_pin;
  data->motor_y_gpio_pin = json_config.modules.motor_module.motor_y_gpio_pin;
  //

  motor_pin_mode(data->motor_x_gpio_pin, OUTPUT);
  motor_pin_mode(data->motor_y_gpio_pin, OUTPUT);

  print(INFO, "wiring pi installed");
  return 0;
}

static int angle_to_delay_microseconds(int angle, int *dest_mcs) {
  assert(dest_mcs);

  if (!angle)
    return -1;

  *dest_mcs = 2000 / 180 * angle + 500;

  return 0;
}

static int set_motor_angle(int motor_pin, int motor_angle) {
  int mcs;

  // invert if motor pin X
  if (motor_pin == json_config.modules.motor_module.motor_y_gpio_pin) {
    motor_angle = (motor_angle - 180) * -1;
  }

  angle_to_delay_microseconds(motor_angle, &mcs);


  motor_digital_write(motor_pin, HIGH);
  motor_delay_microseconds(mcs);
  motor_digital_write(motor_pin, LOW);
  motor_delay(json_config.modules.motor_module.motor_delay_ms);

  return 0;
}

int start_configure_motors_angle(struct motors_controller_data *data) {
  assert(data);
  int retval = 0;
  retval = motor_controller_init(data);

  if (retval != 0) {
    print(ERROR, "cant initialize motor controller");
    return -1;
  }

  data->is_working = true;
  data->motor_x_angle = 90;
  data->motor_y_angle = 90;

  while (data->is_working) {
    // TODO:
    // motor_angle_update(data->motor_x_gpio_pin, data->motor_x_angle,
    //                    data->motor_y_gpio_pin, data->motor_y_angle);
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
