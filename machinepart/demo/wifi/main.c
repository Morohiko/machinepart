#include "wifi.h"

#include "string.h"
#include "stdio.h"

#include <unistd.h>
#include <assert.h>

#define WIFI_PORT 3333
#define MAX_TCP_CONNECTION 10

int send_message_through_tcp(int port, char *msg, int quantity) {
    assert(msg);

    struct tcp_socket sock;
   
    if (create_tcp_socket(&sock, port) != 0) {
        printf("ERROR: cannot create socket\n");
	return -1;
    }

    printf("DEBUG: socket created\n");
    if (listen_tcp_connection(&sock, MAX_TCP_CONNECTION) != 0) {
        printf("ERROR, cannot start tcp listen connection\n");
        return -1;
    }

    printf("DEBUG: socket created\n");
    if (accept_tcp_connection(&sock) != 0) {
        printf("ERROR, cannot accept connection\n");
        return -1;
    }

    printf("DEBUG: connection accepted\n");

    while(quantity--) {
        if (send_tcp_message(&sock, msg) < 0) {
	    printf("ERROR: cannot send message\n");
	    break;
	}

        sleep(1);
    }

    close_socket(&sock);

    printf("DEBUG: look like send msg through tcp is good working\n");

    return 0;
}

int recv_message_through_tcp(int port, char *msg, int quantity) {
    assert(msg);

    struct tcp_socket sock;
   
    if (create_tcp_socket(&sock, port) != 0) {
        printf("ERROR: cannot create socket\n");
	return -1;
    }

    printf("DEBUG: socket created\n");
    if (listen_tcp_connection(&sock, MAX_TCP_CONNECTION) != 0) {
        printf("ERROR, cannot start tcp listen connection\n");
        return -1;
    }

    printf("DEBUG: socket created\n");
    if (accept_tcp_connection(&sock) != 0) {
        printf("ERROR, cannot accept connection\n");
        return -1;
    }

    printf("DEBUG: connection accepted\n");

    while(quantity--) {
        if (recv_tcp_message(&sock, msg) < 0) {
	    printf("ERROR: cannot recv message\n");
	    break;
	}

	printf("DEBUG: received msg = %s\n", msg);
        sleep(1);
    }

    close_socket(&sock);

    printf("DEBUG: look like recv msg through tcp is good working\n");

    return 0;
}

int main() {
    printf("hello world\n");

    int status = 0;

//    char *message = "test123";
    char message[100];
//    send_message_through_tcp(WIFI_PORT, message, 5);
    recv_message_through_tcp(WIFI_PORT, message, 5);

    return 0;
}
