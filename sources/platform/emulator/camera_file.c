#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <camera.h>
#include <camera_file.h>
#include <json_config.h>
#include <log.h>

#define FILES_COUNT 10

char buffer_files[FILES_COUNT][100];
int iter = 0;

int platform_init_camera(camera_transmitter_t *cam_ctx)
{
#if 0
    int max_size = json_config.modules.camera_module.frame_width *
                   json_config.modules.camera_module.frame_height *
                   json_config.modules.camera_module.frame_elem_size;
    cam_ctx->data.data = malloc(max_size);
    cam_ctx->data.max_size = max_size;
#endif
    return 0;
}

#if 0
static int read_from_camera(camera_transmitter_t *cam_ctx)
{
    DIR *d = opendir(CAMERA_RSC_DIR);
    struct dirent *dir;
    if (!d) {
        hm_log_error("camera_file: dir %s is not exist", CAMERA_RSC_DIR);
        return -1;
    }
    for (int i = 0; i < FILES_COUNT; i++) {
        memset(buffer_files[iter], '\0', 100);
        memcpy(buffer_files[iter++], CAMERA_RSC_DIR, strlen(CAMERA_RSC_DIR));
    }

    iter = 0;
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }
        memcpy(&buffer_files[iter++][strlen(CAMERA_RSC_DIR)], dir->d_name,
               strlen(dir->d_name));
        hm_log_debug("camera_file: file found: %s\n", dir->d_name);
    }
    iter = 0;
    while (1) {
        pthread_mutex_lock(&cam_ctx->lock);
        FILE *file = fopen(buffer_files[iter], "r");
        if (file == NULL) {
            hm_log_error("camera_file: cant open file %s\n",
                         buffer_files[iter]);
            continue;
        }
        int size = 0;
        char symbol;
        int i = 0;
        if (file == NULL) {
            hm_log_error("camera_file: read file error %s\n",
                         buffer_files[iter]);
        }
        memset(cam_ctx->data.data, '\0', cam_ctx->data.max_size);
        size_t newLen = fread(cam_ctx->data.data, sizeof(char),
                              cam_ctx->data.max_size, file);
        if (newLen > cam_ctx->data.max_size) {
            hm_log_error(
                    "camera_file: readded data size biggest then max size of raw image, "
                    "buffer overflow\n");
            fclose(file);
            pthread_mutex_unlock(&cam_ctx->lock);
            return -1;
        }
        cam_ctx->data.size = newLen;
        fclose(file);
        pthread_mutex_unlock(&cam_ctx->lock);
        iter++;
        if (iter >= FILES_COUNT) {
            iter = 0;
        }
    }
    closedir(d);
    return 0;
}
#endif

int platform_pause_camera()
{
    hm_log_error("camera_file: platform_pause_camera\n");
    return 0;
}

int platform_run_camera(camera_transmitter_t *cam_ctx)
{
#if 0
    platform_init_camera(cam_ctx);
    while (1) {
        int retval = read_from_camera(cam_ctx);
        if (retval == -1) {
            break;
        }
    }
#endif
    return 0;
}
