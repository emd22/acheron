#include <f3d/engine/engine.h>
#include <f3d/engine/log.h>

#include <GL/glew.h>
#include <GL/gl.h>

const char *engine_get_opengl_error(void) {
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
            log_msg(LOG_WARN, "Unknown 0x%x\n", error);
            return "Unknown error";
    };
    return NULL;
}
