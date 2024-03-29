#include <acheron/engine/acheron.h>
#include <acheron/engine/core/cr_debug.h>
#include <acheron/engine/core/cr_log.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
#define __USE_GNU
#include <execinfo.h>
#include <dlfcn.h>
#ifndef _DLFCN_H
#define BACKTRACE_NO_FUNCTION_NAMES
#endif
#endif

#define BACKTRACE_SIZE 32

char *ar_debug_get_location(char *str) {
    ar_assert(AR_LOG_FATAL, (str != NULL));
    char ch;
    while ((ch = *(str++)) != '(');
    return str-1;
}

void _ar_assert(ar_log_error_level_t err_sev, int cond, const char *calling_func, const char *assertstr, int calling_line) {
    if (cond) {
        ar_log(err_sev, "Assertion '%s' failed in %s at line %d\n", assertstr, calling_func, calling_line);
        if (err_sev == AR_LOG_FATAL) {
            ar_exit(NULL);
        }
    }
}

void ar_debug_print_backtrace(void) {
#ifdef __linux__
    void *trace[BACKTRACE_SIZE];
    int trace_size;
    trace_size = backtrace(trace, BACKTRACE_SIZE);
    char **symbols;
    symbols = backtrace_symbols(trace, trace_size);
#ifndef BACKTRACE_NO_FUNCTION_NAMES
    char *end;
    int size;
    ar_log(AR_LOG_DEBUG, "Retrieving function names...\n", 0);
    Dl_info bt_info;
#endif
    ar_log(AR_LOG_DEBUG, "Printing Backtrace\n", 0);
    int i;
    for (i = 0; i < trace_size; i++) {
    #ifndef BACKTRACE_NO_FUNCTION_NAMES
        end = ar_debug_get_location(symbols[i]);
        size = end-symbols[i];
        
        dladdr(trace[i], &bt_info);
        // print function pointer, function name, and location
        printf("|%-14p || %-24s || %-16.*s|\n", trace[i], bt_info.dli_sname, size, symbols[i]);
    #else
        // we dont have access to the dl library, so just print location+address
        printf("%s\n", symbols[i]);
    #endif
    }
    ar_log(AR_LOG_DEBUG, "Backtrace end\n", 0);
    free(symbols);
#endif //__linux__
}
