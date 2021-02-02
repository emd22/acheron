#ifndef AR_CR_LOG_H
#define AR_CR_LOG_H

#include <stdarg.h>


// TODO: remove log_msg
#define log_msg(errlvl, str, ...) _ar_log(__func__, errlvl, str, ##__VA_ARGS__)

#define ar_log(errlvl, str, ...) _ar_log(__func__, errlvl, str, ##__VA_ARGS__)

typedef enum {
    AR_LOG_INFO,
    AR_LOG_WARN,
    AR_LOG_ERROR,
    AR_LOG_FATAL,
    AR_LOG_DEBUG,

    AR_LOG_RENDER_INFO,
    AR_LOG_RENDER_WARN,
    AR_LOG_RENDER_ERROR,
} ar_log_error_level_t;

void _ar_log(const char *function, ar_log_error_level_t level, const char *str, ...);

#endif
