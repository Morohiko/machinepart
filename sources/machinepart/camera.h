#ifndef HM_CAMERA_H
#define HM_CAMERA_H

#include <pthread.h>
#include <stdbool.h>

#include <json_config.h>

typedef struct {
    bool is_working;
    pthread_t tid;
    void *img_ptr[2];
} camera_t;

#if 0
// for testing
int save_current_image_to_file(camera_transmitter_t *cam_ctx, char *filepath);
#endif

camera_t *init_camera(camera_module_t *camera_config);

int destroy_camera(camera_t *camera);

#endif // HM_CAMERA_H
