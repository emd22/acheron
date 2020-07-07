#include <acheron/engine/core/cr_macros.h>

#if 0
#ifdef AR_OS_WINDOWS

#ifndef AR_CR_THREADS_WINDOWS_H
#define AR_CR_THREADS_WINDOWS_H

#include <windows.h>

typedef struct {
    HANDLE thread_handle;
    void *(*thread_func)(void *);
} ar_thread_intern_t;

#endif

#endif
#endif
