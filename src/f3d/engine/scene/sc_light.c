#include <f3d/engine/scene/sc_light.h>
#include <f3d/engine/renderer/rr_shader.h>
#include <f3d/engine/core/cr_log.h>

#include <f3d/engine/rendering/render.h>

#include <stdio.h>
#include <string.h>

static ar_buffer_t lights;
static ar_light_t dummy;
static int shadow_map_id = 0;

int count_light_types(ar_light_type_t type) {
    unsigned i;
    int amt = 0;
    ar_light_t *light;
    for (i = 0; i < lights.index-1; i++) {
        light = ar_buffer_get(&lights, i);
        if (light->type == type)
            amt++;
    }
    return amt;
}

ar_light_t *ar_light_get(ar_buffer_t *lights, ar_light_t *next, ar_light_type_t type) {
    unsigned i;
    ar_light_t *light;
    for (i = 0; i < lights->index; i++) {
        light = ar_buffer_get(lights, i);
        if (light != next && light->type == type) {
            return light;
        }
    }
    return NULL;
}

ar_light_t *ar_light_new(ar_light_type_t type) {
    if (!ar_buffer_is_initialized(&lights)) {
        ar_buffer_init(&lights, AR_BUFFER_STATIC, sizeof(ar_light_t), MAX_LIGHTS, 0);
    }
    
    memset(&dummy, 0, sizeof(ar_light_t));
    dummy.type = AR_LIGHT_DUMMY;
    
    if (lights.index == MAX_LIGHTS) {
        return &dummy;
    }
    ar_light_t *light = ar_buffer_new_item(&lights);
    // make sure all light data is NULL'd
    memset(light, 0, sizeof(ar_light_t));
    // count amount of a type of light because of the shader having
    // multiple arrays for different lights
    light->index = count_light_types(type);
    light->type = type;
    light->use_shadows = false;
    
    if (type == AR_LIGHT_POINT) {
        light->position = VEC3F(0.0f);
        light->ambient = VEC3F(0.10f);
        light->diffuse   = VEC3F(0.8f);
        light->specular  = VEC3F(1.0f);
        light->radius = 8.0f;
    }
    else if (type == AR_LIGHT_DIRECTIONAL) {
        light->direction = (ar_vector3f_t){-0.2, 0.8, -0.7};
        light->ambient   = (ar_vector3f_t){0.02f, 0.02f, 0.02f};
        light->diffuse   = (ar_vector3f_t){0.15f, 0.15f, 0.15f};
        light->specular  = (ar_vector3f_t){0.3f, 0.3f, 0.3f};
    }
    
    return light;
}

void ar_light_shadow_new(ar_light_t *light, int width, int height) {
    if (light == NULL) {
        ar_log(AR_LOG_ERROR, "light == NULL or light is not point light\n", 0);
        return;
    }
    if (!light->initialized)
        ar_light_init(light, shader_main);
        
    light->point_shadow = shadows_point_init(light->position, width, height, light->radius);
    light->use_shadows = true;
    light->point_shadow.shadow_map_id = ++shadow_map_id;
}

void ar_light_shadow_render(ar_light_t *light, ar_shader_t *shader_main) {
    if (light->use_shadows == false)
        return;
    shadows_point_render(&light->point_shadow, light->position, shader_main);
}


// TODO: replace this with something better
void ar_light_init(ar_light_t *light, ar_shader_t *shader) {
    if (light->initialized)
        return;
        
    if (light->type == AR_LIGHT_DUMMY)
        return;
        
    light->initialized = true;
        
    char lightstr[48];
    if (light->type == AR_LIGHT_DIRECTIONAL) {
        ar_log(AR_LOG_INFO, "Initializing directional light (id: %d)\n", light->index);
        sprintf(lightstr, "dirLights[%d].direction", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->direction);

        sprintf(lightstr, "dirLights[%d].ambient", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->ambient);

        sprintf(lightstr, "dirLights[%d].diffuse", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->diffuse);

        sprintf(lightstr, "dirLights[%d].specular", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->specular);
    }
    else if (light->type == AR_LIGHT_POINT) {
        ar_log(AR_LOG_INFO, "Initializing point light (id: %d)\n", light->index);
        
        sprintf(lightstr, "pointLights[%d].position", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->position);

        sprintf(lightstr, "pointLights[%d].ambient", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->ambient);

        sprintf(lightstr, "pointLights[%d].diffuse", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->diffuse);

        sprintf(lightstr, "pointLights[%d].specular", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->specular);

        sprintf(lightstr, "pointLights[%d].radius", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_FLOAT, lightstr, &light->radius);
        
        // Shadows
        sprintf(lightstr, "pointLights[%d].shadow_map", light->index);
        const int temp1 = 4+light->point_shadow.shadow_map_id;
        ar_shader_set_uniform(shader, AR_SHADER_INT, lightstr, &temp1);
        
        sprintf(lightstr, "pointLights[%d].shadow_far_plane", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_FLOAT, lightstr, &light->point_shadow.far_plane);
        
        sprintf(lightstr, "pointLights[%d].shadows_enabled", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_FLOAT, lightstr, &light->use_shadows);
    }
    else {
        ar_log(AR_LOG_ERROR, "light type #%d not implemented\n", light->type);
    }
    ar_light_update(light, shader_main);    
}

void ar_light_update(ar_light_t *light, ar_shader_t *shader) {
    if (light == NULL || shader == NULL)
        return;
        
    char lightstr[32];
    if (light->type == AR_LIGHT_DIRECTIONAL) {
        sprintf(lightstr, "dirLights[%d].direction", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->direction);
    }
    else if (light->type == AR_LIGHT_POINT) {
        light->point_shadow.far_plane = light->radius;
        sprintf(lightstr, "pointLights[%d].position", light->index);
        ar_shader_set_uniform(shader, AR_SHADER_VEC3F, lightstr, &light->position);
        
        if (light->use_shadows) {
            sprintf(lightstr, "pointLights[%d].shadow_map", light->index);
            const int temp0 = 4+light->point_shadow.shadow_map_id;
            ar_shader_set_uniform(shader, AR_SHADER_INT, lightstr, &temp0);
            
            sprintf(lightstr, "pointLights[%d].shadow_far_plane", light->index);
            ar_shader_set_uniform(shader, AR_SHADER_FLOAT, lightstr, &light->point_shadow.far_plane);
            
            sprintf(lightstr, "pointLights[%d].shadows_enabled", light->index);
            ar_shader_set_uniform(shader, AR_SHADER_FLOAT, lightstr, &light->use_shadows);
            
            shadows_point_update(&light->point_shadow, light->position);
            ar_light_shadow_render(light, shader);
        }
    }
}

void ar_light_destroy(ar_light_t *light) {
    if (light->use_shadows) {
        shadows_destroy(&light->point_shadow);
    }
}

void ar_light_buffer_destroy(void) {
    ar_buffer_destroy(&lights);
}
