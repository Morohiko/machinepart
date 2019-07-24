#include "log.h"
#include "controller.h"

static int update_machine_controller(machine_controller *machineController, int module, int state) {
    switch (module) {
    case CTRL_CAMERA_CONTROLER:
        machineController->camera_state = state;
        print("DEBUG: request camera state to %d", machineController->camera_state);
        break;
    case CTRL_CAMERA_TRANSMITTER:
        machineController->camera_transmitter_state = state;
        print("DEBUG: request camera transmitter state to %d", machineController->camera_transmitter_state);

        break;
    case CTRL_GYROSCOPE_RECEIVER:
        machineController->gyryscope_receiver_state = state;
        print("DEBUG: request gyroscope receiver state to %d", machineController->gyryscope_receiver_state);

        break;
    case CTRL_MOTOR:
        machineController->motor_state = state;
        print("DEBUG: request mototr state to %d", machineController->motor_state);

        break;
    default:
        print("ERROR: cannot find module %d", module);
    }
    return 0;
}

//module_number:status(0, 1)  0:0
int receive_stdin_controller_message(machine_controller *machineController) {
    char buff[1000];
    int n1 = 0;
    int n2 = 0;
    scanf("%d:%d", &n1, &n2);
    print("n1 = %d, n2 = %d", n1, n2);
    update_machine_controller(machineController, n1, n2);
    return 0;
}

// mocked
int receive_remote_controller_message(machine_controller *machineController) {
//    char buff[1000];
//    update_machine_controller(machineController);
    return 0;
}

