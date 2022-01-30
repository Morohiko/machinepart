#ifndef CORE_H
#define CORE_H

#include <assert.h>

#include "controller.h"
#include "wifi.h"

int core_loop(machine_controller *controller,
              struct connection_info *conn_info_gyroscope,
              struct connection_info_cam *conn_info_camera);

int start_remote_controller(machine_controller *controller);
int stop_remote_controller(machine_controller *controller);

#endif // CORE_H
