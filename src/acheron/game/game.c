#include <acheron/engine/acheron.h>
#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/game/player.h>

#include <acheron/engine/rendering/render.h>
#include <acheron/engine/scene/sc_scene.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/core/cr_time.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define MOUSE_SPEED 0.07\

int check_mouse(void *arg) {
    ar_control_mouse_event_t *mouse_event;
    mouse_event = (ar_control_mouse_event_t *)arg;
    
    selected_camera->rotation.x -= ar_time_get_delta()*(MOUSE_SPEED*mouse_event->relx);
    selected_camera->rotation.y -= ar_time_get_delta()*(MOUSE_SPEED*mouse_event->rely);
    return 0;
}

void init_lights(ar_scene_t *scene) {
    ar_light_t *light = ar_light_new(AR_LIGHT_POINT);
    light->position = (ar_vector3f_t){-1.37f, 3.0f, -6.13f};
    light->diffuse = (ar_vector3f_t){0.2, 1.0, 0.2};
    ar_scene_attach(scene, AR_SCENE_ATTACH_LIGHT, light);
    ar_light_shadow_new(light, 700, 700);
    
    ar_light_t *light2 = ar_light_new(AR_LIGHT_POINT);
    light2->position = (ar_vector3f_t){1.32f, 3.0f, 5.2f};
    light2->diffuse = (ar_vector3f_t){1.0, 0.2, 0.2};
    ar_scene_attach(scene, AR_SCENE_ATTACH_LIGHT, light2);
    ar_light_shadow_new(light2, 700, 700);
}

void init_objects(ar_scene_t *scene) {
    ar_object_t *object = ar_object_new("Level");
    ar_object_attach(object, AR_OBJECT_ATTACH_MESH, ar_mesh_load(NULL, "../models/reception/reception.obj", MODEL_OBJ, 0));
    object_move(object, 0, 0, 0);
    ar_scene_attach(scene, AR_SCENE_ATTACH_OBJECT, object);
}

int main() {
    ar_instance_t *instance = ar_instance_new(AR_INSTANCE_GRAPHICS);
    ar_window_t *window = ar_window_new("Acheron3d", 640, 480, 0);
    ar_instance_attach(instance, AR_INSTANCE_ATTACH_WINDOW, window);
    ar_init(instance);

    render_init();

    camera_t camera = camera_new(CAMERA_PERSPECTIVE);
    camera.move_speed = 6.0f;
    camera.position = (ar_vector3f_t){0, 3, 4};
    camera.rotation.x = 3.14f;
    camera_select(&camera);
    
    ar_control_set_mode(SDLK_q, AR_CONTROL_MODE_TOGGLE);
    ar_control_set_mode(SDLK_e, AR_CONTROL_MODE_TOGGLE);
    ar_control_set_mode(SDLK_ESCAPE, AR_CONTROL_MODE_TOGGLE);
    ar_handle_set(AR_HANDLE_MOUSE_MOVE, &check_mouse);
    
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
        if (ar_control_check(SDLK_ESCAPE)) {
            ar_window_option_set(window, AR_WINDOW_OPTION_MOUSE_VISIBLE, true);
        }
        ar_shader_use(shader_main);

        player_move(&camera);
        camera_update(selected_camera);
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

    ar_memory_cleanup();
}
