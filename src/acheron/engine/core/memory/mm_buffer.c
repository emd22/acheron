#include <acheron/engine/core/memory/mm_buffer.h>
#include <acheron/engine/core/memory/mm_alloc.h>
#include <acheron/engine/core/cr_log.h>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

enum {
    AR_BUFFER_ITEM_FREE,
    AR_BUFFER_ITEM_USED,
};


// NOTE: buffers can resize dynamically, resizing using the resize_func per buffer object.

// NOTE 2: buffers can now be static, so they hold indexes and sizes while also providing 
// some nice helper functions.

static void *buffer_malloc(ar_buffer_t *buffer, size_t size);
static void *buffer_mrealloc(ar_buffer_t *buffer, size_t new_size);
static void buffer_mfree(ar_buffer_t *buffer);

int ar_buffer_init(ar_buffer_t *buffer, ar_buffer_type_t type, uint16_t obj_sz, uint32_t start_size, uint16_t flags) {
    memset(buffer, 0, sizeof(ar_buffer_t));

    buffer->index = 0;
    buffer->obj_sz = obj_sz;
    buffer->size = start_size;
    buffer->type = type;
    buffer->flags = flags;
    
    size_t size_in_bytes = (size_t)obj_sz*(size_t)start_size;
    
    buffer->data = buffer_malloc(buffer, size_in_bytes);
    // dynamic buffers resize n*2 by default
    buffer->resize_func = &ar_buffer_resize_func_double;

    // to find free items, we allocate a second buffer to store a status value
    if (flags & AR_BUFFER_PACK) {
        buffer->free_map = malloc(start_size);
        memset(buffer->free_map, AR_BUFFER_ITEM_FREE, start_size);
    }
    
    if (buffer->data == NULL) {
        ar_log(
            AR_LOG_ERROR, 
            "Error allocating buffer of size %u*%u(%lu) :: 0x%02X\n", 
            obj_sz, start_size, size_in_bytes, buffer->flags
        );
        return 1;
    }
    buffer->flags |= AR_BUFFER_INITIALIZED;
    return 0;
}

bool ar_buffer_is_initialized(ar_buffer_t *buffer) {
    if (buffer == NULL || (!(buffer->flags & AR_BUFFER_INITIALIZED)) || buffer->data == NULL)
        return false;
    
    return true;   
}

void *ar_buffer_new_item(ar_buffer_t *buffer) {
    if (!ar_buffer_is_initialized(buffer)) {
        ar_log(AR_LOG_ERROR, "Buffer not initialized\n", 0);
        return NULL;
    }

    if (buffer->index >= buffer->size) {
        if (buffer->type != AR_BUFFER_DYNAMIC) {
            ar_log(AR_LOG_ERROR, "Hit end of non-dynamic buffer\n", 0);
            return NULL;
        }
        ar_buffer_resize(buffer, AR_BUFFER_RESIZE_AUTO);
    }

    size_t index = (buffer->index)*(buffer->obj_sz);

    if (buffer->flags & AR_BUFFER_PACK) {
        // linear search for a free slot
        uint32_t i;
        for (i = 0; i < buffer->index; i++) {
            // slot is free, set index
            if (buffer->free_map[i] == AR_BUFFER_ITEM_FREE) {
                // index is item index * size of object
                index = i*buffer->obj_sz;
                // set slot to used
                buffer->free_map[i] = AR_BUFFER_ITEM_USED;
                break;
            }
        }
        // if no slot is free, use next in index
        if (i == buffer->index || i == buffer->index-1) {
            index = (buffer->index)*(buffer->obj_sz);
            buffer->index++;
        }
    }
    // regular linear buffer
    else {
        buffer->index++;
    }

    uint8_t *mem = ((uint8_t *)buffer->data)+(index);
    return mem;
}

void *ar_buffer_push(ar_buffer_t *buffer, void *obj) {
    void *mem = ar_buffer_new_item(buffer);
    memcpy(mem, obj, buffer->obj_sz);
    return mem;
}

long ar_buffer_get_item_index(ar_buffer_t *buffer, void *item) {
    // due to a buffer being one linear chunk of memory, we can get 
    // the index of an item using simple pointer operations.

    char *buf_ptr = buffer->data;
    char *buf_final_ptr = buf_ptr+(buffer->size*buffer->obj_sz);
    char *itemp = (char *)item;

    // check if pointer in buffer
    if (itemp >= buf_ptr && itemp <= buf_final_ptr) {
        return (itemp-buf_ptr)/buffer->obj_sz;
    }
    return -1;
}

void ar_buffer_item_free_at_index(ar_buffer_t *buffer, long index) {
    if (index > buffer->size)
        return;
    if (index == buffer->index-1)
        buffer->index--;
    buffer->free_map[index] = AR_BUFFER_ITEM_FREE;
}

