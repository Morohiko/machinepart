#include "unistd.h"
#include <stdlib.h>

#include "json_config.h"
#include "log.h"
#include "wifi.h"

int gyroscope_receiver_logic() {
  struct udp_socket sock;
  create_udp_socket(&sock, json_config.modules.main_module.target_ip,
                    json_config.modules.main_module.local_ip,
                    json_config.modules.gyroscope_receiver_module.target_port,
                    json_config.modules.gyroscope_receiver_module.local_port);

  while (1) {
    send_udp_message(&sock, "1:2", 10);
    sleep(1);
  }
  return 0;
}

int main() {
  set_log_level(DEBUG);
  init_json_config(JSON_CONFIG_FILE /*maybe create another json_config?*/);
  gyroscope_receiver_logic();
  return 0;
}