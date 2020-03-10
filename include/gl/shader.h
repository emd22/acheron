#ifndef C3D_SHADER_H
#define C3D_SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <gl/type/mat.h>

#define SHADER_VERTEX   GL_VERTEX_SHADER
#define SHADER_FRAGMENT GL_FRAGMENT_SHADER

unsigned shader_load(const char *path, int type);
unsigned shaders_link(unsigned shader0, unsigned shader1);

void shader_set_int(unsigned shaderid, const char *var, int val);
void shader_set_float(unsigned shaderid, const char *var, float val);
void shader_set_mat4(unsigned shaderid, const char *var, mat4_t *mat);
void shader_set_vec3f(unsigned shaderid, const char *var, vector3f_t vec);

#endif
