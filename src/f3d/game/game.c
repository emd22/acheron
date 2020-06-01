#include <f3d/engine/acheron.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/game/player.h>

#include <f3d/engine/rendering/render.h>
#include <f3d/engine/scene/scene.h>

#include <GL/glew.h>
#include <GL/gl.h>

int main() {
    ar_instance_t *instance = ar_instance_new(AR_INSTANCE_GRAPHICS);
    ar_window_t *window = ar_window_new("Acheron3d", 700, 700, 0);
    ar_instance_attach(instance, AR_INSTANCE_ATTACH_WINDOW, window);
    ar_init(instance);

    ar_control_set_mode(SDLK_q, AR_CONTROL_MODE_TOGGLE);
    
    ar_scene_t *scene = ar_scene_new("Main Scene");
    ar_scene_select(scene, shader_main);
    
    glClearColor(1.0, 0, 0, 1.0f);
    
    while (instance->running) {
        ar_controls_poll_events();
        player_move(NULL);
        if (ar_control_check(SDLK_q)) {
            instance->running = false;
        }
        ar_window_buffers_swap(window);
    }
    ar_window_destroy(window);
    ar_instance_destroy();
}
