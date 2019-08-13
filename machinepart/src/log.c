#include "log.h"

static log_level current_log_level = 0;

log_level get_log_level() {
    return current_log_level;
}

void set_log_level(log_level level) {
    current_log_level = level;
}

void print_log_level(log_level level) {
    switch(level) {
    case ERROR:
        printf("ERROR: ");
        break;
    case INFO:
        printf("INFO: ");
        break;
    case DEBUG:
        printf("DEBUG: ");
        break;
    default:
        printf("NONE: ");
    }
}


