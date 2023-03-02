#include <stdlib.h>

#include <CUnit/Basic.h>

#include <shell_client.h>

void test_start_shell_client()
{
    char *ip = "127.0.0.1";
    int port = 1234;
    bool is_daemon = false;
    shell_client_t *ctx = start_shell_client(ip, port, is_daemon);
    CU_ASSERT(ctx != NULL);
    CU_ASSERT(strcmp(ctx->ipv4_addr, ip) == 0);
    CU_ASSERT(ctx->is_daemon == is_daemon);
    CU_ASSERT(ctx->thread_id > 0);
    free(ctx);
}

void test_stop_shell_client()
{
    char *ip = "127.0.0.1";
    int port = 1234;
    bool is_daemon = false;
    shell_client_t *ctx = start_shell_client(ip, port, is_daemon);
    CU_ASSERT(ctx != NULL);
    int ret = stop_shell_client(ctx);
    CU_ASSERT(ret == 0);
}
