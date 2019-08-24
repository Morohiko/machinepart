#include "camera/camera.h"
#include "log.h"
#include "wifi.h"

#include <unistd.h>
#include <string.h>
#include <assert.h>

static void *camera_receiver_thread(void *user_data) {
    struct camera_ctx *cam_ctx = (struct camera_ctx *) user_data;

    start_recv_camera_data_through_udp(cam_ctx);
}

int config_network(struct connection_info *conn_info) {
    memcpy(conn_info->local_ip, LOCAL_IP, 16);
    memcpy(conn_info->target_ip, LOCAL_IP, 16);

    assert(conn_info->local_ip);
    assert(conn_info->target_ip);

//inverted cause tested on local machine
    conn_info->local_port = TARGET_GYROSCOPE_PORT;
    conn_info->target_port = LOCAL_GYROSCOPE_PORT;
    print(INFO, "selected connection:\n local ip: %s, target ip: %s, local port: %d, target port: %d", 
                 conn_info->local_ip, conn_info->target_ip, conn_info->local_port, conn_info->target_port);

    return 0;
}

int main(int argc, char** argv){
    set_log_level(DEBUG);

    struct camera_ctx cam_ctx;

    config_network(&cam_ctx.conn);

    pthread_t cameraReceiverThreadID;
    pthread_create(&cameraReceiverThreadID, NULL, camera_receiver_thread, &cam_ctx);


    sleep(99999);

    return 0;
}
