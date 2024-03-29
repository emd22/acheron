#include <acheron/engine/acheron.h>
#include <acheron/engine/types.h>
#include <acheron/engine/core/cr_handles.h>
#include <acheron/engine/core/cr_controls.h>
#include <acheron/engine/core/cr_debug.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/core/cr_time.h>

#include <acheron/engine/core/threads/cr_thread.h>

#include <acheron/engine/scene/sc_scene.h>
#include <acheron/engine/scene/sc_light.h>
#include <acheron/engine/asset/at_assetman.h>

#define AR_GL_VERSION_MAJOR 4
#define AR_GL_VERSION_MINOR 5

#include <SDL2/SDL.h>

#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

static ar_instance_t ar_instance;
ar_instance_t *ar_instance_selected = NULL;

ar_instance_t *ar_instance_new(int flags) {
    ar_instance.flags = flags;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ar_log(AR_LOG_FATAL, "GL: Could not start SDL\n", 0);
        return NULL;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, AR_GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, AR_GL_VERSION_MINOR);
    if (ar_instance_selected == NULL)
        ar_instance_selected = &ar_instance;

    ar_controls_init();
    ar_time_init();

    return &ar_instance;
}

void ar_instance_attach(ar_instance_t *instance, ar_instance_attach_type_t attach_type, void *ptr) {
    if (attach_type == AR_INSTANCE_ATTACH_WINDOW) {
        ar_window_t *window = (ar_window_t *)ptr;
        instance->window = window;
    }
}

static void signal_handler(int sig) {
    ar_handle_call(AR_HANDLE_SYS_SIGNAL, &sig);
    switch (sig) {
        case SIGINT:
            ar_handle_call(AR_HANDLE_END, NULL);
            exit(EXIT_SUCCESS);
            break;;
        case SIGSEGV:
            ar_log(AR_LOG_FATAL, "Segmentation Fault occurred... attempting to exit gracefully\n", 0);
            ar_debug_print_backtrace();
            ar_handle_call(AR_HANDLE_END, NULL);
            exit(EXIT_FAILURE);
            break;
        case SIGABRT:
            ar_log(AR_LOG_FATAL, "Caught abort signal\n", 0);
            ar_debug_print_backtrace();
            exit(EXIT_FAILURE);
            break;
        case SIGTERM:
            ar_log(AR_LOG_INFO, "Termination signal caught\n", 0);
            ar_handle_call(AR_HANDLE_END, NULL);
            exit(EXIT_SUCCESS);
            break;
        default:
            ar_log(AR_LOG_ERROR, "Unhandled signal %d recieved from system\n", sig);
    }
}

void ar_init(ar_instance_t *instance) {
    ar_threadman_init();
    
    // setup system signals
    signal(SIGINT,  &signal_handler);
    signal(SIGSEGV, &signal_handler);
    signal(SIGABRT, &signal_handler);
    signal(SIGFPE,  &signal_handler);
    signal(SIGILL,  &signal_handler);
    signal(SIGTERM, &signal_handler);
    
    instance->running = true;
    ar_handles_init();
    ar_meshes_init();
    ar_assetman_init();

    if (instance->flags & AR_INSTANCE_GRAPHICS) {
        instance->renderer = ar_renderer_init();
    }
}

void ar_instance_stop_threads(ar_instance_t *instance) {
    // tell threads to stop
    instance->running = false;

    ar_thread_t **asset_thread = NULL;
    // get assetman thread, wait for completion
    asset_thread = ar_assetman_get_thread();
    ar_thread_join((*asset_thread), NULL);
}

bool ar_instance_is_running(ar_instance_t *instance) {
    // no instance passed in, check if any instance is running
    if (instance == NULL)
        instance = ar_instance_get_selected();
    return instance->running;
}

ar_instance_t *ar_instance_get_selected(void) {
    return ar_instance_selected;
}

void ar_instance_destroy(void) {
    if (!ar_instance.flags)
        return;

    if (ar_instance.flags & AR_INSTANCE_GRAPHICS)
        ar_renderer_destroy(&ar_instance.renderer);
        
    ar_instance.flags = 0;
}

void ar_cleanup(void) {
    ar_assetman_destroy();
    ar_threadman_destroy();

    ar_light_buffer_destroy();
    ar_object_buffer_destroy();
    ar_scene_buffer_destroy();
    ar_window_buffer_destroy();
    ar_shader_buffer_destroy();
    ar_texture_buffer_destroy();
    ar_material_buffer_destroy();
    
    ar_meshes_cleanup();
    ar_memory_cleanup();
}

void ar_exit(ar_instance_t *instance) {
    if (instance == NULL)
        instance = ar_instance_get_selected();

    ar_instance_stop_threads(instance);

    ar_instance_destroy();
    ar_cleanup();
}
