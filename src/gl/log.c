#include <gl/log.h>

#include <stdarg.h>
#include <stdio.h>

void _log_msg(const char *function, int errlvl, const char *str, ...) {
    #ifndef DEBUG
    if (errlvl == LOG_INFO || errlvl == LOG_WARN) {
        (void)function;
        (void)str;
        return;
    }
    #endif
    const char *levels[] = {
        "INFO", "WARN", "ERROR", "FATAL"
    };
    printf("[%s] %s: ", levels[errlvl], function);
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}
