project("machinepart")
cmake_minimum_required(VERSION 3.5)

set(HEAD_MACHINE_NAME head_machine)
set(SHELL_CLIENT_LIB_NAME shell_lib)
set(SHELL_CLIENT_NAME shell_bin)

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
option(WITH_CAMERA_ONLY "Enable camera only" OFF)

set(BUILD_TARGET "" CACHE STRING "Chose build target(ubuntu/pi)")

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_compile_definitions(JSON_CONFIG_FILE="${PROJ_DIR}/src/config/config.json")

if(BUILD_TARGET STREQUAL "ubuntu")
  message("-- BUILD FOR UBUNTU")
  add_definitions(-DBUILD_ON_UBUNTU)
elseif(BUILD_TARGET STREQUAL "pi")
   message("-- BUILD FOR PI")
  add_definitions(-DBUILD_ON_PI)
else()
  message(FATAL_ERROR "unknown target, please choose from ubuntu or pi")
endif()

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

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")

if(ENABLE_CAMERA STREQUAL "ON")
   message("ENABLE CAMERA")

   set(CAMERA_SRC ${PROJ_DIR}/src/camera/camera_transmitter.c
                  ${PROJ_DIR}/header/camera/camera_transmitter.h)
   set(CAMERA_INTERNAL_SRC ${PROJ_DIR}/src/camera/camera.c
                           ${PROJ_DIR}/header/camera/camera.h)

   set(ALL_SRC ${ALL_SRC} ${CAMERA_SRC} ${CAMERA_INTERNAL_SRC})
endif()

if(ENABLE_GYROSCOPE_RECEIVER STREQUAL "ON")
   message("ENABLE GYROSCOPE RECEIVER")

   set(GYROSCOPE_RECEIVER ${PROJ_DIR}/src/gyroscope_receiver.c
                          ${PROJ_DIR}/header/gyroscope_receiver.h)

   if(BUILD_TARGET STREQUAL "pi")
      find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
      set(PLATFORM_SRC ${PROJ_DIR}/src/platforms/pi/motor.c
                       ${PROJ_DIR}/src/platforms/pi/motor.h
                       ${PROJ_DIR}/src/platforms/pi/camera_v4l.c
                       ${PROJ_DIR}/src/platforms/pi/camera_v4l.h)
      include_directories(${PROJ_DIR}/src/platforms/pi/)
   elseif(BUILD_TARGET STREQUAL "ubuntu")
      set(PLATFORM_SRC ${PROJ_DIR}/src/platforms/ubuntu/motor.c
                       ${PROJ_DIR}/src/platforms/ubuntu/motor.h
                       ${PROJ_DIR}/src/platforms/ubuntu/camera_file.c
                       ${PROJ_DIR}/src/platforms/ubuntu/camera_file.h)
      add_compile_definitions(CAMERA_RSC_DIR="${PROJ_DIR}/rsc/")

      include_directories(${PROJ_DIR}/src/platforms/ubuntu)
   endif()

   set(MOTOR_CONTROLLER ${PROJ_DIR}/src/motor_controller.c
                        ${PROJ_DIR}/header/motor_controller.h)

   set(ALL_SRC ${ALL_SRC} ${GYROSCOPE_RECEIVER} ${MOTOR_CONTROLLER} ${PLATFORM_SRC})
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

set(JSON_CONFIG_SRC ${PROJ_DIR}/src/config/json_config.c
                    ${PROJ_DIR}/src/config/json_config.h)

set(SHELL_SERVER_SRC ${PROJ_DIR}/src/shell/shell_server.c
                     ${PROJ_DIR}/src/shell/shell_server.h)

set(ALL_SRC ${CONTROLLER_SRC}
            ${CORE_SRC}
            ${UTILS_SRC}
            ${ALL_SRC}
            ${WIFI_SRC}
            ${LOG_SRC}
            ${FPS_CHECKER_SRC}
            ${JSON_CONFIG_SRC}
            ${SHELL_SERVER_SRC})

set(SHELL_CLIENT_SRC ${PROJ_DIR}/src/shell/shell_client.c
                     ${PROJ_DIR}/src/shell/shell_client.h
                     ${JSON_CONFIG_SRC}
                     ${LOG_SRC})

include_directories(${PROJ_DIR}/header)
include_directories(${PROJ_DIR}/src/config)
include_directories(${PROJ_DIR}/src/shell)

#***************************************************
# target
#***************************************************
add_executable(${HEAD_MACHINE_NAME} main.c ${ALL_SRC})
add_library(${SHELL_CLIENT_LIB_NAME} ${SHELL_CLIENT_SRC})
target_link_libraries(${SHELL_CLIENT_LIB_NAME} cjson rt)

target_link_libraries(${HEAD_MACHINE_NAME} ${SHELL_CLIENT_LIB_NAME})
if(BUILD_TARGET STREQUAL "pi")
  target_link_libraries(${HEAD_MACHINE_NAME} ${WIRINGPI_LIBRARIES})
endif()

add_executable(${SHELL_CLIENT_NAME} ${PROJ_DIR}/src/shell/main_shell_client.c)
target_link_libraries(${SHELL_CLIENT_NAME} ${SHELL_CLIENT_LIB_NAME})

#***************************************************
# emulator
#***************************************************
if (BUILD_TARGET STREQUAL "ubuntu")
  set(GYROSCOPE_EMULATOR_NAME "gyroscope_emulator")
  add_executable(${GYROSCOPE_EMULATOR_NAME} ${PROJ_DIR}/emulator/gyroscope_emulator/main.c ${LOG_SRC} ${WIFI_SRC} ${JSON_CONFIG_SRC})
  target_link_libraries(${GYROSCOPE_EMULATOR_NAME} cjson)
endif()

if (BUILD_TARGET STREQUAL "ubuntu")
  set(GYROSCOPE_EMULATOR_NAME "motor_emulator")
  add_executable(${GYROSCOPE_EMULATOR_NAME} ${PROJ_DIR}/emulator/motor_emulator/main.c ${LOG_SRC} ${JSON_CONFIG_SRC})
  target_link_libraries(${GYROSCOPE_EMULATOR_NAME} cjson rt)
endif()

if (BUILD_TARGET STREQUAL "ubuntu")
  set(GYROSCOPE_EMULATOR_NAME "camera_receiver_emulator")

  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
  include_directories(${GTK3_INCLUDE_DIRS})
  link_directories(${GTK3_LIBRARY_DIRS})
  add_definitions(${GTK3_CFLAGS_OTHER})

  add_executable(${GYROSCOPE_EMULATOR_NAME} ${PROJ_DIR}/emulator/camera_receiver_emulator/main.c ${LOG_SRC} ${WIFI_SRC} ${JSON_CONFIG_SRC})
  target_link_libraries(${GYROSCOPE_EMULATOR_NAME} ${GTK3_LIBRARIES} cjson)
endif()
