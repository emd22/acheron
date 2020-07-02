#include <acheron/engine/core/cr_log.h>

#include <stdarg.h>
#include <stdio.h>

/*
 * ar_log definition in core/cr_log.h
 */

void _ar_log(const char *function, ar_log_error_level_t level, const char *str, ...) {
    #ifndef AR_DEBUG
    if (level == AR_LOG_INFO || level == AR_LOG_DEBUG) {
        return;
    }
    #endif
    // https://en.wikipedia.org/wiki/ANSI_escape_code
    const char *levels[] = {
        "\033[34mINFO\033[m",
        "\033[33mWARN\033[m",
        "\033[91mERROR\033[m",
        "\033[91mFATAL\033[m",
        "\033[32mDEBUG\033[m"
    };
    printf("[%s] %s: ", levels[level], function);
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}
