#ifndef FPS_CHECKER_H
#define FPS_CHECKER_H

#include "time.h"

struct fps_checker_t {
  time_t last_saved_time;
  double diff_time;
  int fps_value;
};

int init_fps_checker(struct fps_checker_t *fps_checker);

int destroy_fps_checker();

int update_fps_value(struct fps_checker_t *fps_checker, char *msg);

#endif // FPS_CHECKER_H
