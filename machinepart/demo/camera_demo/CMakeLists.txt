cmake_minimum_required(VERSION 3.5)

set(CAMERA_DEMO_NAME camera_demo)

project(${CAMERA_DEMO_NAME})

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(LOG_SRC ${PROJ_DIR}/header/log.h
            ${PROJ_DIR}/src/log.c)

set(CAMERA_SRC ${PROJ_DIR}/src/camera.cpp
               ${PROJ_DIR}/header/camera.h)

set(ALL_SRC ${LOG_SRC}
            ${CAMERA_SRC})

include_directories(${PROJ_DIR}/header)


add_executable(${CAMERA_DEMO_NAME} main.c ${ALL_SRC})

