#include <f3d/engine/acheron.h>
#include <f3d/engine/renderer/renderer.h>
#include <f3d/engine/types.h>

#include <stdlib.h>
#include <stdbool.h>

static ar_instance_t ar_instance;
ar_instance_t *ar_instance_selected = NULL;

ar_instance_t *ar_instance_new(int flags) {
    ar_instance.flags = flags;
     
    if (ar_instance.flags & AR_INSTANCE_GRAPHICS) {
        ar_instance.renderer = ar_renderer_init();
    }
    
    if (ar_instance_selected == NULL)
        ar_instance_selected = &ar_instance;
        
    return &ar_instance;
}

void ar_instance_attach(ar_instance_t *instance, ar_instance_attach_type_t attach_type, void *ptr) {
    if (attach_type == AR_INSTANCE_ATTACH_WINDOW) {
        ar_window_t *window = (ar_window_t *)ptr;
        instance->window = window;
    }
}

void ar_instance_destroy(void) {
    if (ar_instance.flags == 0)
        return;

    if (ar_instance.flags & AR_INSTANCE_GRAPHICS)
        ar_renderer_destroy(&ar_instance.renderer);

    ar_instance.flags = 0;
}
