#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/videodev2.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "utils.h"
#include "log.h"
#include "camera/camera_transmitter.h"

#define CAMERA_DEVICE_NAME "/dev/video0"

static int fd = -1;

static int xioctl(int fh, int request, void *arg) {
    int r;
    do {
        r = ioctl(fh, request, arg);
    } while (r == -1);

    return r;
}

static int open_device() {
    struct stat st;

    if (stat(CAMERA_DEVICE_NAME, &st) == -1) {
        print(ERROR, "Cannot identify %s", CAMERA_DEVICE_NAME);
        return -1;
    }

    if (!S_ISCHR(st.st_mode)) {
        print(ERROR, "%s is no devicen", CAMERA_DEVICE_NAME);
        return -1;
    }

    print(DEBUG, "st_mode = %d", st.st_mode);
    fd = open(CAMERA_DEVICE_NAME, O_RDWR | O_NONBLOCK, 0);

    if (fd == -1) {
        print(ERROR, "Cannot open %s", CAMERA_DEVICE_NAME);
        return -1;
    }

    return 0;
}

static int init_capabilities(struct camera_ctx *cam_ctx) {
    struct v4l2_capability cap;
    struct v4l2_format fmt;

    unsigned int min;

    if (xioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        print(ERROR, "VIDIOC_QUERYCAP");
        return -1;
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        print(ERROR, "%s is no video capture device", CAMERA_DEVICE_NAME);
        return -1;
    }
    if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
        print(ERROR, "%s does not support read i/o", CAMERA_DEVICE_NAME);
        return -1;
    }

    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = CAMERA_FRAME_WIDTH;
    fmt.fmt.pix.height      = CAMERA_FRAME_HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt)) {
        print(ERROR, "VIDIOC_S_FMT");
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
        print(ERROR, "missmatch data sizes");
        return -1;
    }

    if (!cam_ctx->data.data) {
        print(ERROR, "Out of memory");
        return -1;
    }

    return 0;
}

int init_camera(struct camera_ctx *cam_ctx) {
    print(DEBUG, "camera data size = %d", CAMERA_DATA_SIZE);
    cam_ctx->data.data = malloc(CAMERA_DATA_SIZE);
    cam_ctx->data.size = CAMERA_DATA_SIZE;
    int retval = 0;
    retval = open_device();

    if (retval != 0) {
        print(ERROR, "cannot open camera device");
        return retval;
    }

    retval = init_capabilities(cam_ctx);

    if (retval != 0) {
        print(ERROR, "cannot init capabilities for camera");
        return retval;
    }

    return retval;
}

//for testing
int save_current_image_to_file(struct camera_ctx *cam_ctx, char *filepath) {
    assert(cam_ctx);
    assert(filepath);

    FILE *out_file_fd = fopen("testfile", "wb");

    print(DEBUG, "wait while camera is busy");

    while (cam_ctx->isBusy) {
        continue;
    }

    print(DEBUG, "start write to file %s", filepath);

    cam_ctx->isBusy = true;
    fwrite(cam_ctx->data.data, cam_ctx->data.size, 1, out_file_fd);
    cam_ctx->isBusy = false;

    print(DEBUG, "writed");

    fclose(out_file_fd);

    print(DEBUG, "closed");

    return 0;
}

int run_camera(struct camera_ctx *cam_ctx) {
//    cam_ctx->isWorking = true;
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
            print(ERROR, "cannot do select in camera");
            return -1;
        }

        if (retval == 0) {
            print(ERROR, "select timeout");
            continue;
        }

        if (cam_ctx->isBusy == true) {
            continue;
	}

        cam_ctx->isBusy = true;
	retval = read(fd, cam_ctx->data.data, cam_ctx->data.size);
	cam_ctx->isBusy = false;

        if (retval == -1) {
            print(ERROR, "cannot read from camera");
	    return -1;
        }
        cam_ctx->isNewData = true;

//        print(DEBUG, "camera data size = %zu", cam_ctx->data.size);
    }
    return 0;
}

int pause_camera() {
}

// int get_frame_from_camera(void *data, size_t *size, bool *isBusy, bool *isNewData) {
int get_frame_from_camera(struct camera_ctx *cam_ctx) {
if (cam_ctx->isBusy) {
    print(DEBUG, "ftw cam is busy?");
}
    int retval = 0;

    print(INFO, "init camera");

    retval = init_camera(cam_ctx);

    if (retval != 0) {
        print(ERROR, "cannot init camera");
        return -1;
    }

    print(INFO, "run camera");

    retval = run_camera(cam_ctx);

    if (retval != 0) {
        print(ERROR, "run camera error");
        return -1;
    }

#ifdef aa //opencv
    assert(data); assert(size); assert(isBusy);

    int retval = 0;
    if (*isBusy == true) {
        print(ERROR, "camera ctx is busy, return -1");
        return -1;
    }
    *isBusy = false;
    print(DEBUG, "get next frame");
    retval = get_next_frame(data, size, isBusy, isNewData);
//    ctx->isBusy = false;

    assert(!retval);

    return retval;
#endif
}

