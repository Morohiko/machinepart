#include <netinet/in.h>

struct tcp_socket {
    struct sockaddr_in serv_addr;

    int listenfd;
    int connfd;
};

int create_tcp_socket(struct tcp_socket *sock, int port);
int listen_tcp_connection(struct tcp_socket *sock, int max_connection);
int accept_tcp_connection(struct tcp_socket *sock);
int close_socket();

int send_tcp_message(struct tcp_socket *sock, char *msg);
int recv_tcp_message(struct tcp_socket *sock, char *msg);
