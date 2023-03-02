project("machinepart")
cmake_minimum_required(VERSION 3.5)

set(HEAD_MACHINE_NAME head_machine)
set(SHELL_CLIENT_NAME shell_bin)

find_package(Threads REQUIRED)

#***************************************************
# print system info
#***************************************************
message("-------------------- system info --------------------")
message("-- CMAKE_SYSTEM_INFO_FILE:         ${CMAKE_SYSTEM_INFO_FILE}")
message("-- CMAKE_SYSTEM_NAME:              ${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR:         ${CMAKE_SYSTEM_PROCESSOR}")
message("-- CMAKE_SYSTEM:                   ${CMAKE_SYSTEM}")
message("-- CMAKE_SYSTEM_VERSION:           ${CMAKE_SYSTEM_VERSION}")
message("-----------------------------------------------------")

#***************************************************
# project settings
#***************************************************
set(BUILD_TARGET "emulator" CACHE STRING "Chose build target(emulator/pi)")
option(WITH_CAMERA_RECEIVER_EMULATOR  "With camera receiver emulator" ON)
option(WITH_GYROSCOPE_EMULATOR        "With gyroscope emulator"       ON)
option(WITH_MOTOR_EMULATOR            "With motor emulator"           ON)
option(WITH_TESTS                     "With tests"                    ON)

message("-------------------- project info -------------------")
message("-- BUILD_TARGET:                   ${BUILD_TARGET}")
message("-- WITH_CAMERA_RECEIVER_EMULATOR:  ${WITH_CAMERA_RECEIVER_EMULATOR}")
message("-- WITH_GYROSCOPE_EMULATOR:        ${WITH_GYROSCOPE_EMULATOR}")
message("-- WITH_MOTOR_EMULATOR:            ${WITH_MOTOR_EMULATOR}")
message("-- WITH_TESTS:                     ${WITH_TESTS}")
message("-----------------------------------------------------")

set(JSON_CONFIG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/config/json_config.c)

set(HM_SRC  ${CMAKE_CURRENT_SOURCE_DIR}/machinepart/gyroscope_receiver.c
            ${CMAKE_CURRENT_SOURCE_DIR}/machinepart/motor_controller.c
            ${CMAKE_CURRENT_SOURCE_DIR}/machinepart/camera_transmitter.c
            ${CMAKE_CURRENT_SOURCE_DIR}/machinepart/camera.c
            ${CMAKE_CURRENT_SOURCE_DIR}/config/json_config.c
            ${CMAKE_CURRENT_SOURCE_DIR}/shell/shell_server.c
            ${CMAKE_CURRENT_SOURCE_DIR}/shell/shell_client.c
            # todo: move to utils
            ${CMAKE_CURRENT_SOURCE_DIR}/fps_checker/fps_checker.c
            ${JSON_CONFIG_SRC})

if(BUILD_TARGET STREQUAL "emulator")
  add_definitions(-DBUILD_ON_EMULATOR)
  set(HM_SRC ${HM_SRC}
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/emulator/camera_file.c
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/emulator/motor_shm.c)

   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/platform/emulator)

   add_compile_definitions(CAMERA_RSC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/rsc/")
elseif(BUILD_TARGET STREQUAL "pi")
  add_definitions(-DBUILD_ON_PI)
else()
  message(FATAL_ERROR "unknown target, please choose from emulator or pi")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -Wall")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/machinepart)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/config)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/fps_checker)

add_compile_definitions(JSON_CONFIG_FILE="/etc/config.json")

#***************************************************
# main target
#***************************************************
add_executable(${HEAD_MACHINE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/machinepart/main.c ${HM_SRC})
target_link_libraries(${HEAD_MACHINE_NAME} cjson rt)

#***************************************************
# shell client
#***************************************************
add_executable(${SHELL_CLIENT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/shell/main_shell_client.c
                                    ${CMAKE_CURRENT_SOURCE_DIR}/shell/shell_client.c
                                    ${JSON_CONFIG_SRC})
target_link_libraries(${SHELL_CLIENT_NAME} cjson)

#***************************************************
# emulator
#***************************************************
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/emulator/)

#***************************************************
# tests
#***************************************************
if (${WITH_TESTS} STREQUAL ON)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/)
endif()

#***************************************************
# install
#***************************************************
install(TARGETS ${HEAD_MACHINE_NAME} ${SHELL_CLIENT_NAME})
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/config.json" DESTINATION /etc)
