#include <unistd.h>
#include <assert.h>

#include "log.h"
#include "core.h"
#include "controller.h"

int main_loop(machine_controller *controller) {
    int status = 0;
    while (1) {
        print("DEBUG: main loop");
#ifdef ENABLE_CAMERA
        if (controller->camera_current_state != controller->camera_state) {
            if (controller->camera_state == 1) {
                status = start_camera(controller);
            }
            if (controller->camera_state == 0) {
                status = stop_camera(controller);
            }
            assert(!status);
            controller->camera_current_state = controller->camera_state;
            print("DEBUG: changed camera state to %d", controller->camera_current_state);
        }

        if (controller->camera_transmitter_current_state != controller->camera_transmitter_state) {
            if (controller->camera_transmitter_state == 1) {
                status = start_camera_transmitter(controller);
            }
            if (controller->camera_transmitter_state == 0) {
                status = stop_camera_transmitter(controller);
            }
            assert(!status);
            controller->camera_transmitter_current_state = controller->camera_transmitter_state;
            print("DEBUG: changed camera transmitter state to %d", controller->camera_transmitter_current_state);
        }
#endif
        if (controller->gyroscope_receiver_current_state != controller->gyryscope_receiver_state) {
            if (controller->gyryscope_receiver_state == 1) {
                status = start_gyroscope_data_receiver(controller);
            }
            if (controller->gyryscope_receiver_state == 0) {
                status = stop_gyroscope_data_receiver(controller);
            }
            assert(!status);
            controller->gyroscope_receiver_current_state = controller->gyryscope_receiver_state;
            print("DEBUG: changed gyroscope receiver state to %d", controller->gyroscope_receiver_current_state);
        }
        if (controller->motor_current_state != controller->motor_state) {
            if (controller->motor_state == 1) {
                status = start_motor(controller);
            }
            if (controller->motor_state == 0) {
                status = stop_motor(controller);
            }
            assert(!status);
            controller->motor_current_state = controller->motor_state;
            print("DEBUG: changed motor state to %d", controller->motor_current_state);
        }
        sleep(1);
    }
}

int main() {
    print("======== Start Machinepart =======");

    print("DEBUG: start remote controller");
    machine_controller controller;
   
    start_remote_controller(&controller);

    main_loop(&controller);

    return 0;
}
