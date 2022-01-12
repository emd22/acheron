#ifdef AR_USE_OPENGL

#include <acheron/engine/acheron.h>
#include <acheron/engine/renderer/rr_renderer.h>
#include <acheron/engine/core/cr_window.h>
#include <acheron/engine/core/cr_log.h>

#include <GL/glew.h>
#include <GL/gl.h>

static int get_option_code_gl(ar_renderer_enable_t code) {
    const int option_code_gl_table[] = {
        GL_BLEND,            /* AR_RENDERER_BLEND */
        GL_CULL_FACE,        /* AR_RENDERER_CULL_FACE */
        GL_DEPTH_TEST,       /* AR_RENDERER_DEPTH_TEST */
        GL_FRAMEBUFFER_SRGB, /* AR_RENDERER_FRAMEBUFFER_SRGB */
        GL_MULTISAMPLE,      /* AR_RENDERER_MULTISAMPLE */
        GL_LINE_SMOOTH,      /* AR_RENDERER_LINE_SMOOTH */
        GL_POLYGON_SMOOTH,   /* AR_RENDERER_POLYGON_SMOOTH */
    };
    return option_code_gl_table[code];
}

static int ar_gl_buffer_flags(ar_renderer_buffers_t flags) {
    int final = 0;

    if (flags & AR_RENDERER_BUFFER_COLOR)
        final |= GL_COLOR_BUFFER_BIT;
    if (flags & AR_RENDERER_BUFFER_DEPTH)
        final |= GL_DEPTH_BUFFER_BIT;
    if (flags & AR_RENDERER_BUFFER_STENCIL)
        final |= GL_STENCIL_BUFFER_BIT;

    return final;
}

void ar_renderer_enable(ar_renderer_enable_t code) {
    int glcode = get_option_code_gl(code);
    glEnable(glcode);
}

void ar_renderer_disable(ar_renderer_enable_t code) {
    int glcode = get_option_code_gl(code);
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

void ar_renderer_clear(ar_renderer_buffers_t buffer_flags) {
    int flags = ar_gl_buffer_flags(buffer_flags);
    glClear(flags);
}

void GLAPIENTRY ar_gl_message_callback(
    unsigned src, unsigned type, unsigned id, 
    unsigned severity, int length, 
    const char *message, const void *data)
{
    (void)src;
    (void)id;
    (void)data;
    int log_sev = (type == GL_DEBUG_TYPE_ERROR) ? AR_LOG_RENDER_ERROR : AR_LOG_RENDER_INFO;
    ar_log(log_sev, "GL message[0x%04X]: %.*s || SEV:0x%04X\n", type, length, message, severity);
}

void ar_renderer_intern_init(ar_renderer_instance_t *instance) {
    memset(instance, 0, sizeof(ar_renderer_instance_t));

    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        ar_log(AR_LOG_FATAL, "GL: Could not initialize GLEW\n", 0);
        exit(1);
    }
    if (glDebugMessageCallback) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(&ar_gl_message_callback, NULL);
        GLuint unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            &unusedIds,
            true
        );
        ar_log(AR_LOG_INFO, "Registered glDebugMessageCallback\n", 0);
    }
    else {
        ar_log(AR_LOG_INFO, "glDebugMessageCallback not supported\n", 0);
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
