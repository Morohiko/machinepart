cmake_minimum_required(VERSION 3.5)

set(HELLO_WORLD_DEMO_NAME hello_world_demo)

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(LOG_SRC ${PROJ_DIR}/header/log.h)

include_directories(${PROJ_DIR}/header)

add_executable(${HELLO_WORLD_DEMO_NAME} main.c)
