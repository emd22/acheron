#ifndef AR_CR_DEBUG_H
#define AR_CR_DEBUG_H

#include <acheron/engine/core/cr_log.h>

#define ar_assert(sev, cond) _ar_assert(sev, (cond), __func__, #cond, __LINE__)

void _ar_assert(ar_log_error_level_t err_sev, int cond, const char *calling_func, const char *assertstr, int calling_line);
void ar_debug_print_backtrace(void);

#endif
