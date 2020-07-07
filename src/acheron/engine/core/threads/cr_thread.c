#include <acheron/engine/core/threads/cr_thread.h>

void ar_thread_init(ar_thread_t *thread, ar_thread_update_func_t update_func, void *arg) {
    thread->status = AR_THREAD_RUNNING;
    ar_thread_intern_init(&thread->intern_thread, update_func, arg);
}

void ar_thread_sleep(unsigned msec) {
    ar_thread_intern_sleep(msec);
}

int ar_thread_join(ar_thread_t *thread, void **retval) {
    return ar_thread_intern_join(&thread->intern_thread, retval);
}

void ar_thread_exit(void *arg) {
    ar_thread_intern_exit(arg);
}
