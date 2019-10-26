#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "core.h"
#include "log.h"
#include "wifi.h"
#include "config.h"

#ifdef ENABLE_GYROSCOPE_RECEIVER
#include "gyroscope_receiver.h"
#include "motor_controller.h"
#endif

#ifdef ENABLE_CAMERA
#include "camera/camera_v4l.h"
#include "camera/camera_transmitter.h"
#endif

#define MAX_GYROSCOPE_DATA_SIZE 15

static void* remote_controller_thread(void *user_data) {
    register_log_module("REMOTE_CONTROLLER_MODULE", pthread_self());
    machine_controller *controller = (machine_controller*) user_data;

    print(DEBUG, "remote controller thread");
#ifdef REMOTE_CONTROLLER
    receive_remote_controller_message(controller);
#else // REMOTE_CONTRLLER
    receive_stdin_controller_message(controller);
#endif // REMOTE_CONTRLLER
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
static struct camera_ctx cam_ctx;

static void *camera_thread(void *user_data) {
    register_log_module("CAMERA_MODULE", pthread_self());
    int retval = 0;
    cam_ctx.isBusy = false;
    while (1) {
        get_frame_from_camera(&cam_ctx);
        print(DEBUG, "camera thread");
        sleep(1);
    }
}

int start_camera(machine_controller *controller) {
    pthread_create(&controller->cameraThreadID, NULL, camera_thread, &cam_ctx);
    return 0;
}

int stop_camera(machine_controller *controller) {
    print(DEBUG, "stop camera");
    pthread_cancel(controller->cameraThreadID);
    return 0;
}

static struct connection_info conn_info_camera_frame;
static struct connection_info conn_info_camera_ack;

static void *camera_transmitter_thread(void *user_data) {
    register_log_module("CAMERA_TRANSMITTER_MODULE", pthread_self());

    struct connection_info_cam *conn = (struct connection_info_cam *) user_data;
    memcpy(cam_ctx.conn.local_ip, conn->local_ip, 16);
    memcpy(cam_ctx.conn.target_ip, conn->target_ip, 16);

    cam_ctx.conn.frame_local_port = conn->frame_local_port;
    cam_ctx.conn.frame_target_port = conn->frame_target_port;

    cam_ctx.conn.ack_local_port = conn->ack_local_port;
    cam_ctx.conn.ack_target_port = conn->ack_target_port;

//    start_send_camera_data_through_udp(&cam_ctx);
    start_send_camera_data_through_tcp(&cam_ctx);
}

int start_camera_transmitter(machine_controller *controller,
                             struct connection_info_cam *conn_info_camera) {

    pthread_create(&controller->cameraTransmitterThreadID, NULL, camera_transmitter_thread, conn_info_camera);
    return 0;
}

int stop_camera_transmitter(machine_controller *controller) {
    print(DEBUG, "stop camera transmitter");
    pthread_cancel(controller->cameraTransmitterThreadID);
    return 0;
}

#endif

#ifdef ENABLE_GYROSCOPE_RECEIVER
struct gyroscope_ctx ctx;

static void *gyroscope_receiver_thread(void *user_data) {
    register_log_module("GYROSCOPE_RECEIVER_MODULE", pthread_self());
    struct connection_info *conn = (struct connection_info *) user_data;

    ctx.conn.local_port = conn->local_port;
    memcpy(ctx.conn.local_ip, conn->local_ip, 16);

    ctx.conn.target_port = conn->target_port;
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

static void *motor_controller_thread(void* user_data) {
    register_log_module("MOTOR_CONTROLLER_MODULE", pthread_self());

    print(DEBUG, "motor controller thread");
    start_configure_motors_angle(&ctx.motor_data);
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

#endif // ENABLE_GYROSCOPE_RECEIVER

