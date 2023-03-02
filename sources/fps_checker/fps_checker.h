#ifndef HM_FPS_CHECKER_H
#define HM_FPS_CHECKER_H

#include <time.h>

#include <json_config.h>

typedef struct {
    time_t last_saved_time;
    double diff_time;
    int fps_value;
    double time_to_update;
} fps_checker_t;

fps_checker_t *init_fps_checker(fps_checker_t *fps_checker_config);

int destroy_fps_checker(fps_checker_t *fps_checker);

int update_fps_value(fps_checker_t *fps_checker, char *msg);

#endif // HM_FPS_CHECKER_H
