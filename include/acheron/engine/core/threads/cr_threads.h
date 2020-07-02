#ifndef AR_CR_THREADS_H
#define AR_CR_THREADS_H

#include <acheron/engine/core/cr_macros.h>

#if defined(AR_OS_POSIX)
#include <acheron/engine/core/threads/cr_threads_pthread.h>
#elif defined(AR_OS_WINDOWS)
#include <acheron/engine/core/threads/cr_threads_windows.h>
#endif

typedef struct {
    ar_thread_intern_t intern_thread;
} ar_thread_t;

ar_thread_t ar_thread_new(void *(*thread_func)(void *), void *arg);
int ar_thread_join(ar_thread_t *thread, void **retval);
void ar_thread_exit(void *arg);

#endif
