#ifndef BUFFER_H
#define BUFFER_H

enum {
    BUFFER_ABSOLUTE,
    BUFFER_DOUBLE,
    BUFFER_ADD,
};

enum {
    BUFFER_OPTION_EXPAND,
};

typedef struct {
    void *data;
    // object size in bytes
    unsigned obj_sz;
    unsigned index;
    // allocated size in objects
    unsigned size;
    unsigned short flags;
} buffer_t;

#endif
