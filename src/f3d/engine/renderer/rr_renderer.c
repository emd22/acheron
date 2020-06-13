#include <f3d/engine/renderer/rr_renderer.h>

#include <f3d/engine/core/log.h>
#include <f3d/engine/types.h>

#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

ar_renderer_instance_t ar_renderer_init() {
    ar_render_instance_t instance;
    ar_renderer_intern_init();
    
    // TODO: replace with renderer independent functions
    SDL_GL_SetSwapInterval(1);
    
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    //glEnable(GL_MULTISAMPLE);
}

void ar_renderer_destroy(ar_renderer_instance_t *instance) {
    instance->renderer_type = AR_RENDERER_NONE;
    SDL_Quit();
}
