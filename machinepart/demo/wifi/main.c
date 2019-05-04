#include "wifi.h"
#include "log.h"
#include "string.h"

#include <unistd.h>
#include <assert.h>

#define WIFI_PORT 3333
#define MAX_TCP_CONNECTION 10

int send_message_through_tcp(int port, char *msg, int quantity) {
    assert(msg);

    struct tcp_socket sock;
   
    if (create_tcp_socket(&sock, port) != 0) {
        print("ERROR: cannot create socket");
	return -1;
    }

    print("DEBUG: socket created");
    if (listen_tcp_connection(&sock, MAX_TCP_CONNECTION) != 0) {
        print("ERROR, cannot start tcp listen connection");
        return -1;
    }

    print("DEBUG: socket created");
    if (accept_tcp_connection(&sock) != 0) {
        print("ERROR, cannot accept connection");
        return -1;
    }

    print("DEBUG: connection accepted");

    while(quantity--) {
        if (send_tcp_message(&sock, msg) < 0) {
	    print("ERROR: cannot send message");
	    break;
	}

        sleep(1);
    }

    close_socket(&sock);

    print("DEBUG: look like send msg through tcp is good working");

    return 0;
}

int recv_message_through_tcp(int port, char *msg, int quantity) {
    assert(msg);

    struct tcp_socket sock;
   
    if (create_tcp_socket(&sock, port) != 0) {
        print("ERROR: cannot create socket");
	return -1;
    }

    print("DEBUG: socket created");
    if (listen_tcp_connection(&sock, MAX_TCP_CONNECTION) != 0) {
        print("ERROR, cannot start tcp listen connection");
        return -1;
    }

    print("DEBUG: socket created");
    if (accept_tcp_connection(&sock) != 0) {
        print("ERROR, cannot accept connection");
        return -1;
    }

    print("DEBUG: connection accepted");

    while(quantity--) {
        if (recv_tcp_message(&sock, msg) < 0) {
	    print("ERROR: cannot recv message");
	    break;
	}

	print("DEBUG: received msg = %s", msg);
        sleep(1);
    }

    close_socket(&sock);

    print("DEBUG: look like recv msg through tcp is good working");

    return 0;
}

int main() {
    print("hello world");

    int status = 0;

    char *message = "test123";
//    char message[100];
    send_message_through_tcp(WIFI_PORT, message, 5);
//    recv_message_through_tcp(WIFI_PORT, message, 5);

    return 0;
}
