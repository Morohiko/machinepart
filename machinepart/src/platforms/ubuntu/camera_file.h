#ifndef CAMERA_FILE_H
#define CAMERA_FILE_H

#include "camera/camera_transmitter.h"

int platform_pause_camera();

int platform_run_camera(struct camera_ctx *cam_ctx);

#endif // CAMERA_V4L_H