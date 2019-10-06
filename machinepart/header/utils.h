#ifndef UTILS_H
#define UTILS_H

#define CLEAR(x) memset(&(x), 0, sizeof(x))

// get ip addr from /proc/net/arp
int get_target_ip_addr(char *dest);

#endif // UTILS_H
