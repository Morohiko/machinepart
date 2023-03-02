#ifndef HM_CAMERA_TRANSMITTER_H
#define HM_CAMERA_TRANSMITTER_H

#include <pthread.h>
#include <stdbool.h>

typedef struct {
    void *data;
    size_t size;
    size_t max_size;
} camera_data_t;

typedef struct {
    pthread_t tid;
    pthread_mutex_t lock;
    bool isNewData;
    bool is_working;
    camera_data_t data;
    int listenfd;
    int connfd;
} camera_transmitter_t;

camera_transmitter_t *camera_transmitter_init();
int camera_transmitter_destroy(camera_transmitter_t *camera_transmitter);

#endif // HM_CAMERA_TRANSMITTER_H
