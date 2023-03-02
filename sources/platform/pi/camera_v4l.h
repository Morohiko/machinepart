#ifndef HM_CAMERA_V4L_H
#define HM_CAMERA_V4L_H

#include <camera_transmitter.h>

// for testing
int platform_save_current_image_to_file(camera_transmitter_t *cam_ctx,
                                        char *filepath);

int platform_pause_camera();

int platform_run_camera(camera_transmitter_t *cam_ctx);

#endif // HM_CAMERA_V4L_H
