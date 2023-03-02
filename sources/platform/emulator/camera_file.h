#ifndef HM_CAMERA_FILE_H
#define HM_CAMERA_FILE_H

#include <camera_transmitter.h>

int platform_init_camera();
int platform_deinit_camera();

int platform_pause_camera();

int platform_run_camera(camera_transmitter_t *cam_ctx);

#endif // CAMERA_V4L_H