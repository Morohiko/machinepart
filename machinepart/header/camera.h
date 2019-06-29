#include "config.h"

struct camera_ctx {
    int data[CAMERA_FRAME_WIDTH*CAMERA_FRAME_HEIGHT*CAMERA_FRAME_ELEM_SIZE];
};

#ifdef __cplusplus
extern "C" {
#endif
int get_next_frame(struct camera_ctx *ctx);
#ifdef __cplusplus
}
#endif