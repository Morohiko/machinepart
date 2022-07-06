#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "assert.h"
#include <iostream>
#include <unistd.h>

#include "camera/camera_opencv.h"
#include "json_config.h"

#define CAMERA_FRAME_TYPE CV_8UC3

#ifdef __cplusplus
extern "C" {
#endif
#include "config.h"
#include "log.h"
#ifdef __cplusplus
}
#endif

using namespace cv;
using namespace std;

static int mat_to_int_arr(Mat *src, int *dest) {
  assert(src);
  assert(src->rows);
  assert(src->cols);
  assert(src->data);
  assert(dest);

  print(DEBUG, "start convert mat to int array");

  int count = 0;
  for (int i = 0; i < src->cols; ++i) {
    for (int j = 0; j < src->rows; ++j) {
      dest[count++] = src->at<int>(j, i);
    }
  }
  print(DEBUG, "count = %d", count);
  return 0;
}

static int int_arr_to_mat(int *src, Mat *dest) {
  assert(src);
  assert(dest);

  int count = 0;
  for (int i = 0; i < dest->cols; ++i) {
    for (int j = 0; j < dest->rows; ++j) {
      dest->at<int>(j, i) = src[count++];
    }
  }

  return 0;
}

static int check_camera_config_with_default(Mat *frame) {
  if (json_config.modules.camera_module.frame_width != frame->cols) {
    print(ERROR, "camera frame width: config = %d, camera = %d",
          json_config.modules.camera_module.frame_width, frame->rows);
    return -1;
  }

  if (json_config.modules.camera_module.frame_height != frame->rows) {
    print(ERROR, "camera frame height: config = %d, camera = %d",
          json_config.modules.camera_module.frame_height, frame->cols);
    return -1;
  }

  if (CAMERA_FRAME_TYPE != frame->type()) {
    print(ERROR, "camera frame type: config = %d, camera = %d",
          CAMERA_FRAME_TYPE, frame->type());
    return -1;
  }

  if (json_config.modules.camera_module.frame_elem_size != frame->elemSize()) {
    print(ERROR, "camera frame elem size: config = %d, camera = %zu",
          json_config.modules.camera_module.frame_elem_size, frame->elemSize());
    return -1;
  }

  print(DEBUG, "camera config corresponds with config.h");
  return 0;
}

#if 0
static int resize_image() {
    cv::resize(frame, frame, cv::Size(256, 256), 0, 0, cv::INTER_LINEAR);
    return 0;
}
#endif

static int compress_image(cv::Mat *mat, int *data) {
  std::vector<uchar> buff; // buffer for coding
  std::vector<int> param(2);
  param[0] = cv::IMWRITE_JPEG_QUALITY;
  param[1] = 80; // default(95) 0-100
  // cvDecodeImage(mat);
  //    cv::imencode(".jpg", mat, buff, param);

  std::copy(buff.begin(), buff.end(), data);
}

#include <unistd.h>
// write to arguments received frame and size from cam
// the function itself allocate memory for frame
int get_next_frame(int *data, int *size, bool *isBusy, bool *isNewData) {
  Mat frame;
  VideoCapture cap(0);
  //    VideoCapture
  //    cap("/home/dmytro/projs/headmachine/machinepart/rsc/test_640x480_video.mp4");

  if (!cap.isOpened()) {
    print(ERROR, "Cannot open the web cam");
    return -1;
  }
  print(INFO, "Camera is opened");

  int deviceID = 0;        // 0 = open default camera
  int apiID = cv::CAP_ANY; // 0 = autodetect default API

  // open selected camera using selected API
  cap.open(deviceID + apiID);

  if (!cap.isOpened()) {
    print(ERROR, "Unable to open camera");
    return -1;
  }

  cap.grab(); // for what?

#if 0

    Mat edges;
    namedWindow("edges",1);

    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        imshow("edges", edges);
        sleep(1999);
v//        if(waitKey(30) >= 0) break;
    }
#else
  Mat edges;
  //    namedWindow("edges",1);
  *size = json_config.modules.camera_module.frame_width * json_config.modules.camera_module.frame_height * json_config.modules.camera_module.frame_elem_size;
  for (;;) {
    Mat frame;
    cap >> frame;
    //        cvtColor(frame, edges, COLOR_BGR2GRAY);
    //        imshow("edges", edges);
    if (waitKey(30) >= 0)
      break;
    sleep(1);
    //        print(DEBUG, "isbusy value = %d", *isBusy);
    if (!(*isBusy)) {
      print(DEBUG, "not is busy, try save data");
      *isBusy = true;
      check_camera_config_with_default(&frame);
      mat_to_int_arr(&frame, data);
      *isNewData = true;
      *isBusy = false;
    } else {
      print(DEBUG, "is busy now, continue");
      continue;
    }

    //        if(waitKey(30) >= 0) break;
  }

#endif

  frame.deallocate();
  print(DEBUG, "frame dealocated");
  return 0;
}

#ifdef __cplusplus
extern "C" {
#endif
int get_frame_from_camera(int *data, int *size, bool *isBusy, bool *isNewData) {
  assert(data);
  assert(size);
  assert(isBusy);

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
}

#ifdef __cplusplus
}
#endif
