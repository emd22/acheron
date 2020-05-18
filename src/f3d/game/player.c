#include <f3d/game/player.h>

#include <f3d/engine/engine.h>
#include <f3d/game/game.h>

int player_move(camera_t *camera) {   
    int moved = 0;
    
    static vector3f_t velocity;
    const float max_velocity = 1.0f;
    
    if (keys[CONTROL_FORWARD].pressed) {
        velocity.z = max_velocity;
        moved = 1;
    }
    if (keys[CONTROL_BACKWARD].pressed) {
        velocity.z = -max_velocity;
        moved = 1;
    }
    if (keys[CONTROL_LEFT].pressed) {
        velocity.x = -max_velocity;
        moved = 1;
    }
    if (keys[CONTROL_RIGHT].pressed) {
        velocity.x = max_velocity;
        moved = 1;
    }
    if (controls_check_toggle(CONTROL_1)) {
        light_t *new_light = light_new(LIGHT_POINT);
        new_light->position = camera->position;
        new_light->diffuse   = VEC3F(1.0f);
        //light_shadow_new(new_light, 800, 800);
        light_init(new_light, shader_main);
        
        scene_attach(selected_scene, SCENE_LIGHT, new_light);
        //scene_render_shadows(selected_scene, shader_main);
        //light_shadow_render(new_light, shader_main);
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
