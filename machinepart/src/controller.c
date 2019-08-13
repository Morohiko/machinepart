#include <assert.h>

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
    scanf("%d:%d", &n1, &n2);
    print(DEBUG, "n1 = %d, n2 = %d", n1, n2);
    update_machine_controller(machineController, n1, n2);
    return 0;
}

// mocked
int receive_remote_controller_message(machine_controller *machineController) {
    assert(machineController);
//    char buff[1000];
//    update_machine_controller(machineController);
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

