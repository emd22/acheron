#include <acheron/engine/acheron.h>

#include <acheron/engine/renderer/rr_shader.h>

#include <acheron/engine/scene/sc_scene.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/core/cr_time.h>

#include <acheron/engine/asset/at_assetman.h>

#include <acheron/engine/camera/cm_perspective.h>

#include <GL/glew.h>
#include <GL/gl.h>

ar_camera_perspective_t pers_camera;
ar_camera_t *camera;
ar_scene_t *scene;

int handle_mouse(void *arg) {
    const float mouse_sensitivity = 0.09f;
    ar_control_mouse_event_t *event = (ar_control_mouse_event_t *)arg;
    camera->rotation.x -= ar_time_get_delta()*(mouse_sensitivity*event->relx);
    camera->rotation.y -= ar_time_get_delta()*(mouse_sensitivity*event->rely);
    return 0;
}

void init_object_stuffs() {
    // Scene
    scene = ar_scene_new("Main Scene");

    // create camera, move to (0, 3, -4)
    pers_camera = ar_camera_perspective_new();
    camera = &pers_camera.camera;
    camera->position = (ar_vector3f_t){0, 3, -4};

    // create new object and load asset
    ar_object_t *level = ar_object_new("Level");
    ar_asset_t *level_asset = ar_asset_load(AR_ASSET_MESH, "../models/reception/reception.obj");
    // attach asset to object, attach object to scene
    ar_object_attach(level, AR_OBJECT_ATTACH_MESH, level_asset);
    ar_scene_attach(scene, AR_SCENE_ATTACH_OBJECT, level);

    // create new light, attach to scene
    ar_light_t *light = ar_light_new(AR_LIGHT_POINT);
    light->position = (ar_vector3f_t){1, 5, 0};
    light->diffuse = (ar_vector3f_t){1.0, 1.0, 0.6};
    ar_scene_attach(scene, AR_SCENE_ATTACH_LIGHT, light);

    // select this scene as our default
    ar_scene_select(scene, ar_shaderman_get_render_shader());
}


int main() {
    ar_instance_t *instance = ar_instance_new(AR_INSTANCE_GRAPHICS);
    ar_window_t *window = ar_window_new("Window Title", 1024, 768, 0);
    
    ar_window_option_set(window, AR_WINDOW_OPTION_MOUSE_VISIBLE, 0);
    ar_instance_attach(instance, AR_INSTANCE_ATTACH_WINDOW, window);

    ar_init(instance);

    // set R and E to be toggleable keys
    ar_control_set_mode(SDLK_r, AR_CONTROL_MODE_TOGGLE);
    ar_control_set_mode(SDLK_e, AR_CONTROL_MODE_TOGGLE);

    init_object_stuffs();

    // handle camera rotations
    ar_handle_set(AR_HANDLE_MOUSE_MOVE, &handle_mouse);

    while (instance->running) {
        ar_time_tick();
        ar_controls_poll_events();

        if (ar_control_check(SDLK_q))
            instance->running = false;
        
        // change field of vision and reload camera
        if (ar_control_check(SDLK_e)) {
            pers_camera.fov -= 5.0f;
            camera->reload(camera, &pers_camera);
        }
        if (ar_control_check(SDLK_r)) {
            pers_camera.fov += 5.0f;
            camera->reload(camera, &pers_camera);
        }
        ar_camera_update(camera);

        ar_renderer_draw(camera);
        ar_window_buffers_swap(window);
    }

    ar_window_destroy(window);
    ar_scene_destroy(scene);

    ar_instance_destroy();
    ar_cleanup();

    return 0;
}