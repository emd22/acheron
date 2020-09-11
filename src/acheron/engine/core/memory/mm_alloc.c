#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/cr_log.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STARTING_ALLOCS 256

static ar_buffer_t memory_allocs;

void *_ar_memory_alloc(const char *func, size_t size) {
    (void)func;

    void *ptr = NULL;
    ptr = malloc(size);
#ifdef AR_MEMORY_DEBUG
    if (!ar_buffer_is_initialized(&memory_allocs)) {
        ar_log(AR_LOG_DEBUG, "*** Initializing Memory Debug Buffer ***\n", 0);
        ar_buffer_init(&memory_allocs, AR_BUFFER_DYNAMIC, sizeof(ar_memory_alloc_t), STARTING_ALLOCS, AR_BUFFER_UNTRACKED);
    }
    
    ar_memory_alloc_t *alloc = ar_buffer_new_item(&memory_allocs);
    alloc->ptr = ptr;
    alloc->size = size;
    strcpy(alloc->calling_func, func);

    //ar_memory_alloc_t alloc;
    //alloc.ptr = ptr;
    //alloc.size = size;
    //ar_buffer_push(&memory_allocs, &alloc);
#endif
    return ptr;
}

bool ar_memory_is_allocated(ar_memory_alloc_t *alloc) {
    if (alloc != NULL && alloc->ptr != NULL)
        return true;
    return false;
}

#ifdef AR_MEMORY_DEBUG
static ar_memory_alloc_t *find_alloc(void *ptr) {
    unsigned i;
    ar_memory_alloc_t *alloc;
    for (i = 0; i < memory_allocs.index; i++) {
        alloc = ar_buffer_get(&memory_allocs, i);
        if (alloc != NULL && alloc->ptr == ptr)
            return alloc;
    }
    return NULL;
}
#endif

ar_memory_alloc_t *ar_memory_get_alloc(void *ptr) {
    (void)ptr;
#ifdef AR_MEMORY_DEBUG
    return find_alloc(ptr);
#endif
    return NULL;
}

size_t ar_memory_used(void) {
    (void)memory_allocs;
#ifdef AR_MEMORY_DEBUG
    size_t used = 0;
    unsigned i;
    ar_memory_alloc_t *alloc;
    for (i = 0; i < memory_allocs.index; i++) {
        alloc = ar_buffer_get(&memory_allocs, i);
        if (ar_memory_is_allocated(alloc)) {
            used += alloc->size;
        }
    }
    return used;
#else
    return 0;
#endif
}

void *ar_memory_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        ar_log(AR_LOG_ERROR, "Memory pointer == NULL\n", 0);
        return NULL;
    }
    
    void *newptr = NULL;
    newptr = realloc(ptr, size);
#ifdef AR_MEMORY_DEBUG
    ar_memory_alloc_t *alloc = find_alloc(ptr);
    if (alloc != NULL) {
        alloc->size = size;
        alloc->ptr = newptr;
    }
    else {
        ar_log(AR_LOG_ERROR, "Unknown allocation %p\n", ptr);
    }
#endif
    return newptr;
}

void ar_memory_cleanup(void) {
#ifdef AR_MEMORY_DEBUG
    unsigned i;
    unsigned leaks = 0;
    double leaked_kb = 0;
    ar_memory_alloc_t *alloc;
    for (i = 0; i < memory_allocs.index; i++) {
        alloc = ar_buffer_get(&memory_allocs, i);
        if (ar_memory_is_allocated(alloc)) {
            ar_log(AR_LOG_DEBUG, "%.02fKB of leaked memory from %s!\n", (float)alloc->size/1024.0f, alloc->calling_func);
            leaks++;
            leaked_kb += (double)alloc->size/1024.0f;
            ar_memory_free(alloc->ptr);
        }
    }
    ar_log(AR_LOG_DEBUG, "Mem Info:\n\t* Leaked blocks: %u\n\t* Leaked memory: %.2f KiB\n", leaks, leaked_kb);
    ar_buffer_destroy(&memory_allocs);
#endif
}

void ar_memory_free(void *ptr) {    
#ifdef AR_MEMORY_DEBUG
    ar_memory_alloc_t *alloc = find_alloc(ptr);
    if (alloc != NULL && alloc->ptr != NULL) {
        alloc->size = 0;
        free(alloc->ptr);
        alloc->ptr = NULL;
    }
    return;
#endif
    free(ptr);
}

