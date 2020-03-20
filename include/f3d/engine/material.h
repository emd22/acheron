#ifndef F3D_MATERIAL_H
#define F3D_MATERIAL_H

#include <f3d/engine/texture.h>

typedef struct {
    char name[16];
    texture_t diffuse;
    texture_t specular;
    int diffuse_id;
    int specular_id;

    float shininess;
} material_t;

material_t *material_new(material_t material);
void material_update(material_t *mat, unsigned shaderid);
material_t *material_get(const char *name);
void material_destroy(material_t *material);

#endif
