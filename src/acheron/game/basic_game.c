#include <acheron/engine/acheron.h>

#include <acheron/engine/renderer/rr_shader.h>

#include <acheron/engine/scene/sc_scene.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/core/cr_time.h>

#include <acheron/engine/asset/at_assetman.h>

#include <acheron/engine/camera/cm_ortho.h>
#include <acheron/engine/camera/cm_perspective.h>

#include <acheron/engine/renderer/rr_dp_shadows.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <ar_image/ari_jpeg.h>

#include <stdio.h>

ar_camera_t camera, shadow_cam;
ar_scene_t *scene;

ar_object_t *level, *cube;

int handle_mouse(void *arg) {
    const float mouse_sensitivity = 0.09f;
    ar_control_mouse_event_t *event = (ar_control_mouse_event_t *)arg;
    camera.rotation.x -= ar_time_get_delta()*(mouse_sensitivity*event->relx);
    camera.rotation.y -= ar_time_get_delta()*(mouse_sensitivity*event->rely);
    return 0;
}

void init_object_stuffs() {
    scene = ar_scene_new("Main Scene");

    // create camera, move to (0, 3, -4)
    camera = ar_camera_perspective_new();
    camera.position = (ar_vector3f_t){0, 3, -4};
    ar_camera_fov(&camera, 100);

    shadow_cam = ar_camera_perspective_new();
    shadow_cam.position = camera.position;
    ar_camera_fov(&shadow_cam, 160);

    // create new object and load asset
    level = ar_object_new("Level");
    ar_asset_t *level_asset = ar_asset_load(AR_ASSET_MESH, "../models/reception/reception.obj");

    // attach asset to object, attach object to scene
    ar_object_attach(level, AR_OBJECT_ATTACH_MESH, level_asset);
    ar_scene_attach(scene, AR_SCENE_ATTACH_OBJECT, level);

    // create new object and load asset
    cube = ar_object_new("Cube");
    ar_asset_t *cube_asset = ar_asset_load(AR_ASSET_MESH, "../models/cube.obj");
    
    // attach asset to object, attach object to scene
    ar_object_attach(cube, AR_OBJECT_ATTACH_MESH, cube_asset);
    ar_object_move(cube, 1, 5, 0);
    ar_scene_attach(scene, AR_SCENE_ATTACH_OBJECT, cube);

    // create new light, attach to scene
    ar_light_t *light = ar_light_new(AR_LIGHT_POINT);
    light->position = (ar_vector3f_t){1, 5, 0};
    light->diffuse = (ar_vector3f_t){1.0, 1.0, 0.6};
    ar_scene_attach(scene, AR_SCENE_ATTACH_LIGHT, light);

    // select this scene as our default
    ar_scene_select(scene, ar_shaderman_get_render_shader());
}

void fps_move(ar_camera_t *camera) {
    float speed = 7.0f*ar_time_get_delta();

    // comment out this line for flycam
    //camera->direction.y = 0;

    if (ar_control_check(SDLK_w)) {
        ar_vector_mul_value(AR_VEC3F, &camera->direction, speed, &camera->direction);
        ar_vector_add(AR_VEC3F, &camera->position, &camera->direction, &camera->position);
    }
    if (ar_control_check(SDLK_s)) {
        ar_vector_mul_value(AR_VEC3F, &camera->direction, speed, &camera->direction);
        ar_vector_sub(AR_VEC3F, &camera->position, &camera->direction, &camera->position);
    }
    if (ar_control_check(SDLK_d)) {
        ar_vector_mul_value(AR_VEC3F, &camera->right, speed, &camera->right);
        ar_vector_add(AR_VEC3F, &camera->position, &camera->right, &camera->position);
    }
    if (ar_control_check(SDLK_a)) {
        ar_vector_mul_value(AR_VEC3F, &camera->right, speed, &camera->right);
        ar_vector_sub(AR_VEC3F, &camera->position, &camera->right, &camera->position);
    }
}

