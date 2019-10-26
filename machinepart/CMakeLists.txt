cmake_minimum_required(VERSION 3.5)

set(HEAD_MACHINE_NAME head_machine)

find_package(Threads REQUIRED)

#***************************************************
# print system info
#***************************************************
message("-- CMAKE_SYSTEM_INFO_FILE: ${CMAKE_SYSTEM_INFO_FILE}")
message("-- CMAKE_SYSTEM_NAME:      ${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")
message("-- CMAKE_SYSTEM_VERSION:   ${CMAKE_SYSTEM_VERSION}")

#***************************************************
# choose target option
#***************************************************
option(WITH_MOTOR_CONTROLLER_ONLY "Enable only gyroscope" OFF)
option(WITH_CAMERA_ONLY "Enable only only" OFF)

if(WITH_MOTOR_CONTROLLER_ONLY)
   add_definitions(-DENABLE_GYROSCOPE_RECEIVER)
   set(ENABLE_GYROSCOPE_RECEIVER "ON")
elseif(WITH_CAMERA_ONLY)
   add_definitions(-DENABLE_CAMERA)
   set(ENABLE_CAMERA "ON")
else()
   set(ENABLE_CAMERA "ON")
   set(ENABLE_GYROSCOPE_RECEIVER "ON")
   add_definitions(-DENABLE_CAMERA -DENABLE_GYROSCOPE_RECEIVER)
endif()

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread -lwiringPi")

if(ENABLE_CAMERA STREQUAL "ON")
   message("ENABLE CAMERA")

   set(CAMERA_SRC ${PROJ_DIR}/src/camera/camera_transmitter.c
                  ${PROJ_DIR}/header/camera/camera_transmitter.h)
   set(CAMERA_INTERNAL_SRC ${PROJ_DIR}/src/camera/camera_v4l.c
                              ${PROJ_DIR}/header/camera/camera_v4l.h)

   set(ALL_SRC ${ALL_SRC} ${CAMERA_SRC} ${CAMERA_INTERNAL_SRC})
endif()

if(ENABLE_GYROSCOPE_RECEIVER STREQUAL "ON")
   message("ENABLE GYROSCOPE RECEIVER")

   set(GYROSCOPE_RECEIVER ${PROJ_DIR}/src/gyroscope_receiver.c
                          ${PROJ_DIR}/header/gyroscope_receiver.h)

		  #   set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lwiringPi")

   set(MOTOR_CONTROLLER ${PROJ_DIR}/src/motor_controller.c
                        ${PROJ_DIR}/header/motor_controller.h)

   set(ALL_SRC ${ALL_SRC} ${GYROSCOPE_RECEIVER} ${MOTOR_CONTROLLER})
endif()

#***************************************************
# sources
#***************************************************
set(CONTROLLER_SRC ${PROJ_DIR}/src/controller.c
                   ${PROJ_DIR}/header/controller.h)

set(CORE_SRC ${PROJ_DIR}/src/core.c
             ${PROJ_DIR}/header/core.h)

set(UTILS_SRC ${PROJ_DIR}/src/utils.c
              ${PROJ_DIR}/header/utils.h)

set(WIFI_SRC ${PROJ_DIR}/src/wifi.c
             ${PROJ_DIR}/header/wifi.h)

set(LOG_SRC ${PROJ_DIR}/src/log.c
            ${PROJ_DIR}/header/log.h)

set(FPS_CHECKER_SRC ${PROJ_DIR}/src/fps_checker.c
                    ${PROJ_DIR}/header/fps_checker.h)

set(ALL_SRC ${CONTROLLER_SRC}
            ${CORE_SRC}
            ${UTILS_SRC}
            ${ALL_SRC}
            ${WIFI_SRC}
            ${LOG_SRC}
            ${FPS_CHECKER_SRC})

include_directories(header)

#***************************************************
# target
#***************************************************
add_executable(${HEAD_MACHINE_NAME} main.c ${ALL_SRC})
