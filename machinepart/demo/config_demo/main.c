#include "log.h"
#include "json_config.h"

#define JSON_FILEPATH "/home/user/work/headmachine/machinepart/machinepart/demo/config_demo/config.json"

int main() {
    set_log_level(DEBUG);
    print(INFO, "from config demo");
    init_json_config(JSON_FILEPATH);
    return 0;
}