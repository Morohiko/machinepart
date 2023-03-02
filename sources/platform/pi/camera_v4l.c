#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <camera_transmitter.h>
#include <json_config.h>
#include <log.h>

#define CAMERA_DEVICE_NAME "/dev/video0"

static int fd = -1;

static int xioctl(int fh, int request, void *arg)
{
    int r;
    do {
        r = ioctl(fh, request, arg);
    } while (r == -1);

    return r;
}

static int open_device()
{
    struct stat st;

    if (stat(CAMERA_DEVICE_NAME, &st) == -1) {
        hm_log_error("camera_v4l: Cannot identify %s\n", CAMERA_DEVICE_NAME);
        return -1;
    }

    if (!S_ISCHR(st.st_mode)) {
        hm_log_error("camera_v4l: %s is no devicen", CAMERA_DEVICE_NAME);
        return -1;
    }

    hm_log_debug("camera_v4l: st_mode = %d\n", st.st_mode);
    fd = open(CAMERA_DEVICE_NAME, O_RDWR | O_NONBLOCK, 0);

    if (fd == -1) {
        hm_log_error("camera_v4l: Cannot open %s\n", CAMERA_DEVICE_NAME);
        return -1;
    }

    return 0;
}

static int init_capabilities(camera_transmitter_t *cam_ctx)
{
#if 0
    struct v4l2_capability cap;
    struct v4l2_format fmt;

    unsigned int min;

    if (xioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        hm_log_error("camera_v4l: VIDIOC_QUERYCAP\n");
        return -1;
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        hm_log_error("camera_v4l: %s is no video capture device",
                     CAMERA_DEVICE_NAME);
        return -1;
    }

    // TODO: need check if supports V4L2_CAP_STREAMING for pi platform
    if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
        hm_log_error("camera_v4l: %s does not support read i/o",
                     CAMERA_DEVICE_NAME);
        return -1;
    }

    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = json_config.modules.camera_module.frame_width;
    fmt.fmt.pix.height = json_config.modules.camera_module.frame_height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt)) {
        hm_log_error("camera_v4l: VIDIOC_S_FMT\n");
        return -1;
    }

    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

    if (fmt.fmt.pix.sizeimage != cam_ctx->data.size) {
        hm_log_error("camera_v4l: missmatch data sizes\n");
        return -1;
    }

    if (!cam_ctx->data.data) {
        hm_log_error("camera_v4l: Out of memory\n");
        return -1;
    }

    return 0;
#endif
}

static int platform_init_camera(camera_transmitter_t *cam_ctx)
{
#if 0
    int camera_data_size = json_config.modules.camera_module.frame_height *
                           json_config.modules.camera_module.frame_width *
                           json_config.modules.camera_module.frame_elem_size;
    hm_log_debug("camera_v4l: camera data size = %d\n", camera_data_size);
    cam_ctx->data.data = malloc(camera_data_size);
    cam_ctx->data.size = camera_data_size;
    int retval = 0;
    retval = open_device();

    if (retval != 0) {
        hm_log_error("camera_v4l: cannot open camera device\n");
        return retval;
    }

    retval = init_capabilities(cam_ctx);

    if (retval != 0) {
        hm_log_error("camera_v4l: cannot init capabilities for camera\n");
        return retval;
    }

    return retval;
#endif
}

static int read_from_camera(camera_transmitter_t *cam_ctx)
{
#if 0
    for (;;) {
        fd_set fds;
        struct timeval tv;
        int retval;

        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        /* Timeout. */
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        retval = select(fd + 1, &fds, NULL, NULL, &tv);

        if (retval == -1) {
            hm_log_error("camera_v4l: cannot do select in camera\n");
            return -1;
        }

        if (retval == 0) {
            hm_log_error("camera_v4l: select timeout\n");
            continue;
        }

        pthread_mutex_lock(&cam_ctx->lock);
        retval = read(fd, cam_ctx->data.data, cam_ctx->data.size);

        pthread_mutex_unlock(&cam_ctx->lock);

        if (retval == -1) {
            hm_log_error("camera_v4l: cannot read from camera\n");
            return -1;
        }
        cam_ctx->isNewData = true;
    }
    return 0;
#endif
}

int platform_pause_camera()
{
}

int platform_run_camera(camera_transmitter_t *cam_ctx)
{
#if 0
    int retval = 0;

    hm_log_info("camera_v4l: init camera\n");

    retval = platform_init_camera(cam_ctx);

    if (retval != 0) {
        hm_log_error("camera_v4l: cannot init camera\n");
        return -1;
    }

    hm_log_info("camera_v4l: run camera\n");

    retval = read_from_camera(cam_ctx);

    if (retval != 0) {
        hm_log_error("camera_v4l: run camera error\n");
        return -1;
    }
#endif
}
