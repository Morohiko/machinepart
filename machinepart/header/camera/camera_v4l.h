#ifndef CAMERA_V4L_H
#define CAMERA_V4L_H

#include "stdbool.h"

#include "camera/camera_transmitter.h"

int init_camera(struct camera_ctx *cam_ctx);

// for testing
int save_current_image_to_file(struct camera_ctx *cam_ctx, char *filepath);

int run_camera(struct camera_ctx *cam_ctx);

int pause_camera();

int get_frame_from_camera(struct camera_ctx *cam_ctx);

#endif // CAMERA_V4L_H
