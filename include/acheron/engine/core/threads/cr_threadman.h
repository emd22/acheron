#ifndef AR_CR_THREADMAN_H
#define AR_CR_THREADMAN_H

#include <acheron/engine/core/threads/cr_thread.h>

ar_thread_t *ar_thread_new(ar_thread_update_func_t update_func, void *arg);
void ar_thread_destroy(ar_thread_t *thread);

#endif
