#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>

#include <test_motor_controller.h>
#include <test_shell_client.h>
#include <test_shell_server.h>

#define CHECK_NOT_NULL(ret)    \
    if (ret == NULL) {         \
        CU_cleanup_registry(); \
        return CU_get_error(); \
    }

int init_suite_empty(void)
{
    return 0;
}

int clean_suite_empty(void)
{
    return 0;
}

int test_motor_contoller()
{
    CU_pSuite pSuite = CU_add_suite("suite_motor_controller", init_suite_empty,
                                    clean_suite_empty);
    CHECK_NOT_NULL(pSuite);

    CU_Test *test = CU_add_test(pSuite, "test of start_motor_controller()",
                                test_start_motor_controller);
    CHECK_NOT_NULL(test);

    test = CU_add_test(pSuite, "test of stop_motor_controller()",
                       test_stop_motor_controller);
    CHECK_NOT_NULL(test);

    return 0;
}

int test_shell_client()
{
    CU_pSuite pSuite = CU_add_suite("suite_shell_client", init_suite_empty,
                                    clean_suite_empty);
    CHECK_NOT_NULL(pSuite);

    CU_Test *test = CU_add_test(pSuite, "test of start_shell_client()",
                                test_start_shell_client);
    CHECK_NOT_NULL(test);

    test = CU_add_test(pSuite, "test of stop_shell_client()",
                       test_stop_shell_client);
    CHECK_NOT_NULL(test);

    return 0;
}

int test_shell_server()
{
    CU_pSuite pSuite = CU_add_suite("suite_shell_server", init_suite_empty,
                                    clean_suite_empty);
    CHECK_NOT_NULL(pSuite);

    CU_Test *test = CU_add_test(pSuite, "test of init_shell_server()",
                                test_init_shell_server);
    CHECK_NOT_NULL(test);

    test = CU_add_test(pSuite, "test of destroy_shell_server()",
                       test_destroy_shell_server);
    CHECK_NOT_NULL(test);

    return 0;
}

int main()
{
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    int ret = test_motor_contoller();
    if (ret) {
        printf("ret = %d\n", ret);
    }

    ret = test_shell_client();
    if (ret) {
        printf("ret = %d\n", ret);
    }

    ret = test_shell_server();
    if (ret) {
        printf("ret = %d\n", ret);
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}