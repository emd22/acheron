#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>

#include <f3d/engine/engine.h>
#include <f3d/engine/types.h>

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
light_t *light, *player_light;
camera_t camera;
render_object_t *box;

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
    
    
    const bool fullscreen = true;
    
    int width = 500;
    int height = 500;
    
    if (fullscreen) {
        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(0, &mode);
        width = mode.w;
        height = mode.h;
    }
    window = window_new("Ethan's 3D Engine", width, height, 0);
    if (fullscreen)    
        SDL_SetWindowFullscreen(window.win, SDL_WINDOW_FULLSCREEN_DESKTOP);
    default_window = &window;

    render_init();
    camera = camera_new();
    camera.move_speed = 6.0f;
    camera.position = (vector3f_t){0, 3, 4};
    camera.rotation.x = 3.14f;
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
    
    
    //light = light_new(LIGHT_POINT);
    //light->position = (vector3f_t){5, 3, 4};
    //light_shadow_new(light, 800, 800);
    //light_init(light, shader_main);
    scene = scene_new("Scene");
    
    light = light_new(LIGHT_POINT);
    light->position = (vector3f_t){-1.37f, 3.0f, -6.13f};
    light->diffuse = (vector3f_t){0.2, 1.0, 0.2};
    light_shadow_new(light, 800, 800);
    scene_attach(scene, SCENE_LIGHT, light);
    
    light_t *light2 = light_new(LIGHT_POINT);
    light2->position = (vector3f_t){1.32, 3.00f, 5.20f};
    light_shadow_new(light2, 800, 800);
    scene_attach(scene, SCENE_LIGHT, light2);
    
    //player_light = light_new(LIGHT_POINT);
    //player_light->position = camera.position;
    //player_light->position.x -= 1;
    //player_light->radius = 8.0f;
    //player_light->diffuse = (vector3f_t){1.0f, 0.2f, 0.2f};
    //light_shadow_new(player_light, 800, 800);
    //scene_attach(scene, SCENE_LIGHT, player_light);
    
    scene_select(scene, shader_main);

    engine_setup_signals();
   
    SDL_Event event;
    time_init(); 
   
    //render_object_t *level = object_get("Level");
    physics_object_t level_physics = physics_object_new(PHYSICS_COLLIDER_AABB);
    level_physics.collider.dimensions = (vector3f_t){50, 2, 50};
    level_physics.collider.position = (vector3f_t){0, -1, 0};
    
    box->physics.velocity.x = 1.0f;
    
    while (game_info.flags & GAME_IS_RUNNING) {
        time_tick();
        while (SDL_PollEvent(&event))
            check_event(&event);
            
        shader_use(shader_main);
        
        if (player_move(&camera)) {
        }
        
        physics_update(&box->physics, &level_physics);
        //if (!physics_check_collision(&box->physics, &level_physics)) {
            //box->position.y -= 0.001f;s
        //physics_update_gravity(&box->physics);
        box->position = box->physics.collider.position;
            //box->rotation = box->physics.collider.rotation;
        object_update(box);
        scene_object_update(scene, box, shader_main);
        //}
        
        camera_update(selected_camera);
        shader_set_vec3f(shader_main, "view_pos", selected_camera->position);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        handle_call(HANDLE_DRAW, NULL);
        //mesh_draw(box->collider.debug_mesh, );
        
        window_buffers_swap(&window);

        time_end();
        //SDL_Delay((1000/FRAMERATE_CAP-delta_time));
    }
    handle_call(HANDLE_END, NULL);
    return 0;
}

int on_end(void *arg){
    (void)arg;
    render_destroy();
    window_destroy(&window);
    
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
        32.0f, 0
    });
    
    render_object_t *level = object_new("Level");
    object_attach(level, OBJECT_ATTACH_MESH, mesh_load(NULL, "../models/reception/reception.obj", MODEL_OBJ, 0));
    object_attach(level, OBJECT_ATTACH_MATERIAL, brick);
    object_move(level, 0, 0, 0);

    //render_object_t *wall = object_new("Wall");
    //object_attach(wall, OBJECT_ATTACH_MESH, mesh_load(NULL, "../models/wall.obj", MODEL_OBJ, 0));
    //object_attach(wall, OBJECT_ATTACH_MATERIAL, stone);

    box = object_new("Box");
    object_attach(box, OBJECT_ATTACH_MESH, mesh_load(NULL, "../models/basiccube.obj", MODEL_OBJ, 0));
    object_attach(box, OBJECT_ATTACH_MATERIAL, stone);
    object_move(box, 0, 10, 2);
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
