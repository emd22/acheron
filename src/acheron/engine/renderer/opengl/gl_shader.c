#ifdef AR_USE_OPENGL

#include <acheron/engine/renderer/rr_renderer.h>
#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/engine/renderer/opengl/gl_shader.h>

#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/engine/limits.h>

#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/cr_handles.h>
#include <acheron/engine/core/cr_log.h>

#include <acheron/engine/types.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

const char *get_shader_type_name(int type) {
    if (type == AR_SHADER_NONE)
        return "None";
    else if (type == AR_SHADER_FRAGMENT)
        return  "Fragment";
    else if (type == AR_SHADER_VERTEX)
        return "Vertex";
    else if (type == AR_SHADER_GEOMETRY)
        return "Geometry";
        
    return "Unknown";
}

static void check_shader_status(unsigned id, int type) {
    int info_log_length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
	    char info_log[info_log_length];
	    glGetShaderInfoLog(id, info_log_length, NULL, info_log);
	    ar_log(AR_LOG_ERROR, "Error in %s shader! (%d)\n", get_shader_type_name(type), info_log_length);
	    ar_log(AR_LOG_ERROR, "Error log:\n%s\n", info_log);
	}
}

static void link_shader_check(ar_shader_t *shader) {
    if (!(shader->program != AR_SHADER_NONE))
        ar_shader_link(shader);
}

void ar_shader_use(ar_shader_t *shader) {
    link_shader_check(shader);
    glUseProgram(shader->program);
}

void ar_shader_link(ar_shader_t *shader) {
    // shader already linked
    if (shader->program != AR_SHADER_NONE)
        return;
        
    shader->program = glCreateProgram();
    
    // attach shaders for linking
    if (shader->vertex != AR_SHADER_NONE)
        glAttachShader(shader->program, shader->vertex);
        
    if (shader->fragment != AR_SHADER_NONE)
        glAttachShader(shader->program, shader->fragment);

    if (shader->geometry != AR_SHADER_NONE)
        glAttachShader(shader->program, shader->geometry);

    glLinkProgram(shader->program);
    
    // detach shaders from program
    if (shader->vertex != AR_SHADER_NONE) {
        glDetachShader(shader->program, shader->vertex);
        glDeleteShader(shader->vertex);
    }
    
    if (shader->fragment != AR_SHADER_NONE) {
        glDetachShader(shader->program, shader->fragment);
        glDeleteShader(shader->fragment);
    }
    
    if (shader->geometry != AR_SHADER_NONE) {
        glDetachShader(shader->program, shader->geometry);
        glDeleteShader(shader->geometry);
    }

    int is_linked = 0;
    glGetProgramiv(shader->program, GL_LINK_STATUS, &is_linked);
    if (!is_linked) {
        // free our program memory in OpenGL if the link has failed
        glDeleteProgram(shader->program);
        shader->program = AR_SHADER_NONE;
        return;
    }
    
    ar_handle_call(AR_HANDLE_ON_SHADER_LOAD, shader);
}

void ar_shader_destroy(ar_shader_t *shader) {
    if (shader->program != AR_SHADER_NONE)
        glDeleteProgram(shader->program);
}

static long load_shader(const char *path, int type) {
    unsigned shader_id = glCreateShader(type);
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        ar_log(AR_LOG_ERROR, "%s shader could not be opened\n", get_shader_type_name(type));
        return AR_SHADER_NONE;
    }
    // seek file pointer to end of file, get pointer location
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    
    // allocate +1 for null character
    char *data = ar_memory_alloc(fsize+1);
    // read shader file into buffer
    fread(data, 1, fsize, fp);
    // set terminating null character
    data[fsize] = 0;
    fclose(fp);
    
    glShaderSource(shader_id, 1, (const char **)&data, NULL);
    glCompileShader(shader_id);
    ar_memory_free(data);
    check_shader_status(shader_id, type);
    return shader_id;
}

void ar_shader_attach(ar_shader_t *shader, int type, const char *path) {
    if (type == AR_SHADER_VERTEX) {
        shader->vertex = load_shader(path, type);
    }
    else if (type == AR_SHADER_FRAGMENT) {
        shader->fragment = load_shader(path, type);
    }
    else if (type == AR_SHADER_GEOMETRY) {
        shader->geometry = load_shader(path, type);
    }
    else {
        ar_log(AR_LOG_ERROR, "Unsupported shader type\n", 0);
    }
}

unsigned shader_get_uniform_location(ar_shader_t *shader, const char *var, int *uniform_index) {
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

void ar_shader_set_uniform(ar_shader_t *shader, ar_shader_uniform_type_t uniform_type, const char *uniform_name, const void *value) {
    int uniform_index = 0;
    unsigned uniform_location = shader_get_uniform_location(shader, uniform_name, &uniform_index);
    
    (void)uniform_index;
    ar_renderer_check_error();
    
    if (uniform_type == AR_SHADER_VEC3F) {
        ar_vector3f_t *vec = (ar_vector3f_t *)value;
        glUniform3f(uniform_location, vec->x, vec->y, vec->z);
    }
    else if (uniform_type == AR_SHADER_MAT4) {
        ar_mat4_t *mat = (ar_mat4_t *)value;
        glUniformMatrix4fv(uniform_location, 1, GL_FALSE, mat->val);
    }
    else if (uniform_type == AR_SHADER_FLOAT) {
        float *val = (float *)value;
        glUniform1f(uniform_location, *val);
    }
    else if (uniform_type == AR_SHADER_INT) {
        int *val = (int *)value;
        glUniform1i(uniform_location, *val);
    }
    
    const char *err = ar_renderer_check_error();
    if (err != NULL) {
        ar_log(AR_LOG_ERROR, "OpenGL error: %s : %s\n", err, uniform_name);
    }
}

#endif
