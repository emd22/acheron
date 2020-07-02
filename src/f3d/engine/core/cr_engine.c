#include <f3d/engine/engine.h>

#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GL/gl.h>

void signal_handler(int sig) {
    // CTRL+C Interrupt
    if (sig == SIGINT) {
        handle_call(HANDLE_END, NULL);
        exit(0);
    }
    // Segmentation fault
    else if (sig == SIGSEGV) {
        ar_log(AR_LOG_FATAL, "Segmentation Fault occurred... attempting to exit gracefully\n", 0);
        debug_print_backtrace();
        static int segv_count = 0;
        segv_count++;
        if (segv_count > 2) {
            ar_log(AR_LOG_WARN, "Forcefully exiting...\n", 0);
            exit(1);
        }
        handle_call(HANDLE_END, NULL);
    }
    else {
        ar_log(AR_LOG_WARN, "Unknown signal caught: %d\n", sig);
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
