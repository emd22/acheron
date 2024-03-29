#ifndef F3D_LIGHT_H
#define F3D_LIGHT_H

#include <acheron/engine/types.h>
#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/engine/rendering/shadows.h>

#include <stdbool.h>

typedef enum {
    AR_LIGHT_DUMMY,
    AR_LIGHT_DIRECTIONAL,
    AR_LIGHT_POINT,
    AR_LIGHT_SPOTLIGHT,
} ar_light_type_t;

typedef struct {
    int side_width;
    int side_height;
    
    ar_mat4_t point_vps[6];
    ar_framebuffer_t framebuffer;

    int id;
} ar_point_shadows_t;

typedef struct {
    bool initialized;
    
    ar_light_type_t type;
    int index; // internal, for accessing shader light buffer
    
    ar_vector3f_t ambient;
    ar_vector3f_t diffuse;
    ar_vector3f_t specular;
    
    ar_point_shadows_t *point_shadows;
    bool use_shadows;
    
    // directional light
    ar_vector3f_t direction;
    
    // point light
    ar_vector3f_t position;
    float radius;
} ar_light_t;

ar_light_t *ar_light_new(ar_light_type_t type);
ar_light_t *ar_light_get(ar_buffer_t *lights, ar_light_t *next, ar_light_type_t type);
void ar_light_init(ar_light_t *light, ar_shader_t *shader);
void ar_light_update(ar_light_t *light, ar_shader_t *shader);
void ar_light_destroy(ar_light_t *light);
void ar_light_buffer_destroy(void);

void ar_light_set_point_shadows(ar_light_t *light, ar_point_shadows_t *point_shadows);
void ar_light_shadow_render(ar_light_t *light, ar_shader_t *shader_main);

#endif
