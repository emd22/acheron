#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

#define log_msg(errlvl, str, ...) _log_msg(__func__, errlvl, str, ##__VA_ARGS__)

enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
};

void _log_msg(const char *function, int errlvl, const char *str, ...);

#endif
