#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/engine.h>
#include <f3d/engine/limits.h>

#include <f3d/engine/type/vec.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define SHADER_NONE -1

static shader_t shaders[MAX_SHADERS];
static int shaders_index = 0;

const char *get_shader_type_name(int type) {
    if (type == SHADER_NONE)
        return "None";
    else if (type == SHADER_FRAGMENT)
        return  "Fragment";
    else if (type == SHADER_VERTEX)
        return "Vertex";
    else if (type == SHADER_GEOMETRY)
        return "Geometry";
        
    return "Unknown";
}

void check_status(unsigned id, int type) {
    int info_log_length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
	    char info_log[info_log_length];
	    glGetShaderInfoLog(id, info_log_length, NULL, info_log);
	    log_msg(LOG_ERROR, "Error in %s shader! (%d)\n", get_shader_type_name(type), info_log_length);
	    log_msg(LOG_ERROR, "Error log:\n%s\n", info_log);
	}
}

bool shader_check_linked(shader_t *shader) {
    return (shader->program != SHADER_NONE);
}

static void link_shader_check(shader_t *shader) {
    if (!shader_check_linked(shader))
        shader_link(shader);
} 

void shader_use(shader_t *shader) {
    link_shader_check(shader);
    glUseProgram(shader->program);
}

void shader_link(shader_t *shader) {
    // shader already linked
    if (shader->program != SHADER_NONE)
        return;
        
    shader->program = glCreateProgram();
    
    // attach shaders for linking
    if (shader->vertex != SHADER_NONE)
        glAttachShader(shader->program, shader->vertex);
        
    if (shader->fragment != SHADER_NONE)
        glAttachShader(shader->program, shader->fragment);

    if (shader->geometry != SHADER_NONE)
        glAttachShader(shader->program, shader->geometry);

    glLinkProgram(shader->program);
    
    // detach shaders from program
    if (shader->vertex != SHADER_NONE) {
        glDetachShader(shader->program, shader->vertex);
        glDeleteShader(shader->vertex);
    }
    
    if (shader->fragment != SHADER_NONE) {
        glDetachShader(shader->program, shader->fragment);
        glDeleteShader(shader->fragment);
    }
    
    if (shader->geometry != SHADER_NONE) {
        glDetachShader(shader->program, shader->geometry);
        glDeleteShader(shader->geometry);
    }

    int is_linked = 0;
    glGetProgramiv(shader->program, GL_LINK_STATUS, &is_linked);
    if (!is_linked) {
        // free our program memory in OpenGL if the link has failed
        glDeleteProgram(shader->program);
        shader->program = SHADER_NONE;
        return;
    }
    
    handle_call(HANDLE_ON_SHADER_LOAD, shader);
}

void shader_destroy(shader_t *shader) {
    if (shader->program != SHADER_NONE)
        glDeleteProgram(shader->program);
}

static long load_shader(const char *path, int type) {
    unsigned shader_id = glCreateShader(type);
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        log_msg(LOG_ERROR, "%s shader could not be opened\n", get_shader_type_name(type));
        return SHADER_NONE;
    }
    // seek file pointer to end of file, get pointer location
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    
    // allocate +1 for null character
    char *data = malloc(fsize+1);
    // read shader file into buffer
    fread(data, 1, fsize, fp);
    // set terminating null character
    data[fsize] = 0;
    fclose(fp);
    
    glShaderSource(shader_id, 1, (const char **)&data, NULL);
    glCompileShader(shader_id);
    free(data);
    check_status(shader_id, type);
    return shader_id;
}

shader_t *shader_get(const char *name) {
    int i;
    hash_t hash = util_hash_str(name);
    for (i = 0; i < shaders_index; i++) {
        if (hash == shaders[i].hash)
            return &shaders[i];
    }
    return NULL;
}

