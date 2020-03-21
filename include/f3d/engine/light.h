#ifndef F3D_LIGHT_H
#define F3D_LIGHT_H

#include <f3d/engine/types.h>

#define MAX_LIGHTS 16

enum {
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
    
    // directional light
    vector3f_t direction;
    
    // point light
    vector3f_t position;
    float constant;
    float linear;
    float quadratic;
} light_t;

light_t *light_new(int type);
void light_init(light_t *light, unsigned shaderid);
void light_update(light_t *light, unsigned shaderid);

#endif
