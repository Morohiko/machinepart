#include "stdio.h"

#define print(...) \
        printf("%s:%d: ", __FILE__, __LINE__); \
        printf(__VA_ARGS__);\
        printf("\n")
