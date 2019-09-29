#include "fps_checker.h"
#include "config.h"
#include "log.h"
#include "assert.h"

int init_fps_checker(struct fps_checker_t *fps_checker) {
    assert(fps_checker);

    fps_checker->last_saved_time = time(NULL);
    fps_checker->fps_value = 0;

    if (!fps_checker->last_saved_time) {
        print(ERROR, "cannot get current time for fps checker");
        return -1;
    }
    return 0;
}

int destroy_fps_checker() {
}

int update_fps_value(struct fps_checker_t *fps_checker, char* msg) {
    assert(fps_checker);
    assert(msg);

    if (!fps_checker->last_saved_time) {
        print(ERROR, "cannot get current time for fps checker");
        return -1;
    }

    time_t current_time = time(NULL);

    fps_checker->diff_time += difftime(current_time, fps_checker->last_saved_time);

    if (fps_checker->diff_time < FPS_UPDATE_VALUE_EVERY) {
        fps_checker->fps_value++;
    } else {
        print(INFO, "fps = %d, msg: %s", (fps_checker->fps_value/FPS_UPDATE_VALUE_EVERY), msg);
        fps_checker->diff_time = 0;
        fps_checker->fps_value = 0;
    }

    fps_checker->last_saved_time = current_time;

    return 0;
}
