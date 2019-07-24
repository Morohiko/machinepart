#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <pthread.h>

#define CTRL_CAMERA_CONTROLER 0
#define CTRL_CAMERA_TRANSMITTER 1
#define CTRL_GYROSCOPE_RECEIVER 2
#define CTRL_MOTOR 3

typedef struct {
    int camera_current_state;
    int camera_transmitter_current_state;
    int gyroscope_receiver_current_state;
    int motor_current_state;

    int camera_state;
    int camera_transmitter_state;
    int gyryscope_receiver_state;
    int motor_state;

    pthread_t remoteControllerThreadID;
    pthread_t cameraThreadID;
    pthread_t cameraTransmitterThreadID;
    pthread_t gyroscopeRecvThreadID;
    pthread_t motorControllerThreadID;
} machine_controller;

int receive_stdin_controller_message(machine_controller *machineController);
int receive_remote_controller_message(machine_controller *machineController);

#endif
