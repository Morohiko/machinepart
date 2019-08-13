#ifndef LOG_H
#define LOG_H

#include "stdio.h"

// ************* log level ************ //
typedef enum {
    ERROR = 0,
    INFO,
    DEBUG,
} log_level;

log_level get_log_level();
void print_log_level(log_level level);
void set_log_level(log_level level);

// ************ log module *********** //
typedef struct {
    int main_module_id;               //MAIN_MODULE
    int remote_controller_module_id;  //REMOTE_CONTROLLER_MODULE
    int camera_module_id;             //CAMERA_MODULE
    int camera_transmitter_module_id; //CAMERA_TRANSMITTER_MODULE
    int gyroscope_receiver_module_id; //GYROSCOPE_RECEIVER_MODULE
    int motor_controller_module_id;   //MOTOR_CONTROLLER_MODULE
} log_modules;

void print_log_module_name();

int is_log_module_names_enabled();
void enable_log_with_module_names();
void disable_log_with_module_names();
void register_log_module(char *module_name, int thread_id);

#define print(log_level, ...) \
        if (log_level <= get_log_level()) { \
            printf("%s:%d:", __FILE__, __LINE__); \
            print_log_level(log_level); \
            printf(": "); \
            if (is_log_module_names_enabled()) { \
                print_log_module_name(); \
                printf(": "); \
            } \
            printf(__VA_ARGS__); \
            printf("\n"); \
        }

#endif
