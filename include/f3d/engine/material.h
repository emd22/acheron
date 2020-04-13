#ifndef F3D_MATERIAL_H
#define F3D_MATERIAL_H

#include <f3d/engine/texture.h>
#include <f3d/engine/shader.h>

#include <stdbool.h>

typedef struct {
    char name[32];
    texture_t *diffuse;
    texture_t *specular;
    texture_t *normal;
    
    int diffuse_id;
    int specular_id;
    int normal_id;

    bool use_normals;
    float shininess;
} material_t;

material_t *material_new(material_t material);
void material_update(material_t *mat, shader_t *shader);
material_t *material_get(const char *name);
void material_destroy(material_t *material);

#endif
