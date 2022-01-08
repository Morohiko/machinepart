cmake_minimum_required(VERSION 3.5)

set(NET_TOOLS_DEMO_NAME net_tools_demo)

project(${NET_TOOLS_DEMO_NAME})

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(LOG_SRC   ${PROJ_DIR}/header/log.h
              ${PROJ_DIR}/src/log.c)

set(UTILS_SRC ${PROJ_DIR}/header/utils.h
              ${PROJ_DIR}/src/utils.c)

set(ALL_SRC   ${LOG_SRC}
              ${UTILS_SRC})

include_directories(${PROJ_DIR}/header)

add_executable(${NET_TOOLS_DEMO_NAME} main.c ${ALL_SRC})
