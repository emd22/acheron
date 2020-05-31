#ifndef F3D_LIGHT_H
#define F3D_LIGHT_H

#include <f3d/engine/types.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/shadows.h>

#include <stdbool.h>

enum {
    LIGHT_DUMMY,
    LIGHT_DIRECTIONAL,
    LIGHT_POINT,
    LIGHT_SPOTLIGHT,
};

typedef struct {
    int type;
    int index; // internal, for accessing shader light buffer
    
    vector3f_t ambient;
    vector3f_t diffuse;
    vector3f_t specular;
    
    shadows_point_t point_shadow;
    bool use_shadows;
    
    // directional light
    vector3f_t direction;
    
    // point light
    vector3f_t position;
    float radius;
} light_t;

light_t *light_new(int type);
light_t *light_get(ar_buffer_t *lights, light_t *next, int type);
void light_init(light_t *light, shader_t *shader);
void light_update(light_t *light, shader_t *shader);

void light_shadow_new(light_t *light, int width, int height);
void light_shadow_render(light_t *light, shader_t *shader_main);

#endif
