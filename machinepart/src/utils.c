#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "log.h"

static int get_file_size(char *filename, size_t *file_size) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        print(ERROR, "cannot open file %s", filename);
        return -1;
    }

    char buff[1];
    int count = 0;

    while (fgets(buff, 2, file) != NULL) {
        count++;
    }

    fclose(file);

    *file_size = count;

    return 0;
}

static int read_from_file(char *filename, char *dest) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        print(ERROR, "cannot open file %s", filename);
        return -1;
    }

    char buff[1];
    int count = 0;

// 2 because unicode?
    while (fgets(buff, 2, file) != NULL) {
        dest[count] = buff[0];
        dest[0] = '\n';
        count++;
    }

    fclose(file);

    return 0;
}

static int split_ips_from_arp_info(char *src, char *dest_ip_heap, int *count_of_ips) {
    assert(src); assert(dest_ip_heap); assert(count_of_ips);

    char *first_symbol;
    int ip_heap_iter = 0;

    bool to_end_line = false;
    bool first_line = true;

    while (*src) {
        if (first_line == true) {
            first_line = false;
            to_end_line = true;
            src++;
            continue;
        }
        if (*src == '\n') {
            to_end_line = false;
            src++;
            first_symbol = src;
            continue;
        }
        if (to_end_line == true) {
            src++;
            continue;
        }
        if (*src == ' ') {
            to_end_line = true;
            (*count_of_ips)++;
            while (first_symbol != src) {
                dest_ip_heap[ip_heap_iter++] = *first_symbol;
                first_symbol++;
            }
            dest_ip_heap[ip_heap_iter++] = '\n';
            continue;
        }
        src++;
    }
    return 0;
}

static int get_once_ip_from_heap(char *src, char *dest, int requested_ip) {
    assert(src); assert(dest); assert(requested_ip);

    int spaces = 1;

    char *first_ip = NULL;
    for (int i = 0; i < strlen(src); i++) {
        if (src[i] == '\n') {
            spaces++;
        }

        if (spaces == requested_ip && first_ip == NULL) {
            first_ip = &src[i];
            continue;
        }

        if (spaces > requested_ip) {
            src[i] = '\0';
            int iter = 0;
            memset(dest, '\0', 16);
            while (first_ip != &src[i]) {
                if (*first_ip != '\n') {
                    dest[iter++] = *first_ip;
                }
                first_ip++;
            }
            return 0;
        }
    }
    return -1;
}

// get ip`s addr from /proc/net/arp
int get_target_ip_addr(char *dest) {
    assert(dest);

    char *filename = "/proc/net/arp";

    size_t file_size = 0;
    get_file_size(filename, &file_size);

    char source[file_size];
    if (read_from_file(filename, source)) {
        print(ERROR, "Cannot get ip addr");
        return -1;
    }

    char ip_heap[file_size];

    int count_of_ips = 0;

    split_ips_from_arp_info(source, ip_heap, &count_of_ips);

// in case empty file
    if (count_of_ips == 0) {
        print(ERROR, "Cannot find ip`s from file %s", filename);
        return -1;
    }

// if single ip return it
    if (count_of_ips == 1) {
        memcpy(dest, ip_heap, (strlen(ip_heap) - 1));
        return 0;
    }

    print(INFO, "need check ip addr from ip_heap, just put number of ip, \"start from 1\", 0 for localhost\n%s", ip_heap);

    int requested_ip;
    scanf("%d", &requested_ip);
    if (requested_ip == 0) {
        memcpy(dest, "127.0.0.1", 10);
        return 0;
    }

    if (get_once_ip_from_heap(ip_heap, dest, requested_ip)) {
         print(ERROR, "cannot found ip");
         return -1;
    }

    return 0;
}
