#include <assert.h>
#include <unistd.h>

#include "log.h"
#include "controller.h"

static int update_machine_controller(machine_controller *machineController, int module, int state) {
    assert(machineController);
    switch (module) {
    case CTRL_CAMERA_CONTROLER:
        machineController->camera_state = state;
        print(DEBUG, "request camera state to %d", machineController->camera_state);
        break;
    case CTRL_CAMERA_TRANSMITTER:
        machineController->camera_transmitter_state = state;
        print(DEBUG, "request camera transmitter state to %d", machineController->camera_transmitter_state);
        break;
    case CTRL_GYROSCOPE_RECEIVER:
        machineController->gyroscope_receiver_state = state;
        print(DEBUG, "request gyroscope receiver state to %d", machineController->gyroscope_receiver_state);
        break;
    case CTRL_MOTOR:
        machineController->motor_state = state;
        print(DEBUG, "request mototr state to %d", machineController->motor_state);
        break;
    default:
        print(ERROR, "cannot find module %d", module);
    }
    return 0;
}

//module_number:status(0, 1)  0:0
int receive_stdin_controller_message(machine_controller *machineController) {
    assert(machineController);
    char buff[1000];
    int n1 = 0;
    int n2 = 0;
    while (1) {
        print(INFO, "wait STDIN controller message in format: \"2:1\", where :1 - on, :0 - off");
        print(INFO, "modules 0: - CAMERA_CONTROLER, 1: - CAMERA_TRANSMITTER, 2: - GYROSCOPE_RECEIVER, 3: - MOTOR");
        scanf("%d:%d", &n1, &n2);
        print(INFO, "n1 = %d, n2 = %d", n1, n2);
        update_machine_controller(machineController, n1, n2);
    }
    return 0;
}

#define REMOTE_CONTROLLER_MESSAGE_SIZE 4
static int parse_remote_controller_message(char *msg, int *dest_mod, int *dest_value) {
    assert(msg); assert(dest_mod); assert(dest_value);

    sscanf(msg, "%d:%d", dest_mod, dest_value);
    return 0;
}

// receive remote control from gp
int receive_remote_controller_message(machine_controller *machineController) {
    assert(machineController);
    char buff[1000];
    int n1 = 0;
    int n2 = 0;

    print(DEBUG, "\n=============== controller create socket ==============");

    print(DEBUG, "local ip = %s", machineController->conn.local_ip);
    print(DEBUG, "target ip = %s", machineController->conn.target_ip);

    print(DEBUG, "local port = %d", machineController->conn.local_port);
    print(DEBUG, "target port = %d", machineController->conn.target_port);

// configure remote controller socket
    if (create_tcp_socket(&machineController->controller_sock,
                          machineController->conn.local_port) != 0) {
        print(ERROR, "cannot create socket");
        return -1;
    }

    print(DEBUG, "socket created");
    if (listen_tcp_connection(&machineController->controller_sock, MAX_TCP_CONNECTION) != 0) {
        print(ERROR, "cannot start tcp listen connection");
        return -1;
    }

    print(DEBUG, "listen tcp connection");
    if (accept_tcp_connection(&machineController->controller_sock) != 0) {
        print(ERROR, "cannot accept connection");
        return -1;
    }

    char controller_message[REMOTE_CONTROLLER_MESSAGE_SIZE];

    while (1) {
        print(INFO, "wait REMOTE controller message in format: \"2:1\", where :1 - on, :0 - off");
        print(INFO, "modules 0: - CAMERA_CONTROLER, 1: - CAMERA_TRANSMITTER, 2: - GYROSCOPE_RECEIVER, 3: - MOTOR");
        while (recv_tcp_message(&machineController->controller_sock, controller_message, REMOTE_CONTROLLER_MESSAGE_SIZE)) {
            print(DEBUG, "controller message received, message: %s", controller_message);
            sleep(1);
        }

        print(DEBUG, "controller received message: %s", controller_message);

	if (parse_remote_controller_message(controller_message, &n1, &n2)) {
            print(ERROR, "cannot parse controller message");
            continue;
	}

        update_machine_controller(machineController, n1, n2);
    }
    return 0;
}

int init_machine_controller_states(machine_controller *controller) {
    assert(controller);

    controller->camera_current_state = 0;
    controller->camera_current_state = 0;
    controller->camera_transmitter_current_state = 0;
    controller->gyroscope_receiver_current_state = 0;
    controller->motor_current_state = 0;

    controller->camera_state = 0;
    controller->camera_transmitter_state = 0;
    controller->gyroscope_receiver_state = 0;
    controller->motor_state = 0;

    return 0;
}

