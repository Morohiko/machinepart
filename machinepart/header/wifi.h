#ifndef WIFI_H
#define WIFI_H

#include <netinet/in.h>

#include "config.h"

struct connection_info {
  char mp_ip[16];
  char gp_ip[16];

  int mp_port;
  int gp_port;
};

struct connection_info_cam {
  char mp_ip[16];
  char gp_ip[16];

  int frame_mp_port;
  int frame_gp_port;

  int ack_mp_port;
  int ack_gp_port;
};

struct tcp_socket {
  struct sockaddr_in serv_addr;

  int listenfd;
  int connfd;
};

int create_tcp_socket(struct tcp_socket *sock, int port);
int listen_tcp_connection(struct tcp_socket *sock, int max_connection);
int accept_tcp_connection(struct tcp_socket *sock);
int close_tcp_socket();

int send_tcp_message(struct tcp_socket *sock, char *msg, size_t msg_size);
int recv_tcp_message(struct tcp_socket *sock, char *msg, size_t msg_size);

struct udp_socket {
  int sock_fd;

  struct sockaddr_in local_sock;
  struct sockaddr_in target_sock;
};

int create_udp_socket(struct udp_socket *sock, const char *mp_ip,
                      const char *gp_ip, int mp_port, int gp_port);
int close_udp_socket(struct udp_socket *sock);
int send_udp_message(struct udp_socket *sock, char *msg, size_t msg_size);
int recv_udp_message(struct udp_socket *sock, char *msg, size_t msg_size);

#endif // WIFI_H
