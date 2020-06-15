#ifndef F3D_SHADER_H
#define F3D_SHADER_H

#include <f3d/engine/types.h>
#include <f3d/engine/util.h>
#include <f3d/engine/limits.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdbool.h>

#define SHADER_VERTEX   GL_VERTEX_SHADER
#define SHADER_FRAGMENT GL_FRAGMENT_SHADER
#define SHADER_GEOMETRY GL_GEOMETRY_SHADER

typedef enum {
    AR_SHADER_MAIN,
    AR_SHADER_SHADOW,
    AR_SHADER_SKYBOX,
    AR_SHADER_UI,
    AR_SHADER_DEBUG,
    
    AR_SHADER_TABLE_SIZE,
} ar_shader_table_index_t;

typedef struct {
    hash_t hash;
    unsigned location;
    float value;
} shader_uniform_t;

typedef struct {
    char name[32];
    hash_t hash;
    
    shader_uniform_t uniforms[MAX_SHADER_UNIFORMS];
    int uniform_index;
    
    long vertex, fragment, geometry;
    long program;
} ar_shader_t;

ar_shader_t *ar_shader_new(const char *name);
void ar_shader_attach(ar_shader_t *shader, int type, const char *path);
ar_shader_t *ar_shader_get(const char *name);
void ar_shader_use(ar_shader_t *shader);
void ar_shader_link(ar_shader_t *shader);
bool ar_shader_check_linked(ar_shader_t *shader);
void ar_shader_destroy(ar_shader_t *shader);

void ar_shader_set_int(ar_shader_t *shader, const char *var, int val);
void ar_shader_set_float(ar_shader_t *shader, const char *var, float val);
void ar_shader_set_mat4(ar_shader_t *shader, const char *var, mat4_t *mat);
void ar_shader_set_vec3f(ar_shader_t *shader, const char *var, ar_vector3f_t vec);

#endif
