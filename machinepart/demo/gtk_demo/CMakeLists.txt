cmake_minimum_required(VERSION 3.5)

set(GTK_DEMO_NAME gtk_demo)

project(${GTK_DEMO_NAME})

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(LOG_SRC ${PROJ_DIR}/src/log.c)

set(CONFIG_SRC ${PROJ_DIR}/src/config/json_config.c)

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

add_compile_definitions(JSON_CONFIG_FILE="${PROJ_DIR}/src/config/config.json")
add_compile_definitions(RSC_PATH="${PROJ_DIR}/rsc/")

set(ALL_SRC ${LOG_SRC}
            ${CONFIG_SRC})

include_directories(${PROJ_DIR}/header)
include_directories(${PROJ_DIR}/src/config)

add_executable(${GTK_DEMO_NAME} main.c ${ALL_SRC})
target_link_libraries(${GTK_DEMO_NAME} ${GTK3_LIBRARIES} cjson)