void save_image(ar_texture_t *texture) {
    FILE *fp = fopen("atlas.jpg", "wb");
    ari_jpeg_t jpeg;
    jpeg.width = texture->width;
    jpeg.height = texture->height;
    jpeg.pitch = 0;
    jpeg.channels = 4;
    jpeg.pixel_format = ARI_RGBA;

    void *data = ar_texture_get_data(texture);

    jpeg.data = data;

    ari_jpeg_save(fp, &jpeg, ARI_RGBA, ARI_SAMP_444, 80, ARI_TJFLAG_FASTDCT);
    fclose(fp);

    ar_memory_free(data);
}

int main() {
    ar_instance_t *instance = ar_instance_new(AR_INSTANCE_GRAPHICS | AR_INSTANCE_ASYNC_ASSETS);
    ar_window_t *window = ar_window_new("Acheron3d FPS Test", 1024, 768, 0);
    
    ar_window_option_set(window, AR_WINDOW_OPTION_MOUSE_VISIBLE, 0);
    ar_instance_attach(instance, AR_INSTANCE_ATTACH_WINDOW, window);

    ar_init(instance);

    init_object_stuffs();

    ar_texture_t *texture = ar_texture_new(AR_TEXTURE_MIPMAP);
    texture->image = *ar_asset_get_image(ar_asset_load(AR_ASSET_IMAGE, "../images/brick.jpg"));
    ar_texture_update(texture);

    ar_material_t *material = ar_material_new();
    material->diffuse_texture = texture;
    //material->specular_texture = NULL;
    //material->normal_texture = NULL;
    material->shininess = 32.0f;

    ar_object_attach(cube, AR_OBJECT_ATTACH_MATERIAL, material);

    // handle camera rotations
    ar_handle_set(AR_HANDLE_MOUSE_MOVE, &handle_mouse);

    ar_texture_t *atlas = ar_texture_new(0);
    atlas->data_width = AR_TEXTURE_BYTE;
    atlas->draw_type = AR_TEXTURE_RGBA;
    atlas->data_type = AR_TEXTURE_RGBA;
    atlas->bind_type = AR_TEXTURE_2D;

    ar_texture_bind(atlas);
    ar_texture_set_data(atlas, 512, 512, AR_TEXTURE_RGBA, AR_TEXTURE_BYTE, NULL);

    material->diffuse_texture = atlas;

    ar_control_set_mode(SDLK_ESCAPE, AR_CONTROL_MODE_TOGGLE);
    ar_control_set_mode(SDLK_t, AR_CONTROL_MODE_TOGGLE);

    ar_framebuffer_t shadow_fb = ar_framebuffer_new(512, 512);
    ar_texture_t *shadow_tex = ar_texture_new(0);
    ar_texture_set_data(shadow_tex, 512, 512, AR_TEXTURE_RGBA, AR_TEXTURE_BYTE, NULL);

    while (instance->running) {
        ar_time_tick();
        ar_controls_poll_events();

        ar_shader_use(ar_shaderman_get_render_shader());

        if (ar_control_check(SDLK_q))
            instance->running = false;
        if (ar_control_check(SDLK_ESCAPE)) {
            ar_window_option_set(window, AR_WINDOW_OPTION_MOUSE_VISIBLE, !ar_window_option_get(window, AR_WINDOW_OPTION_MOUSE_VISIBLE));
        }

        fps_move(&camera);
        ar_camera_update(&camera);

        ar_framebuffer_bind(&shadow_fb);
        ar_renderer_draw(&shadow_cam);

        // bind default framebuffer, render from our player camera perspective
        ar_framebuffer_bind(NULL);
        ar_renderer_draw(&camera);

        if (ar_control_check(SDLK_t)) {
            //glReadPixels(window->width/2-128, window->height/2-128, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE, dfb);
            ar_framebuffer_to_texture(&shadow_fb, shadow_tex, AR_FRAMEBUFFER_COLOR0);

            void *data = ar_texture_get_data(shadow_tex);
            ar_texture_sub_image(atlas, 0, 0, 512, 512, data);
            ar_memory_free(data);
            ar_log(AR_LOG_INFO, "Read pixels into subimg\n", 0);
        }

        ar_window_buffers_swap(window);
    }
    save_image(atlas); 
    ar_texture_destroy(texture);

    ar_camera_destroy(&camera);
    ar_window_destroy(window);
    ar_scene_destroy(scene);

    ar_instance_destroy();
    ar_cleanup();

    return 0;
}