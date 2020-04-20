#ifndef F3D_MATERIAL_H
#define F3D_MATERIAL_H

#define MATERIALS_MAX 128

#include <f3d/engine/image/texture.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/util.h>

#include <stdbool.h>

#define MATERIAL_UPDATE         0x01
#define MATERIAL_NO_DIFFUSE     0x02
#define MATERIAL_NO_SPECULARMAP 0x04
#define MATERIAL_NO_NORMALMAP   0x10

enum {
    MATERIAL_DIFFUSE,
    MATERIAL_SPECULARMAP,
    MATERIAL_NORMALMAP,
};

typedef struct {
    char name[32];
    hash_t hash;
    
    texture_t *diffuse;
    texture_t *specular;
    texture_t *normal;
    
    float shininess;
    int flags;
} material_t;

material_t *material_new(material_t material);
void material_update(material_t *mat, shader_t *shader);
material_t *material_get(const char *name);
void material_destroy(material_t *material);

#endif
