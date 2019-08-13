#ifndef  LOG_H
#define LOG_H

#include "stdio.h"

typedef enum {
    ERROR = 0,
    INFO,
    DEBUG,
} log_level;

#define print(log_level, ...) \
        if (log_level <= get_log_level()) { \
            printf("%s:%d: ", __FILE__, __LINE__); \
            print_log_level(log_level); \
            printf(__VA_ARGS__); \
            printf("\n"); \
        }

log_level get_log_level();

void print_log_level(log_level level);

void set_log_level(log_level level);


#endif
