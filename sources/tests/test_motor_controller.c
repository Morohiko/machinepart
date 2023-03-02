#include <stdio.h>
#include <stdlib.h>

#include <CUnit/Basic.h>

#include <motor_controller.h>

void test_start_motor_controller()
{
    motor_module_t motor_config;
    motor_controller_t *ctx = start_motor_controller(&motor_config);
    CU_ASSERT(ctx != NULL);

    CU_ASSERT(ctx->is_working == true);
    CU_ASSERT(ctx->tid > 0);
    free(ctx);
}

void test_stop_motor_controller()
{
    motor_module_t motor_config;
    motor_controller_t *ctx = start_motor_controller(&motor_config);
    CU_ASSERT(ctx != NULL);

    int ret = stop_motor_controller(ctx);
    CU_ASSERT(ret == 0);
    ret = stop_motor_controller(NULL);
    CU_ASSERT(ret == -1);
}
