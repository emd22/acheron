#ifndef F3D_LIGHT_H
#define F3D_LIGHT_H

#include <f3d/engine/types.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/shadows.h>

#include <stdbool.h>

typedef enum {
    AR_LIGHT_DUMMY,
    AR_LIGHT_DIRECTIONAL,
    AR_LIGHT_POINT,
    AR_LIGHT_SPOTLIGHT,
} ar_light_type_t;

typedef struct {
    ar_light_type_t type;
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
} ar_light_t;

ar_light_t *ar_light_new(ar_light_type_t type);
ar_light_t *ar_light_get(ar_buffer_t *lights, ar_light_t *next, ar_light_type_t type);
void ar_light_init(ar_light_t *light, ar_shader_t *shader);
void ar_light_update(ar_light_t *light, ar_shader_t *shader);

void ar_light_shadow_new(ar_light_t *light, int width, int height);
void ar_light_shadow_render(ar_light_t *light, ar_shader_t *shader_main);

#endif
