#ifndef CAMERA_V4L_H
#define CAMERA_V4L_H

#include "camera/camera_transmitter.h"

int v4l2_init_camera(struct camera_ctx *cam_ctx);

// for testing
int v4l2_save_current_image_to_file(struct camera_ctx *cam_ctx, char *filepath);

int v4l2_run_camera(struct camera_ctx *cam_ctx);

int v4l2_pause_camera();

int v4l2_camera_module_loop(struct camera_ctx *cam_ctx);

#endif // CAMERA_V4L_H
