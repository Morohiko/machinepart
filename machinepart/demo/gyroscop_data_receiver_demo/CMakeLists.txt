cmake_minimum_required(VERSION 3.5)

set(WIFI_DEMO_NAME gyroscop_receiver_demo)

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(WIFI_SRC ${PROJ_DIR}/src/wifi.c
             ${PROJ_DIR}/header/wifi.h)

set(LOG_SRC  ${PROJ_DIR}/header/log.h
             ${PROJ_DIR}/src/log.c)

set(ALL_SRC ${WIFI_SRC}
            ${LOG_SRC})

include_directories(${PROJ_DIR}/header)

add_executable(${WIFI_DEMO_NAME} main.c ${ALL_SRC})
