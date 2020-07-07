#ifndef AR_CR_DEBUG_H
#define AR_CR_DEBUG_H

#define ar_assert(cond) _ar_assert((cond), __func__, #cond, __LINE__)

void _ar_assert(int cond, const char *calling_func, const char *assertstr, int calling_line);
void ar_debug_print_backtrace(void);

#endif
