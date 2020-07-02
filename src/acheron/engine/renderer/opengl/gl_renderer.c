#ifdef AR_USE_OPENGL

#include <acheron/engine/acheron.h>
#include <acheron/engine/renderer/rr_renderer.h>
#include <acheron/engine/core/cr_window.h>
#include <acheron/engine/core/cr_log.h>

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

const char *ar_renderer_check_error(void) {
    int error = glGetError();
    switch (error) {
        case GL_NO_ERROR:
            return NULL;
        case GL_INVALID_ENUM:
            return "Invalid enum";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
        case GL_STACK_OVERFLOW:
            return "Stack overflow";
        case GL_STACK_UNDERFLOW:
            return "Stack underflow";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        default:
            ar_log(AR_LOG_WARN, "Unknown 0x%x\n", error);
            return "Unknown error";
    };
    return NULL;
}

void ar_renderer_intern_init(ar_renderer_instance_t *instance) {
    memset(instance, 0, sizeof(ar_renderer_instance_t));

    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        ar_log(AR_LOG_FATAL, "GL: Could not initialize GLEW\n", 0);
        exit(1);
    }
    
    ar_window_t *default_window = ar_instance_get_selected()->window;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, default_window->width, default_window->height);
    ar_renderer_enable(AR_RENDERER_FRAMEBUFFER_SRGB);
    ar_renderer_enable(AR_RENDERER_DEPTH_TEST);
    ar_renderer_enable(AR_RENDERER_CULL_FACE);
    glDepthFunc(GL_LESS);
    
    instance->renderer_type = AR_RENDERER_OPENGL;
}

#endif
