#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "assert.h"
#include "log.h"
#include "camera.h"
#include <iostream>

using namespace cv;
using namespace std;

static int mat_to_int_arr(Mat* src, int *dest) {
    assert(src); assert(src->rows);
    assert(src->cols); assert(src->data);
    assert(dest);

    int count = 0;

    for(int i = 0; i < src->cols; ++i) {
        for (int j = 0; j < src->rows; ++j) {
            dest[count++] = src->at<int>(i, j);
        }
    }

    return 0;
}

static int int_arr_to_mat(int *src, Mat *dest) {
    assert(src); assert(dest);

    int count = 0;
    for(int i = 0; i < dest->cols; ++i) {
        for (int j = 0; j < dest->rows; ++j) {
            dest->at<int>(i, j) = src[count++];
        }
    }

    return 0;
}

static int check_camera_config_with_default(Mat *frame) {
    if (CAMERA_FRAME_WIDTH != frame->cols) {
        print("ERROR: camera frame width: config = %d, camera = %d", CAMERA_FRAME_WIDTH, frame->rows);
        return -1;
    }

    if (CAMERA_FRAME_HEIGHT != frame->rows) {
        print("ERROR: camera frame height: config = %d, camera = %d", CAMERA_FRAME_HEIGHT, frame->cols);
        return -1;
    }

    if (CAMERA_FRAME_TYPE != frame->type()) {
        print("ERROR: camera frame type: config = %d, camera = %d", CAMERA_FRAME_TYPE, frame->type());
        return -1;
    }

    if (CAMERA_FRAME_ELEM_SIZE != frame->elemSize()) {
        print("ERROR: camera frame elem size: config = %d, camera = %d", CAMERA_FRAME_ELEM_SIZE, frame->elemSize());
        return -1;
    }

    print("INFO: camera config corresponds with config.h");
    return 0;
}

// write to arguments received frame and size from cam
// the function itself allocate memory for frame
int get_next_frame(struct camera_ctx *cam) {
    Mat frame;
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        print("Cannot open the web cam");
        return -1;
    }
    print("Camera is opened");

    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API

    // open selected camera using selected API
    cap.open(deviceID + apiID);

    if (!cap.isOpened()) {
        print("ERROR! Unable to open camera");
        return -1;
    }
    cap.grab();
    int attempts = 10;
    do {
        cap.read(frame);
        if (!frame.empty()) {
            break;
        }
        print("Warning: cannot get frame, attempt left %d");
    } while (--attempts > 0);

    if (attempts == 0) {
        print("ERROR: Cannot get frame, return -1");
        return -1;
    }

    check_camera_config_with_default(&frame);

    mat_to_int_arr(&frame, &cam->data[0]);

    frame.deallocate();

    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif
int get_frame_from_camera(struct camera_ctx *ctx) {
    assert(ctx);
    int retval = 0;
    if (ctx->isBusy == true) {
        print("WARNING: camera ctx is busy, return -1");
        return -1;
    }

    ctx->isBusy = true;
    retval = get_next_frame(ctx);
    ctx->isBusy = false;

    assert(!retval);

    return retval;
}
#ifdef __cplusplus
}
#endif
