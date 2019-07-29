#include "gyroscope_receiver.h"

#include "wifi.h"
#include "log.h"
#include "string.h"
#include "config.h"
#include <assert.h>
#define MAX_GYROSCOPE_DATA_SIZE 15

//parse like X:Y:Z 
static int parse_gyroscope_data(char *data) {
    int x = 0;
    int y = 0;
    int z = 0;

    sscanf(data, "%d:%d:%d", &x, &y, &z);

    print("DEBUG: try parse gyroscope data: %s, x = %d, y = %d, z = %d", data, x, y, z);
    return 0;
}

int recv_gyroscope_data(struct gyroscope_ctx *ctx) {
    char msg[MAX_GYROSCOPE_DATA_SIZE];

    assert(msg);

    struct udp_socket sock;
   
    if (create_udp_socket(&sock,
#ifdef WITH_SELECTED_IP
                          LOCAL_IP, TARGET_IP,
#endif
                          LOCAL_PORT, TARGET_PORT) != 0) {
        print("ERROR: cannot create socket");
    return -1;
    }

    print("DEBUG: socket created");

    while(1) {
        memset(msg, '\0', MAX_GYROSCOPE_DATA_SIZE);
        if (recv_udp_message(&sock, msg, MAX_GYROSCOPE_DATA_SIZE) < 0) {
            print("ERROR: cannot send message");
            break;
        }

    if (msg != NULL) {
        print("DEBUG: received udp msg: %s", msg);
    }

    if (parse_gyroscope_data(msg)) {
        print("ERROR: cant parse gyroscope data, msg: %s", msg);
        break;
    }
//        sleep(1);
    }

    if (close_udp_socket(&sock)) {
        print("ERROR: cannot close socket");
        return -1;
    }

    print("DEBUG: look like recv msg through udp is good working");

    return 0;
}


