#ifndef F3D_BUFFER_H
#define F3D_BUFFER_H

#include <stdbool.h>
#include <stddef.h>

#define AR_BUFFER_RESIZE_AUTO -1

typedef enum {
    AR_BUFFER_STATIC,
    AR_BUFFER_DYNAMIC,
} ar_buffer_type_t;

typedef struct ar_buffer_s {
    bool initialized;

    ar_buffer_type_t type;

    void *data;
    // object size in bytes
    unsigned obj_sz;
    unsigned index;
    // allocated size in objects
    unsigned size;
    
    size_t (*resize_func)(struct ar_buffer_s *);
} ar_buffer_t;

extern size_t ar_buffer_total_used;

int  ar_buffer_init(ar_buffer_t *buffer, ar_buffer_type_t type, unsigned obj_sz, unsigned start_size);
void *ar_buffer_push(ar_buffer_t *buffer, void *obj);
void *ar_buffer_get(ar_buffer_t *buffer, unsigned index);
ar_buffer_t ar_buffer_duplicate(ar_buffer_t *buffer, ar_buffer_type_t type);
ar_buffer_t ar_buffer_from_data(ar_buffer_type_t type, void *data, unsigned obj_sz, unsigned data_size);
void ar_buffer_copy_data(ar_buffer_t *buffer, void *data, int data_size);
void ar_buffer_resize(ar_buffer_t *buffer, int size);
void ar_buffer_destroy(ar_buffer_t *buffer);

size_t ar_buffer_resize_func_double(ar_buffer_t *buffer);

#endif
