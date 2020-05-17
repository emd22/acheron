#ifndef F3D_BUFFER_H
#define F3D_BUFFER_H

#define BUFFER_RESIZE_AUTO -1

typedef enum {
    BUFFER_STATIC,
    BUFFER_DYNAMIC,
} buffer_type_t;

typedef struct {
    buffer_type_t type;

    void *data;
    // object size in bytes
    unsigned obj_sz;
    unsigned index;
    // allocated size in objects
    unsigned size;
} buffer_t;

extern unsigned long long buffer_total_used;

int  buffer_init(buffer_t *buffer, buffer_type_t type, unsigned obj_sz, unsigned start_size);
void buffer_push(buffer_t *buffer, void *obj);
buffer_t buffer_duplicate(buffer_t *buffer, buffer_type_t type);
buffer_t buffer_from_data(buffer_type_t type, void *data, unsigned obj_sz, unsigned data_size);
void buffer_copy_data(buffer_t *buffer, void *data, int data_size);
void buffer_resize(buffer_t *buffer, int set);
void buffer_destroy(buffer_t *buffer);

#endif
