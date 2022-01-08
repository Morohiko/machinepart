cmake_minimum_required(VERSION 3.5)

set(MOTOR_DEMO_NAME motor_demo)

project(${MOTOR_DEMO_NAME})

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lwiringPi -pthread")

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(MOTOR_CONTROLLER_SRC ${PROJ_DIR}/header/motor_controller.h
                         ${PROJ_DIR}/src/motor_controller.c)

set(CONFIG_SRC ${PROJ_DIR}/header/config.h)

set(LOG_SRC ${PROJ_DIR}/header/log.h
            ${PROJ_DIR}/src/log.c)

set(ALL_SRC ${MOTOR_CONTROLLER_SRC}
            ${CONFIG_SRC}
            ${LOG_SRC})

include_directories(${PROJ_DIR}/header)

add_executable(${MOTOR_DEMO_NAME} main.c ${ALL_SRC})
