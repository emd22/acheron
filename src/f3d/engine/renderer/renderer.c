#include <f3d/engine/renderer/renderer.h>

#include <f3d/engine/core/log.h>
#include <f3d/engine/types.h>

#include <stdlib.h>
#include <stdbool.h>

// TODO: use different extension loading libaries for different backends
#ifdef AR_USE_OPENGL
#include <GL/glew.h>
#include <SDL2/SDL.h>

ar_renderer_instance_t ar_renderer_init(void) {
    ar_renderer_instance_t instance;
    memset(&instance, 0, sizeof(ar_renderer_instance_t));

    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        ar_log(AR_LOG_FATAL, "GL: Could not initialize GLEW\n", 0);
        exit(1);
    }
    // enable vsync
    SDL_GL_SetSwapInterval(1);
    
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    //glEnable(GL_MULTISAMPLE);
    ar_renderer_opengl_init();
    
    instance.renderer_type = AR_RENDERER_OPENGL;
    return instance;
}

void ar_renderer_destroy(ar_renderer_instance_t *instance) {
    instance->renderer_type = AR_RENDERER_NONE;
    SDL_Quit();
}

#else

ar_renderer_instance_t ar_renderer_init(void) {
    ar_renderer_instance_t instance;
    instance.renderer_type = AR_RENDERER_NONE;
    return instance;
}

void ar_renderer_destroy(ar_renderer_instance_t *instance) {
    instance->renderer_type = AR_RENDERER_NONE;
}

#endif
