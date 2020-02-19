#include <gl/shader.h>
#include <gl/log.h>

#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>

unsigned shader_load(const char *path, int type) {
    unsigned id = glCreateShader(type);
    const char *typestrs[] = {
        "Fragment", "Vertex"
    };
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        log_msg(LOG_ERROR, "Shader '%s' of type '%s' could not be opened\n", path, (type == GL_VERTEX_SHADER) ? typestrs[1] : typestrs[0]);
    }
    fseek();
}
