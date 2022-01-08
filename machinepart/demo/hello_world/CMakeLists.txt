cmake_minimum_required(VERSION 3.5)

set(HELLO_WORLD_DEMO_NAME hello_world_demo)

project(${HELLO_WORLD_DEMO_NAME})

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(LOG_SRC ${PROJ_DIR}/header/log.h
            ${PROJ_DIR}/src/log.c)

include_directories(${PROJ_DIR}/header)

add_executable(${HELLO_WORLD_DEMO_NAME} main.c ${LOG_SRC})
