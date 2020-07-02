#include <acheron/engine/core/cr_macros.h>

#ifdef AR_OS_POSIX

#ifndef AR_CR_THREADS_PTHREAD_H
#define AR_CR_THREADS_PTHREAD_H

#include <pthread.h>

typedef struct {
    pthread_t pthread;
} ar_thread_intern_t;

int  ar_thread_intern_new(ar_thread_intern_t *thread, void *(*thread_func)(void *), void *arg);
int  ar_thread_intern_join(ar_thread_intern_t *thread, void **retval);
void ar_thread_intern_exit(void *arg);

#endif
#endif
