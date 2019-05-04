cmake_minimum_required(VERSION 3.5)

set(WIFI_DEMO_NAME wifi_demo)

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
message(STATUS ${PROJ_DIR})

set(WIFI_SRC ${PROJ_DIR}/src/wifi.c
            ${PROJ_DIR}/header/wifi.h)

include_directories(${PROJ_DIR}/header)

add_executable(${WIFI_DEMO_NAME} main.c ${WIFI_SRC})
