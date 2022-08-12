#ifndef CAMERA_V4L_H
#define CAMERA_V4L_H

#include "camera/camera_transmitter.h"

// for testing
int platform_save_current_image_to_file(struct camera_ctx *cam_ctx,
                                        char *filepath);

int platform_pause_camera();

int platform_run_camera(struct camera_ctx *cam_ctx);

#endif // CAMERA_V4L_H
