#include <f3d/engine/core/memory/memory.h>
#include <f3d/engine/type/buffer.h>
#include <f3d/engine/core/log.h>

#include <stdlib.h>
#include <stdbool.h>

#define AR_MEMORY_DEBUG

#define AR_MEMORY_STARTING_ALLOCS 256

static ar_buffer_t memory_allocs;
static bool memory_direct = true;

void *ar_memory_alloc(size_t size) {
    void *ptr = NULL;
#ifdef AR_MEMORY_DEBUG
    if (!memory_allocs.initialized && !memory_direct) {
        ar_buffer_init(&memory_allocs, AR_BUFFER_DYNAMIC, sizeof(ar_memory_alloc_t), AR_MEMORY_STARTING_ALLOCS);
    }
    
    ptr = malloc(size);
    
    if (!memory_direct) {
        ar_memory_alloc_t alloc;
        alloc.ptr = ptr;
        alloc.size = size;
        ar_buffer_push(&memory_allocs, &alloc);
    }
    return ptr;
#else
    ptr = malloc(size);
#endif
    return ptr;
}

void ar_memory_set_option(ar_memory_option_t option, int value) {
    switch (option) {
        case AR_MEMORY_DIRECT:
            memory_direct = value;
            break;
    }
}

bool ar_memory_is_allocated(ar_memory_alloc_t *alloc) {
    if (alloc->ptr != NULL && alloc->size > 0)
        return true;
    return false;
}

#ifdef AR_MEMORY_DEBUG
static ar_memory_alloc_t *find_alloc(void *ptr) {
    unsigned i;
    ar_memory_alloc_t *alloc;
    for (i = 0; i < memory_allocs.index; i++) {
        alloc = ar_buffer_get(&memory_allocs, i);
        if (alloc->ptr == ptr)
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
}

void *ar_memory_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        ar_log(AR_LOG_ERROR, "Memory pointer == NULL\n", 0);
        return NULL;
    }
    
    void *newptr = NULL;
#ifdef AR_MEMORY_DEBUG
    ar_memory_alloc_t *alloc = find_alloc(ptr);
    newptr = realloc(ptr, size);
    if (alloc != NULL) {
        alloc->size = size;
        alloc->ptr = newptr;
    }
#else
    newptr = realloc(ptr, size);
#endif
    return newptr;
}

void ar_memory_free(void *ptr) {
    if (ptr == NULL) {
        ar_log(AR_LOG_ERROR, "Memory pointer == NULL\n", 0);
        return;
    }
    
#ifdef AR_MEMORY_DEBUG
    ar_memory_alloc_t *alloc = find_alloc(ptr);
    if (alloc != NULL && alloc->ptr != NULL) {
        alloc->size = 0;
        free(alloc->ptr);
        alloc->ptr = NULL;
    }
#else
    free(ptr);
#endif
}

