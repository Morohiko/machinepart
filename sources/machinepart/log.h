#ifndef HM_LOG_H
#define HM_LOG_H

#include <stdio.h>
#include <string.h>

#define LOG_CRIT 1u
#define LOG_ERROR 2u
#define LOG_INFO 3u
#define LOG_DEBUG 4u
#define LOG_TRACE 5u

#ifndef CURR_LOG_LEVEL
#define CURR_LOG_LEVEL LOG_INFO
#endif // CURR_LOG_LEVEL

#define hm_log_module_simple(level_str, ...) \
    printf("%s: ", level_str);               \
    printf(__VA_ARGS__);

#define hm_log_module_expanded(level_str, ...)                               \
    printf("%s:%d [%s]: ",                                                   \
           (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__), \
           __LINE__, level_str);                                             \
    printf(__VA_ARGS__);

#define hm_log_module_current(...) hm_log_module_simple(__VA_ARGS__)

#if (CURR_LOG_LEVEL >= LOG_CRIT)
#define hm_log_crit(...)                            \
    do {                                            \
        hm_log_module_current("CRIT", __VA_ARGS__); \
        exit(0);                                    \
    } while (0)
#else
#define hm_log_crit(...) \
    do {                 \
    } while (0)
#endif

#if (CURR_LOG_LEVEL >= LOG_ERROR)
#define hm_log_error(...)                            \
    do {                                             \
        hm_log_module_current("ERROR", __VA_ARGS__); \
    } while (0)
#else
#define hm_log_error(...) \
    do {                  \
    } while (0)
#endif

#if (CURR_LOG_LEVEL >= LOG_ERROR)
#define hm_log_error(...)                            \
    do {                                             \
        hm_log_module_current("ERROR", __VA_ARGS__); \
    } while (0)
#else
#define hm_log_error(...) \
    do {                  \
    } while (0)
#endif

#if (CURR_LOG_LEVEL >= LOG_INFO)
#define hm_log_info(...)                            \
    do {                                            \
        hm_log_module_current("INFO", __VA_ARGS__); \
    } while (0)
#else
#define hm_log_info(...) \
    do {                 \
    } while (0)
#endif

#if (CURR_LOG_LEVEL >= LOG_DEBUG)
#define hm_log_debug(...)                            \
    do {                                             \
        hm_log_module_current("DEBUG", __VA_ARGS__); \
    } while (0)
#else
#define hm_log_debug(...) \
    do {                  \
    } while (0)
#endif

#if (CURR_LOG_LEVEL >= LOG_TRACE)
#define hm_log_trace(...)                            \
    do {                                             \
        hm_log_module_current("TRACE", __VA_ARGS__); \
    } while (0)
#else
#define hm_log_trace(...) \
    do {                  \
    } while (0)
#endif

#endif // HM_LOG_H
