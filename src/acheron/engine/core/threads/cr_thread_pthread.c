#include <acheron/engine/core/cr_macros.h>

#ifdef AR_OS_POSIX

#include <unistd.h>
#include <acheron/engine/core/threads/cr_thread_pthread.h>

#include <sys/time.h>
#include <sys/select.h>

int ar_thread_intern_init(ar_thread_intern_t *thread, void *(*thread_func)(void *), void *arg) {
    int res;
    res = pthread_create(&thread->pthread, NULL, thread_func, arg);
    return res;
}

void ar_thread_intern_sleep(unsigned msec) {
    struct timeval tv;
    tv.tv_sec = msec/1000;
    tv.tv_usec = msec % 1000;
    select(0, NULL, NULL, NULL, &tv);
}

int ar_thread_intern_join(ar_thread_intern_t *thread, void **retval) {
    return pthread_join(thread->pthread, retval);
}

void ar_thread_intern_exit(void *arg) {
    pthread_exit(arg);
}

#endif
