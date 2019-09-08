#include "wifi.h"
#include "log.h"
#include "config.h"
#include "string.h"

#include <unistd.h>
#include <assert.h>

int send_message_through_tcp(int port, char *msg, size_t msg_size, int quantity) {
    assert(msg);

    struct tcp_socket sock;
   
    if (create_tcp_socket(&sock, port) != 0) {
        print(ERROR, "cannot create socket");
        return -1;
    }

    print(DEBUG, "socket created");
    if (listen_tcp_connection(&sock, MAX_TCP_CONNECTION) != 0) {
        print(ERROR, "cannot start tcp listen connection");
        return -1;
    }

    print(DEBUG, "socket created");
    if (accept_tcp_connection(&sock) != 0) {
        print(ERROR, "cannot accept connection");
        return -1;
    }

    print(DEBUG, "connection accepted");

    while(quantity--) {
        if (send_tcp_message(&sock, msg, msg_size) < 0) {
            print(ERROR, "cannot send message");
            break;
        }
        sleep(1);
    }

    close_tcp_socket(&sock);

    print(DEBUG, "look like send msg through tcp is good working");

    return 0;
}

int recv_message_through_tcp(int port, char *msg, size_t msg_size, int quantity) {
    assert(msg);

    struct tcp_socket sock;
   
    if (create_tcp_socket(&sock, port) != 0) {
        print(ERROR, "cannot create socket");
        return -1;
    }

    print(DEBUG, "socket created");
    if (listen_tcp_connection(&sock, MAX_TCP_CONNECTION) != 0) {
        print(ERROR, "cannot start tcp listen connection");
        return -1;
    }

    print(DEBUG, "socket created");
    if (accept_tcp_connection(&sock) != 0) {
        print(ERROR, "cannot accept connection");
        return -1;
    }

    print(DEBUG, "connection accepted");

    while(quantity--) {
        if (recv_tcp_message(&sock, msg, msg_size) < 0) {
            print(ERROR, "cannot recv message");
            break;
        }

        print(DEBUG, "received msg = %s", msg);
        sleep(1);
    }

    close_tcp_socket(&sock);

    print(DEBUG, "look like recv msg through tcp is good working");

    return 0;
}

int main() {
    set_log_level(DEBUG);

    char *message = "test123";

    send_message_through_tcp(TARGET_TEST_PORT, message, 8, 5);

//    char message[100];
//    recv_message_through_tcp(TARGET_TEST_PORT, message, 100, 5);

    return 0;
}
