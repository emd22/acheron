#include <f3d/engine/acheron.h>
#include <f3d/engine/core/controls.h>
#include <f3d/engine/renderer/renderer.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/types.h>

#include <f3d/engine/scene/scene.h>
#include <f3d/engine/scene/light.h>

#define AR_GL_VERSION_MAJOR 4
#define AR_GL_VERSION_MINOR 2

#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>

static ar_instance_t ar_instance;
ar_instance_t *ar_instance_selected = NULL;

ar_instance_t *ar_instance_new(int flags) {
    ar_instance.flags = flags;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ar_log(AR_LOG_FATAL, "GL: Could not start SDL\n", 0);
        return NULL;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, AR_GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, AR_GL_VERSION_MINOR);
    if (ar_instance_selected == NULL)
        ar_instance_selected = &ar_instance;

    ar_controls_init();

    return &ar_instance;
}

void ar_instance_attach(ar_instance_t *instance, ar_instance_attach_type_t attach_type, void *ptr) {
    if (attach_type == AR_INSTANCE_ATTACH_WINDOW) {
        ar_window_t *window = (ar_window_t *)ptr;
        instance->window = window;
    }
}

void ar_init(ar_instance_t *instance) {
    instance->running = true;
    if (instance->flags & AR_INSTANCE_GRAPHICS) {
        instance->renderer = ar_renderer_init();
    }
}

ar_instance_t *ar_instance_get_selected(void) {
    return ar_instance_selected;
}

void ar_instance_destroy(void) {
    if (ar_instance.flags == 0)
        return;

    if (ar_instance.flags & AR_INSTANCE_GRAPHICS)
        ar_renderer_destroy(&ar_instance.renderer);
        
    ar_instance.flags = 0;
}

void ar_instances_destroy(void) {
    return;
}

void ar_cleanup(void) {
    ar_lights_destroy();
    ar_scenes_destroy();
    ar_instances_destroy();
}
