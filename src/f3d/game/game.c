#include <f3d/engine/acheron.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/engine.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/game/player.h>

#include <f3d/engine/rendering/render.h>
#include <f3d/engine/scene/scene.h>
#include <f3d/engine/core/time.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define MOUSE_SPEED 0.07

void check_mouse(SDL_Event *event) {
    selected_camera->rotation.x -= ar_time_get_delta()*(MOUSE_SPEED*event->motion.xrel);
    selected_camera->rotation.y -= ar_time_get_delta()*(MOUSE_SPEED*event->motion.yrel);
}

void init_lights(ar_scene_t *scene) {
    ar_light_t *light = ar_light_new(AR_LIGHT_POINT);
    light->position = (ar_vector3f_t){-1.37f, 3.0f, -6.13f};
    light->diffuse = (ar_vector3f_t){0.2, 1.0, 0.2};
    ar_scene_attach(scene, AR_SCENE_ATTACH_LIGHT, light);
    ar_light_shadow_new(light, 700, 700);
    
    ar_light_t *light2 = ar_light_new(AR_LIGHT_POINT);
    light2->position = (ar_vector3f_t){0.0f, 3.0f, 2.0f};
    light2->diffuse = (ar_vector3f_t){1.0, 0.2, 0.2};
    ar_scene_attach(scene, AR_SCENE_ATTACH_LIGHT, light2);
    ar_light_shadow_new(light2, 700, 700);
}

void init_objects(ar_scene_t *scene) {
    ar_object_t *object = ar_object_new("Cube");
    ar_object_attach(object, AR_OBJECT_ATTACH_MESH, mesh_load(NULL, "../models/reception/reception.obj", MODEL_OBJ, 0));
    object_move(object, 0, 0, 0);
    ar_scene_attach(scene, AR_SCENE_ATTACH_OBJECT, object);
}


int main() {
    ar_instance_t *instance = ar_instance_new(AR_INSTANCE_GRAPHICS);
    ar_window_t *window = ar_window_new("Acheron3d", 700, 700, 0);
    ar_instance_attach(instance, AR_INSTANCE_ATTACH_WINDOW, window);
    ar_init(instance);
    
    engine_setup_signals();
    
    render_init();

    camera_t camera = camera_new(CAMERA_PERSPECTIVE);
    camera.move_speed = 6.0f;
    camera.position = (ar_vector3f_t){0, 3, 4};
    camera.rotation.x = 3.14f;
    camera_select(&camera);
    
    ar_control_set_mode(SDLK_q, AR_CONTROL_MODE_TOGGLE);
    ar_controls_set_mouse_func(&check_mouse);
    
    ar_scene_t *scene = ar_scene_new("Main Scene");

    init_objects(scene);
    init_lights(scene);

    ar_scene_select(scene, shader_main);
    
    ar_scene_render_shadows(scene, shader_main);
    ar_window_option_set(window, AR_WINDOW_OPTION_MOUSE_VISIBLE, false);
    
    while (instance->running) {
        time_tick();
        ar_controls_poll_events();
        if (ar_control_check(SDLK_q)) {
            instance->running = false;
        }
        ar_shader_use(shader_main);

        player_move(&camera);
        camera_update(selected_camera);
        ar_shader_set_vec3f(shader_main, "view_pos", selected_camera->position);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        render_all();
        ar_window_buffers_swap(window);
        time_end();
    }
    
    render_destroy();
    ar_scene_destroy(scene);
    ar_window_destroy(window);
    ar_instance_destroy();
    
    ar_cleanup();
    
    ar_log(AR_LOG_DEBUG, "Memory Leaked: %.02fKB\n", (float)ar_memory_used()/1024.0f);
    ar_memory_cleanup();
}
