#include "wifi.h"
#include "log.h"
#include "string.h"
#include "config.h"

#include <unistd.h>
#include <assert.h>

int send_message_through_udp(char *msg, int quantity) {
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

    while(quantity--) {
        if (send_udp_message(&sock, msg) < 0) {
	    print("ERROR: cannot send message");
	    break;
	}

        sleep(1);
    }

    if (close_udp_socket(&sock)) {
        print("ERROR: cannot close socket");
	return -1;
    }

    print("DEBUG: look like send msg through udp is good working");

    return 0;
}

int recv_message_through_udp(char *msg, int quantity) {
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

    while(quantity--) {
        if (recv_udp_message(&sock, msg) < 0) {
	    print("ERROR: cannot send message");
	    break;
	}

	if (msg != NULL) {
	    print("DEBUG: received udp msg: %s", msg);
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

int main() {
    char *message = "test123";
    send_message_through_udp(message, 20);

//    char message[100];
//    recv_message_through_udp(message, 5);

    return 0;
}
