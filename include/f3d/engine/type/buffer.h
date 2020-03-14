#ifndef BUFFER_H
#define BUFFER_H

typedef struct {
    void *data;
    // object size in bytes
    unsigned obj_sz;
    unsigned index;
    // allocated size in objects
    unsigned size;
} buffer_t;

int  buffer_init(buffer_t *buffer, unsigned obj_sz, unsigned start_size);
void buffer_push(buffer_t *buffer, void *obj);
void buffer_resize(buffer_t *buffer);
void buffer_destroy(buffer_t *buffer);

#endif
