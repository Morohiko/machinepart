#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/videodev2.h>

#include "camera/camera.h"
#include "camera/camera_transmitter.h"

#include "log.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define FILEPATH "testdata"

struct buffer {
  void *start;
  size_t length;
};

struct buffer *buffers;
static int fd = -1;
static char *dev_name = "/dev/video0";
static int xioctl(int fh, int request, void *arg) {
  int r;
  do {
    r = ioctl(fh, request, arg);
  } while (r == -1 && EINTR == errno);

  return r;
}

static int open_device() {
  struct stat st;

  if (-1 == stat(dev_name, &st)) {
    print(ERROR, "Cannot identify '%s': %d, %s\\n", dev_name, errno,
          strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (!S_ISCHR(st.st_mode)) {
    print(ERROR, "%s is no devicen", dev_name);
    exit(EXIT_FAILURE);
  }

  print(DEBUG, "st_mode = %d", st.st_mode);

  fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);

  if (-1 == fd) {
    print(ERROR, "Cannot open '%s': %d, %s\\n", dev_name, errno,
          strerror(errno));
    exit(EXIT_FAILURE);
  }
}

static int method_read() {
  open_device();
  // init device
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format fmt;
  unsigned int min;
  if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
    if (EINVAL == errno) {
      print(ERROR, "%s is no V4L2 device\\n", dev_name);
      return -1;
    } else {
      print(ERROR, "VIDIOC_QUERYCAP");
      return -1;
    }
  }
  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    print(ERROR, "%s is no video capture device\\n", dev_name);
    return -1;
  }
  if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
    print(ERROR, "%s does not support read i/o\\n", dev_name);
    return -1;
  }

  /* Select video input, video standard and tune here. */

  CLEAR(cropcap);

  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; /* reset to default */

    if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
      switch (errno) {
      case EINVAL:
        /* Cropping not supported. */
        break;
      default:
        /* Errors ignored. */
        break;
      }
    }
  } else {
    /* Errors ignored. */
  }

  CLEAR(fmt);

  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = 640;
  fmt.fmt.pix.height = 480;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

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

  buffers = calloc(1, sizeof(fmt.fmt.pix.sizeimage));

  if (!buffers) {
    print(ERROR, "Out of memory\\n");
    exit(EXIT_FAILURE);
  }

  buffers[0].length = fmt.fmt.pix.sizeimage;
  buffers[0].start = malloc(fmt.fmt.pix.sizeimage);

  if (!buffers[0].start) {
    print(ERROR, "Out of memory\\n");
    exit(EXIT_FAILURE);
  }

  // start capturing
  // nothing to do

  // main loop capture

  //    unsigned int count;

  //    count = frame_count;
  int count = 1;
  while (count-- > 0) { // one frame
                        //    for (;;) {
    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    /* Timeout. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    r = select(fd + 1, &fds, NULL, NULL, &tv);

    if (r == -1) {
      print(ERROR, "cannot do select in camera");
      return -1;
      //            if (EINTR == errno)
      //                continue;
      //            errno_exit("select");
    }

    if (0 == r) {
      print(ERROR, "select timeout\\n");
      exit(EXIT_FAILURE);
    }
    // read frame
    if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
      switch (errno) {
      case EAGAIN:
        print(ERROR, "EAGAIN");
        return 0;

      case EIO:
        /* Could ignore EIO, see spec. */
        /* fall through */

      default:
        print(ERROR, "cannot read from camera");
        return -1;
        //                errno_exit("read");
      }
    }

    print(DEBUG, "camera buffer size = %zu", buffers[0].length);
    // process image
    int out_buf = 1;
    if (out_buf) {
      FILE *out_file_fd = fopen("testfile", "wb");
      fwrite(buffers[0].start, buffers[0].length, 1, out_file_fd);
      //            fwrite(buffers[0].start, buffers[0].length, 1, stdout);
    }
    fflush(stderr);
    fprintf(stderr, ".");
    fflush(stdout);
  }
}
// *** machine part api ***
struct camera_ctx cam_ctx;

static void *try_save_image_to_file() {
  int request_img;
  print(INFO, "started controller thread");
  while (1) {
    scanf("%d", &request_img);
    save_current_image_to_file(&cam_ctx, FILEPATH);
  }
}

int main() {
  set_log_level(DEBUG);
  int retval;
#define USE_MACHINEPART_API
#ifndef USE_MACHINEPART_API
  // deprecated, just use machinepart api
  retval = method_read();
#else
  // *** for testing only ***
  pthread_t controllerThread;

  pthread_create(&controllerThread, NULL, try_save_image_to_file, NULL);
  // *** for testing only ***

  retval = start_camera(&cam_ctx);
#endif

  if (retval != 0) {
    print(ERROR, "error in method read");
    return -1;
  }
  return 0;
}
