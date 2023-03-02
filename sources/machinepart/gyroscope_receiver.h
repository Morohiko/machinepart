#ifndef HM_GYROSCOPE_RECEIVER_H
#define HM_GYROSCOPE_RECEIVER_H

#include <stdbool.h>

#include <motor_controller.h>

typedef struct {
    bool is_working;
    pthread_t tid;
    // connection_info_t conn;
    // struct udp_socket sock;
    int gp_sockfd;
    int gp_connfd;
    motor_controller_t motor_data;
} gyroscope_receiver_t;

gyroscope_receiver_t *start_gyroscope_receiver(
        gyroscope_receiver_module_t *gyroscope_receiver_config);

int stop_gyroscope_receiver(gyroscope_receiver_t *gyroscope_receiver);

#endif // HM_GYROSCOPE_RECEIVER_H
