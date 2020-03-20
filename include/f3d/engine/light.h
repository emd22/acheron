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
    // internal index for shaders
    int index;
    vector3f_t direction;
    vector3f_t ambient;
    vector3f_t diffuse;
    vector3f_t specular;
} light_t;

light_t *light_new(int type);
void light_init(light_t *light, unsigned shaderid);

#endif
