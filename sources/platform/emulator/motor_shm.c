#include <motor_shm.h>
#include <errno.h>
#include <fcntl.h>
#include <log.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MOTOR_SHARED_MEMORY_NAME "motor_shared_memory_name"

// typedef struct {
//     // int fd;
//     int pin;
//     int angle;
// } platform_motor_t;

typedef struct {
    uint32_t motor_x_pin;
    uint32_t motor_x_angle;
    uint32_t motor_y_pin;
    uint32_t motor_y_angle;
} motors_data_t;

motors_data_t *motors_data;

platform_motor_t *motor_init(uint8_t pin_x, uint8_t pin_y)
{
    hm_log_debug("motor: motor_initialize\n");
    platform_motor_t *ctx =
            (platform_motor_t *)malloc(sizeof(platform_motor_t));
    if (!ctx) {
        hm_log_error("motor: can`t init motor, mem err\n");
        return NULL;
    }
    ctx->pin_x = pin_x;
    ctx->pin_y = pin_y;
    int shm;

    shm = shm_open(MOTOR_SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0777);
    if (shm == -1) {
      hm_log_error("motor: shm open error, errno = %s\n", strerror(errno));
      return NULL;
    }

    ftruncate(shm, sizeof(motors_data_t));

    motors_data =
        (motors_data_t *)mmap(0, sizeof(motors_data_t),
                                    PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
    return ctx;
}

int motor_deinit(platform_motor_t *motor)
{
    int retval = 0;
    retval = shm_unlink(MOTOR_SHARED_MEMORY_NAME);
    if (retval == -1) {
      hm_log_error("motor: cant unlink shm in motor_deinitialize\n");
      return -1;
    }
    if (!motor) {
        hm_log_error("motor: platform_motor_t is not inited\n");
        return -1;
    }
    free(motor);
    return 0;
}

int motor_set_angle(platform_motor_t *motor, uint16_t angle_x, uint16_t angle_y)
{
    motor->angle_x = angle_x;
    motor->angle_y = angle_y;
    return 0;
}
