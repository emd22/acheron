#include <f3d/engine/core/threads/cr_threads.h>

ar_thread_t ar_thread_new(void *(*thread_func)(void *), void *arg) {
    ar_thread_t thread;
    ar_thread_intern_new(&thread.intern_thread, thread_func, arg);
    return thread;
}

int ar_thread_join(ar_thread_t *thread, void **retval) {
    return ar_thread_intern_join(&thread->intern_thread, retval);
}

void ar_thread_exit(void *arg) {
    ar_thread_intern_exit(arg);
}
