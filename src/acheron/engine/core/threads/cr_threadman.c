#include <acheron/engine/core/threads/cr_threadman.h>
#include <acheron/engine/core/threads/cr_thread.h>
#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/cr_log.h>

#include <acheron/engine/limits.h>

typedef struct {
    ar_thread_intern_t *thread;
    void *arg;
} ar_thread_arg_t;

static ar_buffer_t thread_buffer;

#if 0
static void *thread_update(void *ptr) {
    ar_thread_arg_t *this_thread = (ar_thread_arg_t *)ptr;
    //this_thread->thread->update_func(this_thread->thread, this_thread->arg);
}
#endif

void ar_threadman_init(void) {
    ar_buffer_init(&thread_buffer, AR_BUFFER_STATIC, sizeof(ar_thread_t), AR_MAX_THREADS, 0);
    int i;
    ar_thread_t *thread;
    for (i = 0; i < AR_MAX_THREADS; i++) {
        thread = ar_buffer_new_item(&thread_buffer);
        thread->status = AR_THREAD_STOPPED;
    }
}

static ar_thread_t *find_free_thread(void) {
    int i;
    ar_thread_t *thread;
    for (i = 0; i < AR_MAX_THREADS; i++) {
        thread = ar_buffer_get(&thread_buffer, i);
        if (thread->status == AR_THREAD_STOPPED) {
            return thread;
        }
    }
    ar_log(AR_LOG_ERROR, "Could not find free thread\n", 0); 
    return NULL;
}

ar_thread_t *ar_thread_new(ar_thread_update_func_t update_func, void *arg) {    
    ar_thread_t *thread = find_free_thread();
    ar_thread_init(thread, update_func, arg);
    return thread;
}

void ar_thread_destroy(ar_thread_t *thread) {
    thread->status = AR_THREAD_STOPPING;
}

void ar_threadman_destroy(void) {
    ar_buffer_destroy(&thread_buffer);
}
