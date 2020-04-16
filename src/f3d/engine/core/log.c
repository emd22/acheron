#include <f3d/engine/core/log.h>

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
    // https://en.wikipedia.org/wiki/ANSI_escape_code
    const char *levels[] = {
        "\033[34mINFO\033[m",
        "\033[33mWARN\033[m",
        "\033[91mERROR\033[m",
        "\033[91mFATAL\033[m",
        "\033[32mDEBUG\033[m"
    };
    printf("[%s] %s: ", levels[errlvl], function);
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}
