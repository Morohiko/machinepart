cmake_minimum_required(VERSION 3.5)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

set(CAMERA_DATA_RECEIVER_DEMO_NAME camera_data_receiver_demo)

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(LOG_SRC ${PROJ_DIR}/header/log.h
            ${PROJ_DIR}/src/log.c)

set(WIFI_SRC ${PROJ_DIR}/header/wifi.h
            ${PROJ_DIR}/src/wifi.c)

set(CAMERA_SRC ${PROJ_DIR}/src/camera/camera.c
               ${PROJ_DIR}/header/camera/camera.h)

set(CAMERA_OPENCV_SRC ${PROJ_DIR}/src/camera/camera_opencv.cpp
                      ${PROJ_DIR}/header/camera/camera_opencv.h)

set(ALL_SRC ${LOG_SRC}
            ${WIFI_SRC}
            ${CAMERA_SRC}
            ${CAMERA_OPENCV_SRC})

include_directories(${PROJ_DIR}/header)

add_executable(${CAMERA_DATA_RECEIVER_DEMO_NAME} main.c ${ALL_SRC})

target_link_libraries(${CAMERA_DATA_RECEIVER_DEMO_NAME} ${OpenCV_LIBS} )

