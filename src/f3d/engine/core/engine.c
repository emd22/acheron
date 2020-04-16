#include <f3d/engine/engine.h>

#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

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

void signal_handler(int sig) {
    // CTRL+C Interrupt
    if (sig == SIGINT) {
        handle_call(HANDLE_END, NULL);
        exit(0);
    }
    // Segmentation fault
    else if (sig == SIGSEGV) {
        log_msg(LOG_WARN, "Segmentation Fault occurred... attempting to exit gracefully\n", 0);
        debug_print_backtrace();
        static int segv_count = 0;
        segv_count++;
        if (segv_count > 2) {
            log_msg(LOG_WARN, "Forcefully exiting...\n", 0);
            exit(1);
        }
        handle_call(HANDLE_END, NULL);
    }
    else {
        log_msg(LOG_WARN, "Unknown signal caught: %d\n", sig);
    }
}

void engine_setup_signals(void) {
    signal(SIGINT, &signal_handler);
    signal(SIGSEGV, &signal_handler);
}

void engine_render_wireframe(bool n) {
    if (n)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
