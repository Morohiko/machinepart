#include "log.h"

int main() {
  set_log_level(INFO);

  print(INFO, "hello world");
  return 0;
}
