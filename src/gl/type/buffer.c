#include <gl/type/buffer.h>
#include <gl/log.h>

#define EXPAND_TYPE(buffer) (buffer->flags >> 14)

int buffer_init(buffer_t *buffer, unsigned obj_sz, unsigned start_size) {
    buffer->used = 0;
    buffer->obj_sz = obj_sz;
    buffer->start_size = start_size;
    
    EXPAND_TYPE(buffer) = BUFFER_ABSOLUTE;
    buffer->data = malloc(obj_sz*start_size);
    
    if (buffer->data == NULL) {
        log_msg(
                LOG_ERROR, 
                "Error allocating buffer of size %u*%u(%lu)", 
                obj_sz, start_size, 
                (unsigned long)((unsigned long)obj_sz*(unsigned long)start_size)
        );
        return 1;
    }
        
    return 0;
}

void buffer_set_option(buffer_t *buffer, int option, int value) {
    if (option == BUFFER_OPTION_EXPAND) {
        EXPAND_TYPE(buffer) = value;
    }
}

void buffer_resize(buffer_t *buffer, int size) {
    if (buffer->data == NULL)
        return;
        
    switch (EXPAND_TYPE(buffer)) {
        case BUFFER_ABSOLUTE:
            buffer->data = realloc(buffer->data, buffer->obj_sz*size);
            buffer->size = buffer->obj_sz*size;
            break;
        case BUFFER_DOUBLE:
            buffer->data = realloc(buffer->data, buffer->size*2);
            buffer->size *= 2;
            break;
        case BUFFER_ADD:
            buffer->data = realloc(buffer->data, buffer->size+size);
            buffer->size += size;
            break;
        default:
            break;
            
    }
}

void buffer_destroy(buffer_t *buffer) {
    free(buffer->data);
    buffer->index = 0;
    buffer->size = 0;
    buffer->data = NULL;
}
