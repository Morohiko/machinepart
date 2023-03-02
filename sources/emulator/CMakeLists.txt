#***************************************************
# gyroscope emulator
#***************************************************
if (${WITH_GYROSCOPE_EMULATOR} STREQUAL ON)
  set(GYROSCOPE_EMULATOR_NAME "gyroscope_emulator")
  add_executable(${GYROSCOPE_EMULATOR_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/gyroscope_emulator/main.c ${JSON_CONFIG_SRC})
  target_link_libraries(${GYROSCOPE_EMULATOR_NAME} cjson)
  install(TARGETS ${GYROSCOPE_EMULATOR_NAME})
endif()

#***************************************************
# motor emulator
#***************************************************
if (${WITH_MOTOR_EMULATOR} STREQUAL ON)
  set(MOTOR_EMULATOR_NAME "motor_emulator")
  add_executable(${MOTOR_EMULATOR_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/motor_emulator/main.c ${JSON_CONFIG_SRC})
  target_link_libraries(${MOTOR_EMULATOR_NAME} cjson rt)
  install(TARGETS ${GYROSCOPE_EMULATOR_NAME})
endif()

#***************************************************
# camera receiver emulator
#***************************************************
if (${WITH_CAMERA_RECEIVER_EMULATOR} STREQUAL ON)
  set(CAMERA_RECEIVER_EMULATOR_NAME "camera_receiver_emulator")
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
  include_directories(${GTK3_INCLUDE_DIRS})
  link_directories(${GTK3_LIBRARY_DIRS})
  add_definitions(${GTK3_CFLAGS_OTHER})

  add_executable(${CAMERA_RECEIVER_EMULATOR_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/camera_receiver_emulator/main.c ${JSON_CONFIG_SRC})
  target_link_libraries(${CAMERA_RECEIVER_EMULATOR_NAME} ${GTK3_LIBRARIES} cjson)
  install(TARGETS ${CAMERA_RECEIVER_EMULATOR_NAME})

endif()
