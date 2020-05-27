#include <f3d/engine/renderer/renderer.h>

#include <f3d/engine/core/log.h>
#include <f3d/engine/types.h>

#include <stdlib.h>
#include <stdbool.h>

// TODO: use different extension loading libaries for different backends
#ifdef SB_USE_OPENGL
#include <GL/glew.h>

#define SB_GL_VERSION_MAJOR 4
#define SB_GL_VERSION_MINOR 2

static void init_context() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        sb_log(LOG_FATAL, "GL: Could not start SDL\n", 0);
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, SB_GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, SB_GL_VERSION_MINOR);

    glewExperimental = 1;
    if (glewInit != GLEW_OK) {
        sb_log(LOG_FATAL, "GL: Could not initialize GLEW\n", 0);
        exit(1);
    }
    // enable vsync
    SDL_GL_SetSwapInterval(1);
    
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    //glEnable(GL_MULTISAMPLE);
}

void sb_renderer_init(void) {
    init_context();
}

#else

void sb_renderer_init(void) {
    return;
}

#endif
