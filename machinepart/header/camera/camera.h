#ifndef CAMERA_H
#define CAMERA_H

#include "camera/camera_transmitter.h"

// for testing
int save_current_image_to_file(struct camera_ctx *cam_ctx, char *filepath);

int pause_camera();

int run_camera(struct camera_ctx *cam_ctx);

#endif // CAMERA_H
