#include "log.h"
#include "utils.h"  

int main() {
    char test_src[100];

    get_target_ip_addr(test_src);

    print("DEBUG: founded ip addr: %s", test_src);

    return 0;
}

