#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "log.h"
#include "string.h"
#include "wifi.h"

// by default s_addr = htonl(INADDR_ANY), all interfaces
int create_tcp_socket(struct tcp_socket *sock, int port) {
  assert(sock);

  sock->listenfd = socket(AF_INET, SOCK_STREAM, 0);
  sock->connfd = 0;

  if (sock->listenfd <= 0) {
    print(ERROR, "cannot create socket");
    return -1;
  }

  sock->serv_addr.sin_family = AF_INET;
  sock->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sock->serv_addr.sin_port = htons(port);

  bind(sock->listenfd, (struct sockaddr *)&sock->serv_addr,
       sizeof(sock->serv_addr));
  print(DEBUG, "socket created");

  return 0;
}

int listen_tcp_connection(struct tcp_socket *sock, int max_connection) {
  assert(sock);
  if (sock->listenfd < 0) {
    print(ERROR, "cannot start listen socket cause fd = %d", sock->listenfd);
    return -1;
  }

  return listen(sock->listenfd, max_connection);
}

int accept_tcp_connection(struct tcp_socket *sock) {
  assert(sock);

  if (sock->connfd < 0) {
    print(ERROR, "cannot accept, config is %d", sock->connfd);
    return -1;
  }

  sock->connfd = accept(sock->listenfd, (struct sockaddr *)NULL, NULL);
  print(DEBUG, "accepted!");
  return 0;
}

int send_tcp_message(struct tcp_socket *sock, char *msg, size_t msg_size) {
  assert(sock);
  assert(msg);

  if (sock->connfd <= 0) {
    print(ERROR, "socket was not created, maybe accept it?");
    return -1;
  }

  int sended = send(sock->connfd, msg, msg_size, 0);

  return 0;
}

int recv_tcp_message(struct tcp_socket *sock, char *msg, size_t msg_size) {
  assert(sock);
  assert(msg);

  if (sock->connfd <= 0) {
    print(ERROR, "socket was not created, maybe accept it?");
    return -1;
  }

  ssize_t size = recv(sock->connfd, msg, msg_size, 0);
  if (size == 0) {
    print(ERROR, "recv tcp message size == 0");
    return -1;
  }

  return 0;
}

int close_tcp_socket(struct tcp_socket *sock) {
  assert(sock);

  if (sock->connfd <= 0) {
    print(ERROR, "cannot close socket, connfd = %d", sock->connfd);
    return -1;
  }

  close(sock->connfd);

  if (sock->listenfd <= 0) {
    print(ERROR, "cannot close socket, listenfd = %d", sock->listenfd);
    return -1;
  }

  close(sock->listenfd);

  print(DEBUG, "socket was closed");

  return 0;
}

int create_udp_socket(struct udp_socket *sock, const char *local_ip,
                      const char *target_ip, int local_port, int target_port) {
  assert(sock);

  sock->sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sock->sock_fd <= 0) {
    print(ERROR, "cannot create socket");
    return -1;
  }

  bzero(&sock->local_sock, sizeof(sock->local_sock));
  sock->local_sock.sin_family = AF_INET;
  sock->local_sock.sin_port = htons(local_port);

  assert(local_ip);

  if (!inet_aton(local_ip, &sock->local_sock.sin_addr)) {
    print(ERROR, "cannot do inet_aton, local_ip = %s", local_ip);
    return -1;
  }

  bind(sock->sock_fd, (struct sockaddr *)&sock->local_sock,
       sizeof(sock->local_sock));

  print(DEBUG, "local socket created with port = %d", local_port);

  bzero(&sock->target_sock, sizeof(sock->target_sock));
  sock->target_sock.sin_family = AF_INET;
  sock->target_sock.sin_port = htons(target_port);

  assert(target_ip);

  if (!inet_aton(target_ip, &sock->target_sock.sin_addr)) {
    print(ERROR, "cannot do inet_aton, target_ip = %s", target_ip);
    return -1;
  }

  print(DEBUG, "target sock is on port = %d", target_port);

  return 0;
}

int send_udp_message(struct udp_socket *sock, char *msg, size_t msg_size) {
  assert(sock);
  assert(msg);

  if (sock->sock_fd <= 0) {
    print(ERROR, "socket was not created, maybe accept it?");
    return -1;
  }

  sendto(sock->sock_fd, msg, msg_size, 0, (struct sockaddr *)&sock->target_sock,
         sizeof(sock->target_sock));

  return 0;
}

int recv_udp_message(struct udp_socket *sock, char *msg, size_t msg_size) {
  assert(sock);
  assert(msg);

  if (sock->sock_fd <= 0) {
    print(ERROR, "socket was not created, maybe accept it?");
    return -1;
  }

  recvfrom(sock->sock_fd, msg, msg_size, 0,
           (struct sockaddr *)&sock->target_sock, sizeof(sock->target_sock));

  return 0;
}

int close_udp_socket(struct udp_socket *sock) {
  assert(sock);

  if (sock->sock_fd < 0) {
    print(ERROR, "cannot close udp socket cause fd = %d", sock->sock_fd);
    return -1;
  }

  close(sock->sock_fd);

  print(DEBUG, "udp socket is closed");
  return 0;
}
