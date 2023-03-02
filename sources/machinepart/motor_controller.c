#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <json_config.h>
#include <log.h>
#include <motor_shm.h>
#include <motor_controller.h>

void *motor_controller_thread(void *user_data)
{
    motor_controller_t *ctx = (motor_controller_t *)user_data;
    while (ctx->is_working) {
        // TODO:
        // motor_angle_update(data->motor_x_pin, data->motor_x_angle,
        //                    data->motor_y_pin, data->motor_y_angle);
        motor_set_angle(ctx->platform_motor, 90, 90);
    }
    return NULL;
}

motor_controller_t *start_motor_controller(motor_module_t *motor_config)
{
    motor_controller_t *ctx =
            (motor_controller_t *)malloc(sizeof(motor_controller_t));
    if (!ctx) {
        hm_log_error("motor_controller: can`t allocate mem\n");
        return NULL;
    }

    ctx->platform_motor =
            motor_init(motor_config->motor_x_pin, motor_config->motor_y_pin);

    motor_set_angle(ctx->platform_motor, 90, 90);

    ctx->is_working = true;
    pthread_create(&ctx->tid, NULL, motor_controller_thread, ctx);
    return ctx;
}

int stop_motor_controller(motor_controller_t *data)
{
    if (data == NULL) {
        hm_log_error("motor_controller: can`t stop motor controller, ctx is null\n");
        return -1;
    }
    data->is_working = false;
    pthread_join(data->tid, NULL);
    free(data);
    return 0;
}
