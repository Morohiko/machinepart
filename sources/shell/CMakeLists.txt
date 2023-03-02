cmake_minimum_required(VERSION 3.16)
project("shell_client")

option(WITH_STANDALONE_SERVER "Make standalone server application" OFF)
option(WITH_STANDALONE_CLIENT "Make standalone client application" ON)

###################################################################
# shell server
###################################################################
set(SHELL_SERVER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/shell_server.c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
###################################################################
# shell client
###################################################################
set(SHELL_CLIENT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/main_shell_client.c
                     ${CMAKE_CURRENT_SOURCE_DIR}/shell_client.c)

add_executable("shell_client" ${SHELL_CLIENT_SRC})
