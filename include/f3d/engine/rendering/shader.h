#ifndef F3D_SHADER_H
#define F3D_SHADER_H

#include <f3d/engine/types.h>
#include <f3d/engine/util.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define SHADER_VERTEX   GL_VERTEX_SHADER
#define SHADER_FRAGMENT GL_FRAGMENT_SHADER
#define SHADER_GEOMETRY GL_GEOMETRY_SHADER

#define MAX_SHADER_UNIFORMS 64
#define MAX_SHADERS 16

typedef struct {
    hash_t hash;
    unsigned location;
} shader_uniform_t;

typedef struct {
    char name[32];
    hash_t hash;
    
    shader_uniform_t uniforms[MAX_SHADER_UNIFORMS];
    int uniform_index;
    
    long vertex, fragment, geometry;
    long program;
} shader_t;

shader_t *shader_new(const char *name);
void shader_attach(shader_t *shader, int type, const char *path);
void shader_link(shader_t *shader);
shader_t *shader_get(const char *name);
void shader_destroy(shader_t *shader);
void shader_use(shader_t *shader);

void shader_set_int(shader_t *shader, const char *var, int val);
void shader_set_float(shader_t *shader, const char *var, float val);
void shader_set_mat4(shader_t *shader, const char *var, mat4_t *mat);
void shader_set_vec3f(shader_t *shader, const char *var, vector3f_t vec);

#endif
