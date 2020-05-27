#ifndef F3D_LOG_H
#define F3D_LOG_H

#include <stdarg.h>


// TODO: remove log_msg
#define log_msg(errlvl, str, ...) _sb_log(__func__, errlvl, str, ##__VA_ARGS__)

#define sb_log(errlvl, str, ...) _sb_log(__func__, errlvl, str, ##__VA_ARGS__)

enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_DEBUG,
};


void _sb_log(const char *function, int errlvl, const char *str, ...);

#endif