shader_t *shader_new(const char *name) {
    int index = shaders_index++;
    shader_t *shader = &shaders[index];
    shader->hash = util_hash_str(name);
    
    shader->vertex = SHADER_NONE;
    shader->fragment = SHADER_NONE;
    shader->geometry = SHADER_NONE;
    shader->program = SHADER_NONE;
    shader->uniform_index = 0;
    
    strcpy(shader->name, name);
    return shader;
}

void shader_attach(shader_t *shader, int type, const char *path) {
    if (type == SHADER_VERTEX) {
        shader->vertex = load_shader(path, type);
    }
    else if (type == SHADER_FRAGMENT) {
        shader->fragment = load_shader(path, type);
    }
    else if (type == SHADER_GEOMETRY) {
        shader->geometry = load_shader(path, type);
    }
    else {
        log_msg(LOG_ERROR, "Unsupported shader type\n", 0);
    }
}

unsigned shader_get_uniform_location(shader_t *shader, const char *var, int *uniform_index) {
    link_shader_check(shader);
    
    hash_t hash = util_hash_str(var);
    int i;
    for (i = 0; i < shader->uniform_index; i++) {
        // compare hash to cached uniforms
        if (shader->uniforms[i].hash == hash) {
            // we found the hash, so we set the index
            if (uniform_index != NULL)
                (*uniform_index) = i;
            // uniform has already been cached, so we return the location
            return shader->uniforms[i].location;
        }
    }
    // uniform not found, add to uniform buffer in shader
    shader_uniform_t new_uniform;
    new_uniform.hash = hash;
    new_uniform.location = glGetUniformLocation(shader->program, var);
    
    // cache the location
    if (shader->uniform_index != MAX_SHADER_UNIFORMS)
        shader->uniforms[shader->uniform_index++] = new_uniform;
        
    // set uniform_index to the index of the new cached object
    if (uniform_index != NULL)
        (*uniform_index) = shader->uniform_index-1;
    
    new_uniform.value = 0.0f;
    return new_uniform.location;
}

void shader_set_mat4(shader_t *shader, const char *var, mat4_t *mat) {
    const unsigned location = shader_get_uniform_location(shader, var, NULL);
    glUniformMatrix4fv(location, 1, GL_FALSE, mat->val);
    const char *errmsg = engine_get_opengl_error();
    if (errmsg != NULL)
        log_msg(LOG_ERROR, "OpenGL error: %s: %s\n", var, errmsg);
}

void shader_set_vec3f(shader_t *shader, const char *var, vector3f_t vec) {
    const unsigned location = shader_get_uniform_location(shader, var, NULL);
    glUniform3f(location, vec.x, vec.y, vec.z);
    const char *errmsg = engine_get_opengl_error();
    if (errmsg != NULL)
        log_msg(LOG_ERROR, "OpenGL error: %s: %s\n", var, errmsg);
}

void shader_set_float(shader_t *shader, const char *var, float val) {
    int uniform_i;
    
    const unsigned location = shader_get_uniform_location(shader, var, &uniform_i);
    // check if uniform is already set to value
    if (shader->uniforms[uniform_i].value == val)
        return;
    shader->uniforms[uniform_i].value = val;
    
    glUniform1f(location, val);
    const char *errmsg = engine_get_opengl_error();
    if (errmsg != NULL)
        log_msg(LOG_ERROR, "OpenGL error: %s: %s\n", var, errmsg);
}

void shader_set_int(shader_t *shader, const char *var, int val) {
    int uniform_i;
    
    const unsigned location = shader_get_uniform_location(shader, var, &uniform_i);
    // check if uniform is already set to value
    if (shader->uniforms[uniform_i].value == val)
        return;
    shader->uniforms[uniform_i].value = val;
    
    glUniform1i(location, val);
    const char *errmsg = engine_get_opengl_error();
    if (errmsg != NULL)
        log_msg(LOG_ERROR, "OpenGL error: %s: %s\n", var, errmsg);
}

