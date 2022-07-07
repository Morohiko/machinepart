#include "config.h"
#include "json_config.h"
#include "log.h"
#include "string.h"
#include "wifi.h"

#include <assert.h>
#include <unistd.h>

int send_message_through_udp(char *msg, size_t msg_size, int quantity) {
  assert(msg);

  struct udp_socket sock;

  print(DEBUG, "local ip = %s", json_config.modules.main_module.local_ip);
  print(DEBUG, "target ip = %s", json_config.modules.main_module.target_ip);

  print(DEBUG, "local port = %d", LOCAL_TEST_PORT);
  print(DEBUG, "target port = %d", TARGET_TEST_PORT);

  if (create_udp_socket(&sock, json_config.modules.main_module.local_ip,
                        json_config.modules.main_module.target_ip,
                        LOCAL_TEST_PORT, TARGET_TEST_PORT) != 0) {
    print(ERROR, "cannot create socket");
    return -1;
  }

  print(DEBUG, "socket created");

  while (quantity--) {
    if (send_udp_message(&sock, msg, msg_size) < 0) {
      print(ERROR, "cannot send message");
      break;
    }
    print(DEBUG, "sended message = %s", msg);
    sleep(1);
  }

  if (close_udp_socket(&sock)) {
    print(ERROR, "cannot close socket");
    return -1;
  }

  print(DEBUG, "look like send msg through udp is good working");

  return 0;
}

int recv_message_through_udp(char *msg, size_t msg_size, int quantity) {
  assert(msg);

  struct udp_socket sock;

  if (create_udp_socket(&sock, json_config.modules.main_module.local_ip,
                        json_config.modules.main_module.target_ip,
                        LOCAL_TEST_PORT, TARGET_TEST_PORT) != 0) {
    print(ERROR, "cannot create socket");
    return -1;
  }

  print(DEBUG, "socket created");

  while (quantity--) {
    if (recv_udp_message(&sock, msg, msg_size) < 0) {
      print(ERROR, "cannot send message");
      break;
    }

    if (msg != NULL) {
      print(DEBUG, "received udp msg: %s", msg);
    }
    //        sleep(1);
  }

  if (close_udp_socket(&sock)) {
    print(ERROR, "cannot close socket");
    return -1;
  }

  print(DEBUG, "look like recv msg through udp is good working");

  return 0;
}

int main() {
  set_log_level(DEBUG);
#if 0
    char *message = "0:0:0";
    send_message_through_udp(message, 8, 20);
#else
  char message[100];
  recv_message_through_udp(message, 100, 5);
#endif
  return 0;
}
