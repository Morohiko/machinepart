PROJECT("runtest")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -Wall")

set(TESTS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/test_motor_controller.c
              ${CMAKE_CURRENT_SOURCE_DIR}/test_shell_client.c
              ${CMAKE_CURRENT_SOURCE_DIR}/test_shell_server.c)

add_executable(runtests test_main.c ${HM_SRC} ${TESTS_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

target_link_libraries(runtests cunit cjson rt)

add_custom_target(test "./runtests" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" VERBATIM)
