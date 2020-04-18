#ifndef F3D_SHADER_H
#define F3D_SHADER_H

#include <f3d/engine/types.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define SHADER_VERTEX   GL_VERTEX_SHADER
#define SHADER_FRAGMENT GL_FRAGMENT_SHADER

#define MAX_UNIFORMS 512

typedef struct {
    char name[32];
    unsigned id;
} shader_t;

unsigned shader_load(const char *path, int type);
shader_t shaders_link(const char *name, unsigned shader0, unsigned shader1);
void shader_use(shader_t *shader);
void shader_destroy(shader_t *shader);

void shader_set_int(shader_t *shader, const char *var, int val);
void shader_set_float(shader_t *shader, const char *var, float val);
void shader_set_mat4(shader_t *shader, const char *var, mat4_t *mat);
void shader_set_vec3f(shader_t *shader, const char *var, vector3f_t vec);

#endif
