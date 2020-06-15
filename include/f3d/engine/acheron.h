#ifndef ACHERON_H
#define ACHERON_H

#include <stdbool.h>

#include <f3d/engine/renderer/rr_renderer.h>
#include <f3d/engine/core/window.h>
#include <f3d/engine/core/controls.h>
#include <f3d/engine/core/memory/mm_memory.h>

typedef enum {
    AR_INSTANCE_ATTACH_WINDOW,
} ar_instance_attach_type_t;

enum {
    AR_INSTANCE_GRAPHICS = 0x01,  
};

#define AR_INSTANCE_ALL (AR_INSTANCE_GRAPHICS)

typedef struct {
    int flags;
    bool running;
    ar_window_t *window;
    ar_renderer_instance_t renderer;
} ar_instance_t;

extern ar_instance_t *ar_instance_selected;

ar_instance_t *ar_instance_new(int flags);
void ar_instance_attach(ar_instance_t *instance, ar_instance_attach_type_t attach_type, void *ptr);
ar_instance_t *ar_instance_get_selected(void);
void ar_init(ar_instance_t *instance);
void ar_instance_destroy(void);
void ar_instances_destroy(void);
void ar_cleanup(void);

#endif
