#include <assert.h>
#include <stdlib.h>

#include <fps_checker.h>
#include <log.h>

fps_checker_t *init_fps_checker(fps_checker_t *fps_checker_config)
{
    fps_checker_t *fps_checker = (fps_checker_t *)malloc(sizeof(fps_checker));
    if (!fps_checker) {
        hm_log_error("fps_checker: can`t allocate memory for module\n");
        return NULL;
    }
    fps_checker->last_saved_time = time(NULL);
    fps_checker->fps_value = 0;
    fps_checker->time_to_update = fps_checker_config->time_to_update;

    if (!fps_checker->last_saved_time) {
        hm_log_error("fps_checker: cannot get current time for fps checker\n");
        free(fps_checker);
        return NULL;
    }
    return fps_checker;
}

int destroy_fps_checker(fps_checker_t *fps_checker)
{
    if (!fps_checker) {
        hm_log_error("fps_checker: can`t destroy fps_checker, null\n");
        return -1;
    }
    free(fps_checker);
    return 0;
}

int update_fps_value(fps_checker_t *fps_checker, char *msg)
{
    assert(fps_checker);
    assert(msg);

    if (!fps_checker->last_saved_time) {
        hm_log_error("fps_checker: cannot get current time for fps checker\n");
        return -1;
    }

    time_t current_time = time(NULL);

    fps_checker->diff_time +=
            difftime(current_time, fps_checker->last_saved_time);

    if (fps_checker->diff_time < fps_checker->time_to_update) {
        fps_checker->fps_value++;
    } else {
        hm_log_info("fps_checker: fps = %d, time to update %lf, msg: %s\n",
                    fps_checker->fps_value, fps_checker->time_to_update, msg);
        fps_checker->diff_time = 0;
        fps_checker->fps_value = 0;
    }

    fps_checker->last_saved_time = current_time;

    return 0;
}
