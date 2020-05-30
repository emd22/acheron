#include <f3d/engine/renderer/renderer.h>

#include <f3d/engine/core/log.h>
#include <f3d/engine/types.h>

#include <stdlib.h>
#include <stdbool.h>

// TODO: use different extension loading libaries for different backends
#ifdef AR_USE_OPENGL
#include <GL/glew.h>

#define AR_GL_VERSION_MAJOR 4
#define AR_GL_VERSION_MINOR 2

ar_renderer_instance_t ar_renderer_init(void) {
    ar_renderer_instance_t instance;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ar_log(LOG_FATAL, "GL: Could not start SDL\n", 0);
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, AR_GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, AR_GL_VERSION_MINOR);

    glewExperimental = 1;
    if (glewInit != GLEW_OK) {
        ar_log(LOG_FATAL, "GL: Could not initialize GLEW\n", 0);
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
