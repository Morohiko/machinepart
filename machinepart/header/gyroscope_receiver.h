#ifndef GYROSCOPE_RECEIVER_H
#define GYROSCOPE_RECEIVER_H

struct gyroscope_ctx {
    int x;
    int y;
    int z;
};

int recv_gyroscope_data(struct gyroscope_ctx *ctx);

#endif

