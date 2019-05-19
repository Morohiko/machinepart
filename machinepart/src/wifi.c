#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>

#include "string.h"
#include "log.h"
#include "wifi.h"

// by default s_addr = htonl(INADDR_ANY), all interfaces
int create_tcp_socket(struct tcp_socket *sock, int port) {
    assert(sock);

    sock->listenfd = socket(AF_INET, SOCK_STREAM, 0);
    sock->connfd = 0;
   
    if (sock->listenfd <= 0) {
        print("ERROR: cannot create socket");
	return -1;
    }

    sock->serv_addr.sin_family = AF_INET;
    sock->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock->serv_addr.sin_port = htons(port);

    bind(sock->listenfd, (struct sockaddr*)&sock->serv_addr, sizeof(sock->serv_addr));

// Make socket pasive
    print("DEBUG: socket created");

    return 0;
}

int listen_tcp_connection(struct tcp_socket *sock, int max_connection) {
    assert(sock);
    if (sock->listenfd < 0) {
        print("ERROR: cannot start listen socket cause fd = %d", sock->listenfd);
	return -1;
    }

    return listen(sock->listenfd, max_connection);
}

int accept_tcp_connection(struct tcp_socket *sock) {
    assert(sock);

    if (sock->connfd < 0) {
        print("ERROR: cannot accept, config is %d", sock->connfd);
	return -1;
    }

    sock->connfd = accept(sock->listenfd, (struct sockaddr*)NULL, NULL);
    print("DEBUG: accepted!");
    return 0;
}

int send_tcp_message(struct tcp_socket *sock, char *msg) {
    assert(sock); assert(msg);

    if (sock->connfd <= 0) {
        print("ERROR: socket was not created, maybe accept it?");
	return -1;
    }

    print("DEBUG: send message \"%s\"", msg);

    send(sock->connfd, msg, strlen(msg), 0);

    return 0;
}

int recv_tcp_message(struct tcp_socket *sock, char *msg) {
    assert(sock); assert(msg);

    if (sock->connfd <= 0) {
        print("ERROR: socket was not created, maybe accept it?");
	return -1;
    }

    ssize_t size = recv(sock->connfd, msg, strlen(msg), 0);
    if (size == 0) {
        print("ERROR: recv tcp message size == 0");
	return -1;
    }
    print("DEBUG: recv tcp size = %zu msg = %s", size, msg);

    return 0;
}

int close_socket(struct tcp_socket *sock) {
    assert(sock);

    if (sock->connfd <= 0) {
        print("ERROR: cannot close socket, connfd = %d", sock->connfd);
	return -1;
    }

    close(sock->connfd);

    if (sock->listenfd <= 0) {
        print("ERROR: cannot close socket, listenfd = %d", sock->listenfd);
	return -1;
    }

    close(sock->listenfd);

    print("DEBUG: socket was closed");

    return 0;
}


