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

static ar_point_shadows_t shadows;

static ar_shader_t *point_shadow_shader = NULL;

int handle_mouse(void *arg) {
    const float mouse_sensitivity = 0.09f;
    ar_control_mouse_event_t *event = (ar_control_mouse_event_t *)arg;
    camera.rotation.x -= ar_time_get_delta()*(mouse_sensitivity*event->relx);
    camera.rotation.y -= ar_time_get_delta()*(mouse_sensitivity*event->rely);
    return 0;
}

void gen_cubemapfb(ar_point_shadows_t *shadows, int width, int height) {
    glGenFramebuffers(1, &shadows->framebuffer.id);
    shadows->framebuffer.x = 0;
    shadows->framebuffer.y = 0;
    shadows->framebuffer.width = 512;
    shadows->framebuffer.height = 512;
    ar_framebuffer_bind(&shadows->framebuffer);
    
    shadows->framebuffer.texture = ar_texture_new(0);
    ar_texture_t *texture = shadows->framebuffer.texture;
    texture->data_width = AR_TEXTURE_FLOAT;    
    texture->bind_type = AR_TEXTURE_CUBEMAP;
    texture->draw_type = AR_TEXTURE_DEPTH;
    texture->data_type = AR_TEXTURE_DEPTH;
    //ar_texture_set_data(texture, width, height, AR_TEXTURE_DEPTH, AR_TEXTURE_FLOAT, NULL);
    //ar_texture_bind(texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);
    glClear(GL_DEPTH_BUFFER_BIT);
    int i;
    for (i = 0; i < 6; i++) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
            0, GL_DEPTH_COMPONENT, width, height,
            0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL
        );
    }
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE); 
    //ar_framebuffer_to_texture(fb, texture, AR_FRAMEBUFFER_DEPTH);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadows->framebuffer.texture->id, 0);
    ar_texture_bind(texture);
    ar_framebuffer_bind(NULL);
}

void generate_point_vp(ar_point_shadows_t *shadows, int index, ar_camera_t *cam, ar_vector3f_t offset, ar_vector3f_t up) {
    ar_vector3f_t to;
    ar_vector_add(AR_VEC3F, &cam->position, &offset, &to);
    shadows->point_vps[index] = ar_math_lookat(cam->position, to, up);
    shadows->point_vps[index] = ar_mat4_mul(cam->projection, shadows->point_vps[index]);
}

void generate_point_vps(ar_point_shadows_t *shadows, ar_camera_t *cam) {
    generate_point_vp(shadows, 0, cam, (ar_vector3f_t){ 1.0f,  0.0f,  0.0f}, (ar_vector3f_t){0, -1,  0});
    generate_point_vp(shadows, 1, cam, (ar_vector3f_t){-1.0f,  0.0f,  0.0f}, (ar_vector3f_t){0, -1,  0});
    generate_point_vp(shadows, 2, cam, (ar_vector3f_t){ 0.0f,  1.0f,  0.0f}, (ar_vector3f_t){0,  0,  1});
    generate_point_vp(shadows, 3, cam, (ar_vector3f_t){ 0.0f, -1.0f,  0.0f}, (ar_vector3f_t){0,  0, -1});
    generate_point_vp(shadows, 4, cam, (ar_vector3f_t){ 0.0f,  0.0f,  1.0f}, (ar_vector3f_t){0, -1,  0});
    generate_point_vp(shadows, 5, cam, (ar_vector3f_t){ 0.0f,  0.0f, -1.0f}, (ar_vector3f_t){0, -1,  0});
}

ar_point_shadows_t init_point_shadows() {
    ar_point_shadows_t shadows;

    point_shadow_shader = ar_shader_new("Point Shadow");
    ar_shader_attach(point_shadow_shader, AR_SHADER_VERTEX, "../shaders/shadows/point_vert.glsl");
    ar_shader_attach(point_shadow_shader, AR_SHADER_FRAGMENT, "../shaders/shadows/point_frag.glsl");
    ar_shader_attach(point_shadow_shader, AR_SHADER_GEOMETRY, "../shaders/shadows/point_geom.glsl");
    
    shadow_cam = ar_camera_perspective_new();
    shadow_cam.position = (ar_vector3f_t){0, 3, -4};
    ar_camera_fov(&shadow_cam, 90);
    
    generate_point_vps(&shadows, &shadow_cam);
    gen_cubemapfb(&shadows, 512, 512);
    
    ar_shader_use(ar_shaderman_get_render_shader());
    
    return shadows;
}

