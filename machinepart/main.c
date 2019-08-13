#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "log.h"
#include "core.h"
#include "controller.h"
#include "wifi.h"
#include "utils.h"

static int main_loop(machine_controller *controller, struct connection_info *conn_info) {
    int status = 0;

    while (1) {
//        print("DEBUG: main loop");
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
            print(DEBUG, "changed camera state to %d", controller->camera_current_state);
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
            print(DEBUG, "changed camera transmitter state to %d", controller->camera_transmitter_current_state);
        }
#endif
        if (controller->gyroscope_receiver_current_state != controller->gyroscope_receiver_state) {
            if (controller->gyroscope_receiver_state == 1) {
                status = start_gyroscope_data_receiver(controller, conn_info);
            }
            if (controller->gyroscope_receiver_state == 0) {
                status = stop_gyroscope_data_receiver(controller);
            }
            assert(!status);
            controller->gyroscope_receiver_current_state = controller->gyroscope_receiver_state;
            print(DEBUG, "changed gyroscope receiver state to %d", controller->gyroscope_receiver_current_state);
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
            print(DEBUG, "changed motor state to %d", controller->motor_current_state);
        }
        sleep(1);
    }
}

static int configure_network(struct connection_info *conn_info) {
    assert(conn_info);

    int retval = 0;
    char ip_addr[16];

    retval = get_target_ip_addr(ip_addr);
    if (retval != 0) {
        print(ERROR, "cannot get target ip addr, retval: %d", retval);
        return retval;
    }

    memcpy(conn_info->local_ip, LOCAL_IP, 16);
    memcpy(conn_info->target_ip, ip_addr, 16);

    print(INFO, "selected ip: local = %s, target = %s", conn_info->local_ip, conn_info->target_ip);
    assert(conn_info->target_ip);

#ifdef ENABLE_GYROSCOPE_RECEIVER
    conn_info->local_port_gyroscope = LOCAL_GYROSCOPE_PORT;
    conn_info->target_port_gyroscope = TARGET_GYROSCOPE_PORT;
    print(INFO, "selected gyroscope port: local = %d, target = %d", conn_info->local_port_gyroscope, conn_info->target_port_gyroscope);
#endif

    return 0;
}

int main() {
    set_log_level(DEBUG);
    enable_log_with_module_names();
    register_log_module("MAIN_MODULE", pthread_self());

    print(INFO, "======== Start Machinepart =======");

    print(INFO, "======== configure network =======");

    struct connection_info conn_info;

    configure_network(&conn_info);

    print(DEBUG, "start remote controller");

    machine_controller controller;

    init_machine_controller_states(&controller);
   
    start_remote_controller(&controller);

    main_loop(&controller, &conn_info);

    return 0;
}
