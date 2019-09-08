cmake_minimum_required(VERSION 3.5)

set(V4L_DEMO_NAME v4l_demo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lv4l2 -lv4l1 -ggdb -pthread")

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(LOG_SRC ${PROJ_DIR}/header/log.h
            ${PROJ_DIR}/src/log.c)

set(UTILS_SRC ${PROJ_DIR}/header/utils.h)

set(CAMERA_SRC ${PROJ_DIR}/src/camera/camera_transmitter.c
               ${PROJ_DIR}/header/camera/camera_transmitter.h)

set(CAMERA_V4L_SRC ${PROJ_DIR}/src/camera/camera_v4l.c
                   ${PROJ_DIR}/header/camera/camera_v4l.h)

set(WIFI_SRC ${PROJ_DIR}/src/wifi.c
             ${PROJ_DIR}/header/wifi.h)

set(ALL_SRC ${LOG_SRC}
            ${UTILS_SRC}
            ${CAMERA_SRC}
	    ${CAMERA_V4L_SRC}
            ${WIFI_SRC})

include_directories(${PROJ_DIR}/header)

add_executable(${V4L_DEMO_NAME} main.c ${ALL_SRC})
