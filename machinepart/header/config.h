#ifndef CONFIG_H
#define CONFIG_H

#define MAX_TCP_CONNECTION 10

//************ ip/port *************//
// #define mp_ip "0.0.0.0"
// #define gp_ip "192.168.43.162"

// #define LOCAL_GYROSCOPE_PORT 3333
// #define TARGET_GYROSCOPE_PORT 3334

// #define LOCAL_CAMERA_FRAME_PORT 3335
// #define TARGET_CAMERA_FRAME_PORT 3336

// #define LOCAL_CAMERA_ACK_PORT 3337
// #define TARGET_CAMERA_ACK_PORT 3338

// #define CONTROLLER_PORT 3400
// #define MAX_CONTROLLER_CONNECTION 20
// #define TARGET_CONTROLLER_PORT 3332

// using in wifi tcp/udp demoes
#define LOCAL_TEST_PORT 3339
#define TARGET_TEST_PORT 3340

//*********** camera config ***********//
//#define CAMERA_FRAME_WIDTH 640
//#define CAMERA_FRAME_HEIGHT 480
// #define CAMERA_FRAME_WIDTH 350
// #define CAMERA_FRAME_HEIGHT 350
// #define CAMERA_DATA_SIZE CAMERA_FRAME_WIDTH *CAMERA_FRAME_HEIGHT * 2

// #define CAMERA_FRAME_TYPE CV_8UC3
// #define CAMERA_FRAME_ELEM_SIZE 2

// capture messages cutted to mtu message
// compatible with gp
// simplifier logic on gp
//#define SEND_MTU_SIZE_MESSAGES
#ifdef SEND_MTU_SIZE_MESSAGES
#define MTU_SIZE_MESSAGE 131072
#endif

//********** fps checker *************//
#define WITH_FPS_CHECKER
// #ifdef WITH_FPS_CHECKER
// #define FPS_UPDATE_VALUE_EVERY 5
// #endif // WITH_FPS_CHECKER

//********* controller **************//
// ifndef then default - stdin
// #define REMOTE_CONTROLLER

//********* motor module ************//
// #define MOTORS_DELAY_MS 20

// #define MOTOR_X_GPIO_PIN 18
// #define MOTOR_Y_GPIO_PIN 17

//******* json config filepath ******//
// #define JSON_CONFIG_FILE \
//   "/home/user/work/machinepart/machinepart/src/config/config.json"

#endif // CONFIG_H
