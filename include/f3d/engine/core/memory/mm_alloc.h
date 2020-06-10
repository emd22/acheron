#ifndef AR_MM_ALLOC_H
#define AR_MM_ALLOC_H

#include <stddef.h>

typedef struct {
    size_t size;
    void *ptr;
} ar_memory_alloc_t;

void *ar_memory_alloc(size_t size);
void *ar_memory_realloc(void *ptr, size_t size);
void  ar_memory_free(void *ptr);
void  ar_memory_cleanup(void);
size_t ar_memory_used(void);
ar_memory_alloc_t *ar_memory_get_alloc(void *ptr);

#endif
