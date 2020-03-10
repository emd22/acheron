#include <gl/shader.h>
#include <gl/log.h>

#include <gl/type/vec.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

void check_status(unsigned id) {
    int info_log_length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
	    char info_log[2048];
	    glGetShaderInfoLog(id, info_log_length, NULL, info_log);
	    log_msg(LOG_WARN, "info_log_length > 0! (%d)\n", info_log_length);
	    log_msg(LOG_ERROR, "Info Log:\n%s\n", info_log);
	}
}

unsigned shaders_link(unsigned shader0, unsigned shader1) {
    log_msg(LOG_INFO, "Linking shaders\n", 0);
    unsigned id = glCreateProgram(); // Program ID
    // attach shaders for linking
    glAttachShader(id, shader0);
    glAttachShader(id, shader1);

    glLinkProgram(id);
    
    // detach shaders from program
    glDetachShader(id, shader0);
    glDetachShader(id, shader1);
    // delete child shaders
    glDeleteShader(shader0);
    glDeleteShader(shader1);
    
    return id;
}

char *filename_from_path(char *str) {
    int len = strlen(str);
    // start new string at end, and move to beginning
    char *newb = str+len;
    // break loop if string hits beginning or hits last slash
    while (newb != str && *(--newb) != '/');
    // add one to remove slash at beginning of filename
    return newb+1;
}


unsigned shader_load(const char *path, int type) {
    unsigned id = glCreateShader(type);
    const char *typestrs[] = {
        "Vertex", "Fragment"
    };
    log_msg(LOG_INFO, "Loading shader '%s' of type '%s'\n", filename_from_path((char *)path), (type == GL_VERTEX_SHADER) ? typestrs[0] : typestrs[1]);
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        log_msg(LOG_ERROR, "Shader could not be opened\n", 0);
        return 0;
    }
    // seek file pointer to end of file, get pointer location
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    
    char *data = malloc(fsize+1);
    // read shader file into buffer
    fread(data, 1, fsize, fp);
    // set terminating null character
    data[fsize] = 0;
    fclose(fp);
    
    log_msg(LOG_INFO, "Compiling shader\n", 0);
    
    glShaderSource(id, 1, (const char **)&data, NULL);
    glCompileShader(id);
    free(data);
    
    check_status(id);
    log_msg(LOG_INFO, "Done\n", 0);
    return id;
}

void shader_set_mat4(unsigned shaderid, const char *var, mat4_t *mat) {
    glUniformMatrix4fv(glGetUniformLocation(shaderid, var), 1, GL_FALSE, mat->val);
}

void shader_set_float(unsigned shaderid, const char *var, float val) {
    glUniform1f(glGetUniformLocation(shaderid, var), val);
}

void shader_set_vec3f(unsigned shaderid, const char *var, vector3f_t vec) {
    glUniform3f(glGetUniformLocation(shaderid, var), vec.x, vec.y, vec.z);
}

void shader_set_int(unsigned shaderid, const char *var, int val) {
    glUniform1i(glGetUniformLocation(shaderid, var), val);
}

