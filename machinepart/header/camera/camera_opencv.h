#ifndef CAMERA_H
#define CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
int get_frame_from_camera(int *data, int *size, bool *isBusy, bool *isNewData);
#ifdef __cplusplus
}
#endif

#endif
