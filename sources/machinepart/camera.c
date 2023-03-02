#include <assert.h>
#include <stdlib.h>

#include <camera.h>
#include <log.h>
#include <json_config.h>

#ifdef BUILD_ON_EMULATOR
#include <camera_file.h>
#elif BUILD_ON_PI
#include <camera_v4l.h>
#endif

static void *camera_thread(void *args)
{
    camera_t *ctx = (camera_t *)args;
    int curr_img = 0;
    while (ctx->is_working) {
        // try read from pipe?
        curr_img = curr_img ? 0 : 1;
        // platform_get_image(ctx->img_ptr[curr_img]);
        // todo: send image
        // send(ctx->img_ptr);
    }
    return NULL;
}

// camera_transmitter -> camera (need new image)
// camera -> camera_transmitter (ptr to new image)
camera_t *init_camera(camera_module_t *camera_config)
{
    camera_t *ctx = (camera_t *)malloc(sizeof(camera_t));
    if (!ctx) {
        hm_log_error("camera: can`t allocate mem\n");
        return NULL;
    }
    size_t img_size = camera_config->frame_width * camera_config->frame_height *
                      camera_config->frame_elem_size;
    platform_init_camera();
    ctx->is_working = true;
    for (int i = 0; i < 2; i++) {
        ctx->img_ptr[i] = malloc(img_size);
        memset(ctx->img_ptr[i], '\0', img_size);
    }
    pthread_create(&ctx->tid, NULL, camera_thread, ctx);
    return ctx;
}

int destroy_camera(camera_t *camera)
{
    if (!camera) {
        hm_log_error("camera: can`t destroy camera module, null\n");
        return -1;
    }
    camera->is_working = false;
    pthread_join(camera->tid, NULL);
    for (int i = 0; i < 2; i++) {
        free(camera->img_ptr[i]);
    }
    free(camera);
    return 0;
}
