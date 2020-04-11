#include <f3d/engine/type/buffer.h>
#include <f3d/engine/log.h>

#include <stdlib.h>
#include <string.h>


// NOTE: buffers resize dynamically, resizing in increments of buffer_size*2.
// a buffer shouldn't be used all the time, but can be useful when it is unsure of
// what the final size will be.

// TODO: add custom memory_alloc/free functions to count the total memory used in the engine
// and detect if there are any leaks. This number should only be used for debugging.
unsigned long long buffer_total_used = 0;

int buffer_init(buffer_t *buffer, unsigned obj_sz, unsigned start_size) {
    buffer->index = 0;
    buffer->obj_sz = obj_sz;
    buffer->size = start_size;
    
    unsigned long bytes_sz = (unsigned long)obj_sz*(unsigned long)start_size;
    buffer->data = malloc(bytes_sz);
    
    if (buffer->data == NULL) {
        log_msg(
                LOG_ERROR, 
                "Error allocating buffer of size %u*%u(%lu)\n", 
                obj_sz, start_size, bytes_sz
        );
        // TODO: F3D_FAILURE and F3D_SUCCESS return codes
        return 1;
    }
    buffer_total_used += bytes_sz;
    return 0;
}

void buffer_push(buffer_t *buffer, void *obj) {
    if (buffer->index >= buffer->size) {
        buffer_resize(buffer);
    }
    unsigned long index = (buffer->index)*(buffer->obj_sz);
    memcpy(((unsigned char *)buffer->data)+(index), obj, buffer->obj_sz);
    buffer->index++;
}

void buffer_resize(buffer_t *buffer) {
    // resizing a destroyed buffer or a error with buffer creation
    if (buffer->data == NULL) {
        log_msg(LOG_ERROR, "buffer->data == NULL\n", 0);
        return;
    }
    
    // if there is a large difference in buffer->index and buffer->size (there shouldn't be)
    // we set the size to the index to avoid coming up too short on the resize.
    if (buffer->index > buffer->size)
        buffer->size = buffer->index;

    // debug message
    log_msg(LOG_INFO, "Resizing buffer from %luKB to %luKB\n", buffer->size/1024, buffer->size*2/1024);
    buffer_total_used -= buffer->size*buffer->obj_sz;
    buffer->size *= 2;
    
    buffer->data = realloc(buffer->data, buffer->size*buffer->obj_sz);

    // error in realloc, probably ran out of memory
    if (buffer->data == NULL) {
        log_msg(LOG_ERROR, "Error resizing buffer (size:%lu, obj_sz: %lu)\n", buffer->size, buffer->obj_sz);
    }
    
    buffer_total_used += buffer->size*buffer->obj_sz;
}

void buffer_destroy(buffer_t *buffer) {
    if (buffer->data == NULL)
        return;
    buffer_total_used -= buffer->size*buffer->obj_sz;
    free(buffer->data);
    buffer->data = NULL;
}
