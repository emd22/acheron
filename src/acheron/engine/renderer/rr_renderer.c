#include <acheron/engine/renderer/rr_renderer.h>

#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/types.h>

#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

ar_renderer_instance_t ar_renderer_init() {
    ar_renderer_instance_t instance;
    ar_renderer_intern_init(&instance);
    
    // TODO: replace with renderer independent functions
    SDL_GL_SetSwapInterval(1);
    
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    //glEnable(GL_MULTISAMPLE);'
    return instance;
}

void ar_renderer_destroy(ar_renderer_instance_t *instance) {
    instance->renderer_type = AR_RENDERER_NONE;
    SDL_Quit();
}