void shadows_send_uniforms(ar_point_shadows_t *shadows, ar_camera_t *cam) {
    char str[64];
    int i;
    for (i = 0; i < 6; i++) {
        sprintf(str, "shadow_matrices[%d]", i);
        ar_shader_set_uniform(point_shadow_shader, AR_SHADER_MAT4, str, &shadows->point_vps[i]);
    }
    ar_shader_set_uniform(point_shadow_shader, AR_SHADER_VEC3F, "light_pos", &cam->position);
}

void shadows_point_render(ar_point_shadows_t *shadows, ar_camera_t *cam) {
    ar_framebuffer_bind(&shadows->framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ar_shader_t *shader_main = ar_shaderman_get_render_shader();
    ar_shader_use(shader_main);
    ar_shader_use(point_shadow_shader);
    float far_plane = 200.0f;
    ar_shader_set_uniform(point_shadow_shader, AR_SHADER_FLOAT, "far_plane", &far_plane);
    shadows_send_uniforms(shadows, cam);
    ar_renderer_draw(cam);
    ar_shader_use(shader_main);
    ar_framebuffer_bind(NULL);
}

void init_object_stuffs() {
    scene = ar_scene_new("Main Scene");

    // create camera, move to (0, 3, -4)
    camera = ar_camera_perspective_new();
    camera.position = (ar_vector3f_t){0, 3, -4};
    ar_camera_fov(&camera, 100);

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

    shadows = init_point_shadows();
    
    // create new light, attach to scene
    ar_light_t *light = ar_light_new(AR_LIGHT_POINT);
    light->position = (ar_vector3f_t){1, 5, -2};
    light->diffuse = (ar_vector3f_t){1.0, 1.0, 0.6};
    light->use_shadows = true;
    ar_light_set_point_shadows(light, &shadows);
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

    /*
    ar_texture_t *atlas = ar_texture_new(0);
    atlas->data_width = AR_TEXTURE_BYTE;
    atlas->draw_type = AR_TEXTURE_RGBA;
    atlas->data_type = AR_TEXTURE_RGBA;
    atlas->bind_type = AR_TEXTURE_2D;

    ar_texture_bind(atlas);
    ar_texture_set_data(atlas, 512, 512, AR_TEXTURE_RGBA, AR_TEXTURE_BYTE, NULL);
    */

    //material->diffuse_texture = atlas;

    ar_control_set_mode(SDLK_ESCAPE, AR_CONTROL_MODE_TOGGLE);
    ar_control_set_mode(SDLK_t, AR_CONTROL_MODE_TOGGLE); 

    //ar_framebuffer_t shadow_fb = ar_framebuffer_new(256, 256);

    while (ar_instance_is_running(instance)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ar_time_tick();
        ar_controls_poll_events();

        ar_shader_use(ar_shaderman_get_render_shader());

        if (ar_control_check(SDLK_q))
            ar_instance_stop_threads(instance);

        if (ar_control_check(SDLK_ESCAPE)) {
            ar_window_option_set(window, AR_WINDOW_OPTION_MOUSE_VISIBLE, !ar_window_option_get(window, AR_WINDOW_OPTION_MOUSE_VISIBLE));
        }
 
        fps_move(&camera);
        ar_camera_update(&camera);
        /*shadow_cam.direction = camera.direction;
        shadow_cam.position = camera.position;
        shadow_cam.rotation = camera.rotation;
        ar_camera_update(&shadow_cam);*/

        //ar_framebuffer_bind(&shadow_fb);
        //ar_renderer_draw(&shadow_cam);
        shadows_point_render(&shadows, &shadow_cam);
        // bind default framebuffer, render from our player camera perspective
        ar_framebuffer_bind(NULL);
        ar_renderer_draw(&camera);
        
        /*
        glBlitNamedFramebuffer(
            shadows.framebuffer.id,
            0,
            0, 0, 512, 512,
            0, 0, 128, 128,
            GL_COLOR_BUFFER_BIT,
            GL_LINEAR
        );*/

        /*if (ar_control_check(SDLK_t)) {
            ar_texture_bind(atlas);
            //glBindFramebuffer(GL_READ_FRAMEBUFFER, shadow_fb.id);
            glCopyTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                0, 0,
                0, 0,
                256, 256
            );
        }*/

        ar_window_buffers_swap(window);
    }
    //save_image(atlas); 
    //ar_texture_destroy(texture);

    ar_camera_destroy(&camera);
    ar_camera_destroy(&shadow_cam);
    ar_window_destroy(window);
    ar_scene_destroy(scene);

    ar_exit(instance);

    return 0;
}
