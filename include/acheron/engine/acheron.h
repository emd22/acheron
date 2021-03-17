#ifndef ACHERON_H
#define ACHERON_H

#include <stdbool.h>

#include <acheron/engine/renderer/rr_renderer.h>
#include <acheron/engine/core/cr_window.h>
#include <acheron/engine/core/cr_handles.h>
#include <acheron/engine/core/cr_controls.h>
#include <acheron/engine/core/memory/mm_memory.h>

#define AR_VALUE_QUERY -1

typedef enum {
    AR_INSTANCE_ATTACH_WINDOW,
} ar_instance_attach_type_t;

enum {
    AR_INSTANCE_GRAPHICS = 0x01,  
    AR_INSTANCE_ASYNC_ASSETS = 0x02,
};

#define AR_INSTANCE_ALL (AR_INSTANCE_GRAPHICS | AR_INSTANCE_ASYNC_ASSETS)

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
