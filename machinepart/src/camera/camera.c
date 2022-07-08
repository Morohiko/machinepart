#include "assert.h"

#include "camera_v4l.h"

int init_camera(struct camera_ctx *cam_ctx) {
  return v4l2_init_camera(cam_ctx);
}

// for testing
int save_current_image_to_file(struct camera_ctx *cam_ctx, char *filepath) {
  assert(cam_ctx);
  assert(filepath);
  return v4l2_save_current_image_to_file(cam_ctx, filepath);
}

int run_camera(struct camera_ctx *cam_ctx) {
  assert(cam_ctx);
  return v4l2_run_camera(cam_ctx);
}

int pause_camera() {}

int camera_module_loop(struct camera_ctx *cam_ctx) {
  assert(cam_ctx);
  return v4l2_camera_module_loop(cam_ctx);
}
