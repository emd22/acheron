#include <acheron/game/player.h>

#include <acheron/engine/acheron.h>
#include <acheron/engine/core/cr_time.h>
#include <acheron/engine/core/cr_controls.h>

#include <SDL2/SDL.h>

int player_move(player_t *player) {
    const float speed = 10.0f*ar_time_get_delta();
    ar_camera_t *camera = player->camera;
    
    camera->direction.y = 0;

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
    return 0;
}

/*
int player_move(ar_camera_t *camera) {
    int moved = 0;
    
    static ar_vector3f_t velocity;
    const float max_velocity = 1.0f;
    
    if (ar_control_check(SDLK_w)) {
        velocity.z = max_velocity;
        moved = 1;
    }
    if (ar_control_check(SDLK_a)) {
        velocity.x = -max_velocity;
        moved = 1;
    }
    if (ar_control_check(SDLK_s)) {
        velocity.z = -max_velocity;
        moved = 1;
    }
    if (ar_control_check(SDLK_d)) {
        velocity.x = max_velocity;
        moved = 1;
    }
    const float friction = 0.08;
    if (velocity.x) {
        if (velocity.x > 0.0f) {
            ar_camera_move(camera, CAMERA_RIGHT);
            velocity.x -= friction;
            if (velocity.x < 0.0)
                velocity.x = 0;
        }
        else {
            ar_camera_move(camera, CAMERA_LEFT);
            velocity.x += friction; 
            if (velocity.x > 0.0)
                velocity.x = 0;      
        }
        camera->move_mul.x = fabs(velocity.x);
        moved = 1;
    
    }
    if (velocity.z) {
        if (velocity.z > 0.0f) {
            ar_camera_move(camera, CAMERA_FORWARD);
            velocity.z -= friction;
            if (velocity.z < 0.0)
                velocity.z = 0;
        }
        else {
            ar_camera_move(camera, CAMERA_BACKWARD);
            velocity.z += friction;
            if (velocity.z > 0.0)
                velocity.z = 0;
        }
        camera->move_mul.z = fabs(velocity.z);
        moved = 1;
    }
    // if we move two directions at the same time, half the speed
    // so we don't move faster than normal.
    if ((camera->move_mul.x == max_velocity || camera->move_mul.x == -max_velocity) &&
        (camera->move_mul.z == max_velocity || camera->move_mul.z == -max_velocity))
    {
        camera->move_mul.x /= 2;
        camera->move_mul.z /= 2;
    }
    return moved;
}

*/

/*
int player_move(camera_t *camera) {   
    if (controls_check_toggle(CONTROL_1)) {
        light_t *new_light = light_new(LIGHT_POINT);
        new_light->position = camera->position;
        new_light->diffuse   = VEC3F(1.0f);
        light_shadow_new(new_light, 800, 800);
        light_init(new_light, shader_main);
        ar_log(AR_LOG_INFO, "New light created at (%.02f, %.02f, %.02f)\n", camera->position.x, camera->position.y, camera->position.z);
        scene_attach(selected_scene, SCENE_LIGHT, new_light);
        light_shadow_render(new_light, shader_main);
    }
    if (controls_check_toggle(CONTROL_2)) {
        static bool wireframe = false;
        wireframe = !wireframe;
        engine_render_wireframe(wireframe);

    }
    const float friction = 0.08;
    if (velocity.x) {
        if (velocity.x > 0.0f) {
            camera_move(selected_camera, CAMERA_RIGHT);
            velocity.x -= friction;
            if (velocity.x < 0.0)
                velocity.x = 0;
        }
        else {
            camera_move(selected_camera, CAMERA_LEFT);
            velocity.x += friction; 
            if (velocity.x > 0.0)
                velocity.x = 0;      
        }
        camera->move_mul.x = fabs(velocity.x);
        moved = 1;
    
    }
    if (velocity.z) {
        if (velocity.z > 0.0f) {
            camera_move(selected_camera, CAMERA_FORWARD);
            velocity.z -= friction;
            if (velocity.z < 0.0)
                velocity.z = 0;
        }
        else {
            camera_move(selected_camera, CAMERA_BACKWARD);
            velocity.z += friction;
            if (velocity.z > 0.0)
                velocity.z = 0;
        }
        camera->move_mul.z = fabs(velocity.z);
        moved = 1;
    }
    // if we move two directions at the same time, half the speed
    // so we don't move faster than normal.
    if ((camera->move_mul.x == max_velocity || camera->move_mul.x == -max_velocity) &&
        (camera->move_mul.z == max_velocity || camera->move_mul.z == -max_velocity))
    {
        camera->move_mul.x /= 2;
        camera->move_mul.z /= 2;
    }
    
    return moved;
}
*/
