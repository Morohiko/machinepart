#include "log.h"
#include "utils.h"

int main() {
  set_log_level(DEBUG);

  char test_src[100];

  get_gp_ip_addr(test_src);

  print(DEBUG, "founded ip addr: %s", test_src);

  return 0;
}
