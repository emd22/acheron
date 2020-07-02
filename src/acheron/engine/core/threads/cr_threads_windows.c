#include <acheron/engine/core/cr_macros.h>

#include <acheron/engine/core/threads/cr_threads_windows.h>

typedef int dummy_typedef_for_iso_compilers_t;

#ifdef AR_OS_WINDOWS

typedef struct {
    ar_thread_intern_t *this_thread;
    void *arg;
} ar_threads_windows_arg_t;

DWORD WINAPI ar_thread_windows_function(LPVOID arg) {
    ar_threads_windows_arg_t *thread_arg = (ar_threads_windows_arg_t *)arg;
    return thread_arg->this_thread->thread_func()
}

int ar_thread_intern_new(ar_thread_intern_t *thread, void *(*thread_func)(void *), void *arg) {
    thread->thread_handle = CreateThread(NULL, 0, &ar_thread_windows_function, NULL, 0, NULL);
    // Windows thread error
    return !thread->thread_handle;
}

void ar_thread_intern_join(ar_thread_intern_t *thread, void **retval) {
    (void)thread;
    (void)retval;
}



#endif
