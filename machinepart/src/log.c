#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "log.h"

// ************* log level ************** //
static log_level current_log_level = 0;

log_level get_log_level() {
    return current_log_level;
}

void set_log_level(log_level level) {
    current_log_level = level;
}

void print_log_level(log_level level) {
    switch(level) {
    case ERROR:
        printf("ERROR");
        break;
    case INFO:
        printf("INFO");
        break;
    case DEBUG:
        printf("DEBUG");
        break;
    default:
        printf("NONE");
    }
}

// ********** log modules ***************//
// 1 - enable, 0 - disable

static int is_log_module_names = 0;

int is_log_module_names_enabled() {
    return is_log_module_names;
}

void enable_log_with_module_names() {
    is_log_module_names = 1;
}

void disable_log_with_module_names() {
    is_log_module_names = 0;
}

static log_modules modules;

void register_log_module(char *module_name, int thread_id) {
    if (!strcmp(module_name, MAIN_MODULE)) {
        modules.main_module_id = thread_id;
        return;
    }
    if (!strcmp(module_name, REMOTE_CONTROLLER_MODULE)) {
        modules.remote_controller_module_id = thread_id;
        return;
    }
    if (!strcmp(module_name, CAMERA_MODULE)) {
        modules.camera_module_id = thread_id;
        return;
    }
    if (!strcmp(module_name, CAMERA_TRANSMITTER_MODULE)) {
        modules.camera_transmitter_module_id = thread_id;
        return;
    }
    if (!strcmp(module_name, GYROSCOPE_RECEIVER_MODULE)) {
        modules.gyroscope_receiver_module_id = thread_id;
        return;
    }
    if (!strcmp(module_name, MOTOR_CONTROLLER_MODULE)) {
        modules.motor_controller_module_id = thread_id;
        return;
    }
    printf("ERROR, cannot register this module, %s", module_name);
}

void print_log_module_name() {
    int thread_id = pthread_self();

    if (modules.main_module_id == thread_id) {
        printf(MAIN_MODULE);
        return;
    }
    if (modules.remote_controller_module_id == thread_id) {
        printf(REMOTE_CONTROLLER_MODULE);
        return;
    }
    if (modules.camera_module_id == thread_id) {
        printf(CAMERA_MODULE);
        return;
    }
    if (modules.camera_transmitter_module_id == thread_id) {
        printf(CAMERA_TRANSMITTER_MODULE);
        return;
    }
    if (modules.gyroscope_receiver_module_id == thread_id) {
        printf(GYROSCOPE_RECEIVER_MODULE);
        return;
    }
    if (modules.motor_controller_module_id == thread_id) {
        printf(MOTOR_CONTROLLER_MODULE);
        return;
    }

    printf("UNKNOWN_MODULE: ");
}

