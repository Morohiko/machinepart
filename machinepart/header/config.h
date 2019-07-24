#ifndef CONFIG_H
#define CONFIG_H

#define MAX_TCP_CONNECTION 10
#define LOCAL_PORT 3333
#define TARGET_PORT 3334

#define WITH_SELECTED_IP
#ifdef WITH_SELECTED_IP
  #define LOCAL_IP "192.168.43.154"
  #define TARGET_IP "192.168.43.47"
#endif

#define CAMERA_FRAME_WIDTH 640
#define CAMERA_FRAME_HEIGHT 480
#define CAMERA_FRAME_TYPE CV_8UC3
#define CAMERA_FRAME_ELEM_SIZE 3

#endif
