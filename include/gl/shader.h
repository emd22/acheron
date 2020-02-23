#ifndef C3D_SHADER_H
#define C3D_SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

#define SHADER_VERTEX   GL_VERTEX_SHADER
#define SHADER_FRAGMENT GL_FRAGMENT_SHADER

unsigned shader_load(const char *path, int type);
unsigned shaders_link(unsigned shader0, unsigned shader1);

#endif
