#ifndef AR_OB_MATERIAL_H
#define AR_OB_MATERIAL_H

#define MATERIALS_MAX 128

//#include <acheron/engine/image/texture.h>

#include <acheron/engine/renderer/rr_texture.h>
#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/engine/util.h>

#include <stdbool.h>

typedef enum {
    AR_MATERIAL_ATTACH_DIFFUSE,
    AR_MATERIAL_ATTACH_SPECULARMAP,
    AR_MATERIAL_ATTACH_NORMALMAP,
} ar_material_attach_t;

typedef struct {
    char *name;
    hash_t hash;

    ar_texture_t *diffuse_texture;
    ar_texture_t *specular_texture;
    ar_texture_t *normal_texture;

    float shininess;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
} ar_material_t;

ar_material_t *ar_material_new(void);
void ar_material_set_active(ar_material_t *material);
void ar_material_buffer_destroy(void);

#endif
