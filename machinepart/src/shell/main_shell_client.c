#include "signal.h"
#include "unistd.h"

#include "config.h"
#include "json_config.h"
#include "log.h"
#include "shell_client.h"

void sig_handler(int signum) { shell_sig_handler(signum); }

int main() {
  signal(SIGINT, sig_handler);
  signal(SIGQUIT, sig_handler);
  set_log_level(DEBUG);
  init_json_config(JSON_CONFIG_FILE);
  if (json_config.shell.state == 0) {
    print(ERROR, "shell is disabled");
    return 0;
  }
  start_shell_client();
  while (1) {
    sleep(1);
  }
  return 0;
}
