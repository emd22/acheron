#include <f3d/engine/renderer/opengl/opengl.h>
#include <f3d/engine/renderer/renderer.h>
#include <f3d/engine/core/window.h>
#include <f3d/engine/acheron.h>

#ifdef AR_USE_OPENGL

#include <GL/glew.h>
#include <GL/gl.h>

static int get_option_code_gl(ar_renderer_enable_t code) {
    switch (code) {
        case AR_RENDERER_BLEND:
            return GL_BLEND;
            
        case AR_RENDERER_CULL_FACE:
            return GL_CULL_FACE;
            
        case AR_RENDERER_DEPTH_TEST:
            return GL_DEPTH_TEST;
            
        case AR_RENDERER_FRAMEBUFFER_SRGB:
            return GL_FRAMEBUFFER_SRGB;
            
        case AR_RENDERER_MULTISAMPLE:
            return GL_MULTISAMPLE;
            
        case AR_RENDERER_LINE_SMOOTH:
            return GL_LINE_SMOOTH;

        case AR_RENDERER_POLYGON_SMOOTH:
            return GL_POLYGON_SMOOTH;
            
        default:
            return -1;
    }
}

void ar_renderer_enable(ar_renderer_enable_t code) {
    int glcode = get_option_code_gl(code);
    if (glcode != -1)
        glEnable(glcode);
}

void ar_renderer_disable(ar_renderer_enable_t code) {
    int glcode = get_option_code_gl(code);
    if (glcode != -1)
        glDisable(glcode);
}

void ar_renderer_opengl_init(void) {
    ar_window_t *default_window = ar_instance_get_selected()->window;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, default_window->width, default_window->height);
    ar_renderer_enable(AR_RENDERER_FRAMEBUFFER_SRGB);
    ar_renderer_enable(AR_RENDERER_DEPTH_TEST);
    ar_renderer_enable(AR_RENDERER_CULL_FACE);
    glDepthFunc(GL_LESS);
}

#endif
