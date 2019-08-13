#ifndef CORE_H
#define CORE_H

#include "controller.h"

struct connection_info {
    char local_ip[16];
    char target_ip[16];
    int local_port_gyroscope;
    int target_port_gyroscope;
};

int start_remote_controller(machine_controller *controller);
int stop_remote_controller(machine_controller *controller);

#ifdef ENABLE_CAMERA
int start_camera(machine_controller *controller);
int stop_camera(machine_controller *controller);

int start_camera_transmitter(machine_controller *controller);
int stop_camera_transmitter(machine_controller *controller);
#endif

#ifdef ENABLE_GYROSCOPE_RECEIVER
int start_gyroscope_data_receiver(machine_controller *controller, struct connection_info *conn);
int stop_gyroscope_data_receiver(machine_controller *controller);
#endif

int start_motor(machine_controller *controller);
int stop_motor(machine_controller *controller);

#endif
