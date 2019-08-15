#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "core.h"
#include "log.h"
#include "wifi.h"
#include "config.h"

#ifdef ENABLE_GYROSCOPE_RECEIVER
#include "gyroscope_receiver.h"
#endif

#ifdef ENABLE_CAMERA
#include "camera.h"
#endif

#define MAX_GYROSCOPE_DATA_SIZE 15

static void* remote_controller_thread(void *user_data) {
    register_log_module("REMOTE_CONTROLLER_MODULE", pthread_self());
    machine_controller *controller = (machine_controller*) user_data;
    while (1) {
        print(DEBUG, "remote controller thread");
        receive_stdin_controller_message(controller);
        sleep(1);
    }
}

int start_remote_controller(machine_controller *controller) {
    pthread_create(&controller->remoteControllerThreadID, NULL, remote_controller_thread, controller);
    return 0;
}

int stop_remote_controller(machine_controller *controller) {
    // must be stop all threads???
    return 0;
}

#ifdef ENABLE_CAMERA
static void *camera_thread(void *user_data) {
    register_log_module("CAMERA_MODULE", pthread_self());
    while (1) {
        print(DEBUG, "camera thread");
        sleep(1);
    }
}

int start_camera(machine_controller *controller) {
    struct camera_ctx cam;
    cam.isBusy = false;
    pthread_create(&controller->cameraThreadID, NULL, camera_thread, &cam);
    return 0;
}

int stop_camera(machine_controller *controller) {
    print(DEBUG, "stop camera");
    pthread_cancel(controller->cameraThreadID);
    return 0;
}

static void *camera_transmitter_thread(void *user_data) {
    register_log_module("CAMERA_TRANSMITTER_MODULE", pthread_self());
    while (1) {
        print(DEBUG, "camera transmitter thread");
        sleep(1);
    }
}

int start_camera_transmitter(machine_controller *controller) {
    pthread_create(&controller->cameraTransmitterThreadID, NULL, camera_transmitter_thread, NULL);
    return 0;
}

int stop_camera_transmitter(machine_controller *controller) {
    print(DEBUG, "stop camera transmitter");
    pthread_cancel(controller->cameraTransmitterThreadID);
    return 0;
}

#endif

#ifdef ENABLE_GYROSCOPE_RECEIVER
static void *gyroscope_receiver_thread(void *user_data) {
    register_log_module("GYROSCOPE_RECEIVER_MODULE", pthread_self());
    struct gyroscope_ctx ctx;
    struct connection_info *conn = (struct connection_info *) user_data;

    ctx.conn.local_port_gyroscope = conn->local_port_gyroscope;
    memcpy(ctx.conn.local_ip, conn->local_ip, 16);

    ctx.conn.target_port_gyroscope = conn->target_port_gyroscope;
    memcpy(ctx.conn.target_ip, conn->target_ip, 16);

    start_receive_gyroscope_data(&ctx);
}

int start_gyroscope_data_receiver(machine_controller *controller, struct connection_info *conn) {
    pthread_create(&controller->gyroscopeRecvThreadID, NULL, gyroscope_receiver_thread, conn);
    return 0;
}

int stop_gyroscope_data_receiver(machine_controller *controller) {
    print(DEBUG, "stop gyroscope data receiver");
    pthread_cancel(controller->gyroscopeRecvThreadID);
    return 0;
}
#endif

static void *motor_controller_thread(void* user_data) {
    register_log_module("MOTOR_CONTROLLER_MODULE", pthread_self());
    while (1) {
        print(DEBUG, "motor controller thread");
        sleep(1);
    }
}

int start_motor(machine_controller *controller) {
    pthread_t motorControllerThreadID;
    pthread_create(&controller->motorControllerThreadID, NULL, motor_controller_thread, NULL);
    return 0;
}

int stop_motor(machine_controller *controller) {
    print(DEBUG, "stop motor");
    pthread_cancel(controller->motorControllerThreadID);
    return 0;
}
