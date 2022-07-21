#include "json_config.h"
#include "log.h"
#include "fcntl.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include "errno.h"
#include <unistd.h>
#include <stdint.h>

#define MOTOR_SHARED_MEMORY_NAME "motor_shared_memory_name"

struct motors_data_t {
  uint32_t motor_x_gpio_pin;
  uint32_t motor_x_angle;
  uint32_t motor_y_gpio_pin;
  uint32_t motor_y_angle;
};

struct motors_data_t *motors_data;

int main() {
    set_log_level(DEBUG);
    init_json_config(JSON_CONFIG_FILE);

    print(DEBUG, "motor_initialize");

    int shm;

    shm = shm_open(MOTOR_SHARED_MEMORY_NAME, O_CREAT|O_RDWR, 0777);

    if (shm == -1) {
        print(ERROR, "shm open error, errno = %s", strerror(errno));
        return -1;
    }

    motors_data = (struct motors_data_t *) mmap(0, sizeof(struct motors_data_t), PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);

    while (1) {
        print(INFO, "%u, %u, %u, %u", motors_data->motor_x_angle, motors_data->motor_x_gpio_pin,
        motors_data->motor_y_angle, motors_data->motor_y_gpio_pin);
        sleep(1);
    }

    return 0;
}