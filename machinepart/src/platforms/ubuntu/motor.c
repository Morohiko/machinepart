#include "motor.h"
#include "errno.h"
#include "fcntl.h"
#include "log.h"
#include "stdio.h"
#include "unistd.h"
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MOTOR_SHARED_MEMORY_NAME "motor_shared_memory_name"
static int pwm_width;

struct motors_data_t {
  uint32_t motor_x_gpio_pin;
  uint32_t motor_x_angle;
  uint32_t motor_y_gpio_pin;
  uint32_t motor_y_angle;
};

struct motors_data_t *motors_data;

int motor_initialize(int pwm_width_mcs) {
  print(DEBUG, "motor_initialize");
  pwm_width = pwm_width_mcs;

  int shm;

  shm = shm_open(MOTOR_SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0777);
  printf("errno = %s\n", strerror(errno));
  if (shm == -1) {
    print(ERROR, "shm open error, errno = %s", strerror(errno));
    return -1;
  }

  ftruncate(shm, sizeof(struct motors_data_t));

  motors_data =
      (struct motors_data_t *)mmap(0, sizeof(struct motors_data_t),
                                   PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
  return 0;
}

int motor_deinitialize() {
  int retval = 0;
  retval = shm_unlink(MOTOR_SHARED_MEMORY_NAME);
  if (retval == -1) {
    print(ERROR, "cant unlink shm in motor_deinitialize");
    return -1;
  }
  return 0;
}

int motor_pin_mode(int pin, int mode) {
  print(TRACE, "motor_pin_mode");
  return 0;
}

int motor_digital_write(int pin, int mode) {
  print(TRACE, "motor_digital_write");
  return 0;
}

int motor_delay_microseconds(int mcs) {
  print(TRACE, "motor_delay_microseconds");
  return 0;
}

int motor_delay(int ms) {
  print(TRACE, "motor_delay");
  return 0;
}

int motor_angle_update(int motor_pin_x, int motor_angle_x, int motor_pin_y,
                       int motor_angle_y) {
  motors_data->motor_x_gpio_pin = motor_pin_x;
  motors_data->motor_x_angle = motor_angle_x;
  motors_data->motor_y_gpio_pin = motor_pin_y;
  motors_data->motor_y_angle = motor_angle_y;
  return 0;
}