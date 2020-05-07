#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>

#include <f3d/engine/engine.h>

#include <f3d/game/game.h>
#include <f3d/game/player.h>

#include <GL/glew.h>
#include <GL/gl.h>


#define FRAMERATE_CAP 60

#define MOUSE_SPEED 0.07

void load_models();
int on_draw(void *arg);
void check_mouse(double xrel, double yrel);
void check_event(SDL_Event *event);
int on_end(void *arg);

window_t window;
material_t *brick, *stone;
light_t *light;
camera_t camera;

scene_t *scene;

int init(void) {
    game_init();
    game_info.flags |= GAME_IS_RUNNING;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_msg(LOG_FATAL, "Could not start SDL\n", 0);
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    window = window_new("Ethan's 3D Engine", 700, 700, 0);
    SDL_SetWindowFullscreen(window.win, SDL_WINDOW_FULLSCREEN_DESKTOP); 
    SDL_GetWindowSize(window.win, &window.width, &window.height);
    default_window = &window;
    
    render_init();
    
    camera = camera_new();
    camera.move_speed = 6.0f;
    camera.position = (vector3f_t){0, 3, 4};
    camera.rotation.x = 3.14;
    // select camera to be default and calculate perspective matrix
    camera_select(&camera);
    log_msg(LOG_INFO, "Camera initialized\n", 0);

    load_models();
    
    ui_init();
    
    // disable mouse cursor
    window_set_mouse_mode(WINDOW_MOUSE_DISABLED);
    
    return 0;
}


void setup_handles() {
    handle_set(HANDLE_END, &on_end);
    handle_set(HANDLE_DRAW, &on_draw);
}

int main() {
    init();
    setup_handles();
    handle_call(HANDLE_INIT, NULL);
    
    //light_t *light2 = light_new(LIGHT_POINT);
    //light2->position = (vector3f_t){-1, 2, -5};
    //light2->diffuse   = VEC3F(0.3f);
    //light2->diffuse.x = 1.0f;
    //light_shadow_new(light2, 800, 800);
    //light_init(light2, shader_main);
    
    light = light_new(LIGHT_POINT);
    light->position = (vector3f_t){5, 3, 4};
    light_shadow_new(light, 800, 800);
    light_init(light, shader_main);
    
    scene = scene_new("Scene");
    scene_attach(scene, SCENE_LIGHT, light);
    //scene_attach(scene, SCENE_LIGHT, light2);
    selected_scene = scene;
    scene_render_shadows(scene, shader_main);

    engine_setup_signals();
   
    SDL_Event event;
    time_init(); 
   
    log_msg(LOG_INFO, "Buffer usage: %.01fKB\n", (double)buffer_total_used/1024.0);
    while (game_info.flags & GAME_IS_RUNNING) {
        time_tick();
        while (SDL_PollEvent(&event))
            check_event(&event);
            
        shader_use(shader_main);
        player_move(&camera);
        
        camera_update(selected_camera);
        shader_set_vec3f(shader_main, "view_pos", selected_camera->position);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //render_all();
        handle_call(HANDLE_DRAW, NULL);
        
        window_buffers_swap(&window);

        time_end();
        //SDL_Delay((1000/FRAMERATE_CAP-delta_time));
    }
    handle_call(HANDLE_END, NULL);
    return 0;
}

int on_end(void *arg){
    (void)arg;
    //shader_destroy(shader_depth);
    //shader_destroy(shader_main);
    render_destroy();
    window_destroy(&window);
    
    log_msg(LOG_INFO, "Buffer usage at program end: %llu\n", buffer_total_used);
    //os_print_backtrace();
    
    SDL_Quit();
    exit(0);
}

void load_models() {
    stone = material_new((material_t){
        "Stone", 0,
        texture_load(NULL, "../images/stone.bmp", IMAGE_BMP),
        NULL,
        texture_load(NULL, "../images/stone_normal.bmp", IMAGE_BMP),
        32.0f, 0
    });
    
    brick = material_new((material_t){
        "Brick", 0,
        texture_load(NULL, "../images/brick.bmp", IMAGE_BMP),
        NULL,
        texture_load(NULL, "../images/brick_normal.bmp", IMAGE_BMP),
        50.0f, 0
    });
    
    //render_object_t *level = object_new("Level");
    //object_attach(level, OBJECT_ATTACH_MESH, mesh_load(NULL, "../models/conference/conference.obj", MODEL_OBJ, 0));
    //object_attach(level, OBJECT_ATTACH_MATERIAL, brick);
    //object_rotate(level, 1.57, 0.0f, 0.0f);
    //object_move(level, 0, 2, -5);
    //object_scale(level, 0.01, 0.01, 0.01);

    //render_object_t *wall = object_new("Wall");
    //object_attach(wall, OBJECT_ATTACH_MESH, level->mesh);
    //object_attach(wall, OBJECT_ATTACH_MATERIAL, stone);

    render_object_t *box = object_new("Box");
    object_attach(box, OBJECT_ATTACH_MESH, mesh_load(NULL, "../models/basiccube.obj", MODEL_OBJ, 0));
    object_attach(box, OBJECT_ATTACH_MATERIAL, stone);
    object_move(box, 0, 2, 0);
    
    objects_sort();
    default_framebuffer = NULL;
}
 
int on_draw(void *arg) {
    (void)arg;
    
    render_all();
    
    return 0;
}

void check_mouse(double xrel, double yrel) {
    if (!(game_info.controls.flags & CONTROLS_MOUSE_CAPTURED))
        return;
    selected_camera->rotation.x -= delta_time*(MOUSE_SPEED*xrel);
    selected_camera->rotation.y -= delta_time*(MOUSE_SPEED*yrel);
}

void check_event(SDL_Event *event) {
    if (event->type == SDL_QUIT) {
        game_info.flags &= ~GAME_IS_RUNNING;
    }
    else if (event->type == SDL_KEYDOWN) {
        controls_handle_keydown(event->key.keysym.sym);
    }
    else if (event->type == SDL_KEYUP) {
        controls_handle_keyup(event->key.keysym.sym);
    }
    else if (event->type == SDL_MOUSEMOTION) {
        check_mouse(event->motion.xrel, event->motion.yrel);
    }
}
