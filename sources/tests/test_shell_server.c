#include <stdlib.h>

#include <CUnit/Basic.h>

#include <test_shell_server.h>
#include <shell_server.h>
#include <json_config.h>

void test_init_shell_server()
{
    shell_t config;
    shell_server_t *ctx = init_shell_server(&config);
    CU_ASSERT(ctx != NULL);
}

void test_destroy_shell_server()
{
    shell_t config;
    shell_server_t *ctx = init_shell_server(&config);
    CU_ASSERT(ctx != NULL);
    if (ctx == NULL)
        return;
    int ret = stop_shell_server(ctx);
    CU_ASSERT(ret == 0);
    ret = destroy_shell_server(ctx);
    CU_ASSERT(ret == 0);
}
