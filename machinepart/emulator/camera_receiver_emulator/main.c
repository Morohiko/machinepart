#include "log.h"

int main() {
    set_log_level(DEBUG);
    print(INFO, "from camera receiver emulator");
    return 0;
}