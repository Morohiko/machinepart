#include "assert.h"
#include "unistd.h"
#include <pthread.h>

#include "config.h"
#include "log.h"
#include "motor_controller.h"

struct motors_controller_data motors_data;

static int create_square_via_motors() {
  for (int x = 0; x < 180; x++) {
    print(DEBUG, "motor position x must be = %d", x);
    usleep(20000);
    motors_data.motor_x_angle = x;
  }

  for (int y = 0; y < 180; y++) {
    print(DEBUG, "motor position y must be = %d", y);
    usleep(20000);
    motors_data.motor_y_angle = y;
  }

  return 0;
}

static void *start_motors(void *data) {
  start_configure_motors_angle(&motors_data);
}

int main() {
  set_log_level(DEBUG);

  pthread_t motors_thread_id;

  motors_data.motor_x_gpio_pin = json_config.modules.motor_module.motor_x_gpio_pin;

  motors_data.motor_y_gpio_pin = json_config.modules.motor_module.motor_y_gpio_pin;

  print(DEBUG, "x gpio pin = %d, y gpio pin = %d", motors_data.motor_x_gpio_pin,
        motors_data.motor_y_gpio_pin);

  pthread_create(&motors_thread_id, NULL, start_motors, NULL);

  create_square_via_motors();

  //    stop_configure_motors_angle(&motors_data);

  print(INFO, "finish rotate motor");

  return 0;
}
