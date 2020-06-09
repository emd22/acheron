#ifndef AR_CORE_MEMORY_H
#define AR_CORE_MEMORY_H

#include <stddef.h>

typedef enum {
    AR_MEMORY_DIRECT,
} ar_memory_option_t;

typedef struct {
    size_t size;
    void *ptr;
} ar_memory_alloc_t;

void *ar_memory_alloc(size_t size);
void *ar_memory_realloc(void *ptr, size_t size);
void  ar_memory_free(void *ptr);
size_t ar_memory_used(void);
void  ar_memory_set_option(ar_memory_option_t option, int value);
ar_memory_alloc_t *ar_memory_get_alloc(void *ptr);

#endif
