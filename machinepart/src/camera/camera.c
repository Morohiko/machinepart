#include "assert.h"

#ifdef BUILD_ON_UBUNTU
#include "camera_file.h"
#elif BUILD_ON_PI
#include "camera_v4l.h"
#endif
#include "log.h"

// for testing
int save_current_image_to_file(struct camera_ctx *cam_ctx, char *filepath) {
  assert(cam_ctx);
  assert(filepath);

  FILE *out_file_fd = fopen("testfile", "wb");

  print(DEBUG, "wait while camera is busy");

  pthread_mutex_lock(&cam_ctx->lock);
  print(DEBUG, "start write to file %s", filepath);

  fwrite(cam_ctx->data.data, cam_ctx->data.size, 1, out_file_fd);

  pthread_mutex_unlock(&cam_ctx->lock);

  print(DEBUG, "writed");

  fclose(out_file_fd);

  print(DEBUG, "closed");

  return 0;
}

int pause_camera() { return platform_pause_camera(); }

int run_camera(struct camera_ctx *cam_ctx) {
  assert(cam_ctx);
  return platform_run_camera(cam_ctx);
}
