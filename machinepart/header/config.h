#ifndef CONFIG_H
#define CONFIG_H

#define MAX_TCP_CONNECTION 10

//************ ip/port *************//
#define LOCAL_IP "0.0.0.0"
#define TARGET_IP "192.168.43.177"
//#define TARGET_IP "127.0.0.1"
//#define TARGET_IP "127.0.0.1"
//#define TARGET_IP "172.24.223.80"

#define LOCAL_CONTROLLER_PORT 3331
#define TARGET_CONTROLLER_PORT 3332

#define LOCAL_GYROSCOPE_PORT 3333
#define TARGET_GYROSCOPE_PORT 3334

#define LOCAL_CAMERA_FRAME_PORT 3335
#define TARGET_CAMERA_FRAME_PORT 3336

#define LOCAL_CAMERA_ACK_PORT 3337
#define TARGET_CAMERA_ACK_PORT 3338

//using in wifi tcp/udp demoes
#define LOCAL_TEST_PORT 3339
#define TARGET_TEST_PORT 3340


//*********** camera config ***********//
//#define CAMERA_FRAME_WIDTH 640
//#define CAMERA_FRAME_HEIGHT 480
#define CAMERA_FRAME_WIDTH 350
#define CAMERA_FRAME_HEIGHT 350
#define CAMERA_DATA_SIZE CAMERA_FRAME_WIDTH*CAMERA_FRAME_HEIGHT*2

#define CAMERA_FRAME_TYPE CV_8UC3
#define CAMERA_FRAME_ELEM_SIZE 2


//*********** modules name ************//
#define MAIN_MODULE "MAIN_MODULE"
#define REMOTE_CONTROLLER_MODULE "REMOTE_CONTROLLER_MODULE"
#define CAMERA_MODULE "CAMERA_MODULE"
#define CAMERA_TRANSMITTER_MODULE "CAMERA_TRANSMITTER_MODULE"
#define GYROSCOPE_RECEIVER_MODULE "GYROSCOPE_RECEIVER_MODULE"
#define MOTOR_CONTROLLER_MODULE "MOTOR_CONTROLLER_MODULE"

// capture messages cutted to mtu message
// compatible with gp
// simplifier logic on gp
//#define SEND_MTU_SIZE_MESSAGES
#ifdef SEND_MTU_SIZE_MESSAGES
#define MTU_SIZE_MESSAGE 131072
#endif

//********** fps checker *************//
#define WITH_FPS_CHECKER
#ifdef WITH_FPS_CHECKER
#define FPS_UPDATE_VALUE_EVERY 5
#endif // WITH_FPS_CHECKER

//********* controller **************//
// ifndef then default - stdin
#define REMOTE_CONTROLLER

#endif // CONFIG_H
