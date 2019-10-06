#ifndef CORE_H
#define CORE_H

#include "controller.h"
#include "wifi.h"

int start_remote_controller(machine_controller *controller);
int stop_remote_controller(machine_controller *controller);

#ifdef ENABLE_CAMERA
int start_camera(machine_controller *controller);
int stop_camera(machine_controller *controller);

int start_camera_transmitter(machine_controller *controller,
                             struct connection_info_cam *conn_info_camera);
int stop_camera_transmitter(machine_controller *controller);
#endif // ENABLE_CAMERA

#ifdef ENABLE_GYROSCOPE_RECEIVER
int start_gyroscope_data_receiver(machine_controller *controller, struct connection_info *conn);
int stop_gyroscope_data_receiver(machine_controller *controller);
#endif // ENABLE_GYROSCOPE_RECEIVER

int start_motor(machine_controller *controller);
int stop_motor(machine_controller *controller);

#endif // CORE_H