void ar_buffer_item_free(ar_buffer_t *buffer, void *item) {
    ar_buffer_item_free_at_index(buffer, ar_buffer_get_item_index(buffer, item));
}

void *ar_buffer_get(ar_buffer_t *buffer, unsigned index) {
    if (index > buffer->size) {
        ar_log(AR_LOG_WARN, "index(%d) > buffer_size(%d)\n", index, buffer->size);
        return NULL;
    }
    void *ptr = ((uint8_t *)buffer->data)+(buffer->obj_sz*index);
    return ptr;
}

ar_buffer_t ar_buffer_duplicate(ar_buffer_t *buffer, ar_buffer_type_t type) {
    ar_buffer_t buf;
    buf = ar_buffer_from_data(type, buffer->data, buffer->obj_sz, buffer->index, buffer->flags);
    return buf;
}

ar_buffer_t ar_buffer_from_data(ar_buffer_type_t type, void *data, uint16_t obj_sz, uint32_t data_size, uint16_t flags) {
    ar_buffer_t buffer;
    ar_buffer_init(&buffer, type, obj_sz, data_size, flags);
    ar_buffer_copy_data(&buffer, data, data_size);
    buffer.index = data_size;
    return buffer;
}

void ar_buffer_copy_data(ar_buffer_t *buffer, void *data, int data_size) {
    const int buffer_start = buffer->index;
    const int block_size = data_size;
    
    if ((unsigned)buffer_start+block_size > buffer->size) {
        if (buffer->type != AR_BUFFER_DYNAMIC) {
            ar_log(AR_LOG_ERROR, "Hit end of non-dynamic buffer\n", 0);
            return;
        }
        ar_buffer_resize(buffer, buffer_start+block_size);
    }
    memcpy((uint8_t *)buffer->data+buffer_start, (uint8_t *)data, block_size);
    buffer->index += block_size;
}

void ar_buffer_resize(ar_buffer_t *buffer, int size) {
    if (!ar_buffer_is_initialized(buffer)) {
        ar_log(AR_LOG_ERROR, "Buffer not initialized\n", 0);
        return;
    }

    if (buffer->type != AR_BUFFER_DYNAMIC)
        return;
    
    // if there is a large difference in buffer->index and buffer->size (there shouldn't
    // be due to ar_buffer_push checking) we set the size to the index to avoid coming up 
    // short on the resize.
    if (buffer->index > buffer->size)
        buffer->size = buffer->index;

    if (size == AR_BUFFER_RESIZE_AUTO)
        buffer->size = buffer->resize_func(buffer);
    else
        buffer->size = size;
    
    const size_t size_in_bytes = buffer->size*buffer->obj_sz;
    buffer->data = buffer_mrealloc(buffer, size_in_bytes);
    if (buffer->flags & AR_BUFFER_PACK) {
        buffer->free_map = realloc(buffer->free_map, buffer->size);
    }

    // error in realloc, probably ran out of memory
    if (buffer->data == NULL) {
        ar_log(AR_LOG_ERROR, "Error resizing buffer %u*%u(%lu)\n", buffer->size, buffer->obj_sz, size_in_bytes);
    }
}

void ar_buffer_reduce_to_data(ar_buffer_t *buffer) {
    ar_buffer_resize(buffer, buffer->index);
}

size_t ar_buffer_resize_func_double(ar_buffer_t *buffer) {
    return buffer->size*2;
}

void ar_buffer_destroy(ar_buffer_t *buffer) {
    if (!ar_buffer_is_initialized(buffer))
        return;

    buffer_mfree(buffer);
    if (buffer->flags & AR_BUFFER_PACK)
        free(buffer->free_map);
    buffer->flags &= ~AR_BUFFER_INITIALIZED;
    buffer->data = NULL;
    buffer->size = 0;
    buffer->index = 0;
}

/*
 * Local buffer memory functions
 */

static void *buffer_malloc(ar_buffer_t *buffer, size_t size) {
    if (buffer->flags & AR_BUFFER_UNTRACKED)
        return malloc(size);
    return ar_memory_alloc(size);
}

static void *buffer_mrealloc(ar_buffer_t *buffer, size_t new_size) {
    if (buffer->flags & AR_BUFFER_UNTRACKED)
        return realloc(buffer->data, new_size);
    return ar_memory_realloc(buffer->data, new_size);
}

static void buffer_mfree(ar_buffer_t *buffer) {
    if (buffer->flags & AR_BUFFER_UNTRACKED)
        free(buffer->data);
    else
        ar_memory_free(buffer->data);
}
