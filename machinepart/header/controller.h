#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <pthread.h>

#include "config.h"
#include "wifi.h"

typedef struct {
    int camera_current_state;
    int camera_transmitter_current_state;
    int gyroscope_receiver_current_state;
    int motor_current_state;

    int camera_state;
    int camera_transmitter_state;
    int gyroscope_receiver_state;
    int motor_state;

    pthread_t remoteControllerThreadID;
    pthread_t cameraThreadID;
    pthread_t cameraTransmitterThreadID;
    pthread_t gyroscopeRecvThreadID;
    pthread_t motorControllerThreadID;

    struct connection_info conn;
    struct tcp_socket controller_sock;
} machine_controller;

int receive_stdin_controller_message(machine_controller *machineController);
int receive_remote_controller_message(machine_controller *machineController);
int receive_controller_message(machine_controller *machineController);

int init_machine_controller_states(machine_controller *controller);

#endif // CONTROLLER_H
