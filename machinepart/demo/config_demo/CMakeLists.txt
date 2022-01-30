cmake_minimum_required(VERSION 3.5)

set(CONFIG_DEMO_NAME config_demo)

project(${CONFIG_DEMO_NAME})

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(LOG_SRC ${PROJ_DIR}/src/log.c)

set(CONFIG_SRC ${PROJ_DIR}/src/config/json_config.c)

set(ALL_SRC ${LOG_SRC}
            ${CONFIG_SRC})

include_directories(${PROJ_DIR}/header)
include_directories(${PROJ_DIR}/src/config)

add_executable(${CONFIG_DEMO_NAME} main.c ${ALL_SRC})
target_link_libraries(${CONFIG_DEMO_NAME} cjson)
