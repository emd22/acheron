#ifndef AR_CR_THREAD_H
#define AR_CR_THREAD_H

#include <acheron/engine/core/cr_macros.h>

#if defined(AR_OS_POSIX)
#include <acheron/engine/core/threads/cr_thread_pthread.h>
#elif defined(AR_OS_WINDOWS)
#include <acheron/engine/core/threads/cr_thread_windows.h>
#endif

typedef void *(*ar_thread_update_func_t)(ar_thread_t *, void *);

typedef enum {
    AR_THREAD_STOPPED,
    AR_THREAD_STOPPING,
    AR_THREAD_RUNNING,
} ar_thread_status_t;

typedef struct ar_thread_s {
    ar_thread_status_t status;
    ar_thread_intern_t intern_thread;
    ar_thread_update_func_t update_func;
} ar_thread_t;

void ar_thread_init(ar_thread_t *thread, ar_thread_update_func_t update_func, void *arg);
void ar_thread_sleep(unsigned msec);
int ar_thread_join(ar_thread_t *thread, void **retval);
void ar_thread_exit(void *arg);

#endif
