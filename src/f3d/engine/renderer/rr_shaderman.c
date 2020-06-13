#include <f3d/engine/renderer/rr_shaderman.h>

#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/core/memory/mm_memory.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/engine.h>
#include <f3d/engine/limits.h>

#include <f3d/engine/types.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define SHADER_NONE -1

static ar_shader_t shaders[MAX_SHADERS];
static ar_shader_t *shader_table[AR_SHADER_TABLE_SIZE];

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

void ar_shader_manager_set(ar_shader_table_index_t index, ar_shader_t *shader) {
    shader_table[index] = shader;
}

ar_shader_t *ar_shader_manager_get(ar_shader_table_index_t index) {
    return shader_table[index];
}

void check_status(unsigned id, int type) {
    int info_log_length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
	    char info_log[info_log_length];
	    glGetShaderInfoLog(id, info_log_length, NULL, info_log);
	    ar_log(AR_LOG_ERROR, "Error in %s shader! (%d)\n", get_shader_type_name(type), info_log_length);
	    ar_log(AR_LOG_ERROR, "Error log:\n%s\n", info_log);
	}
}

bool shader_check_linked(ar_shader_t *shader) {
    return (shader->program != SHADER_NONE);
}

static void link_shader_check(ar_shader_t *shader) {
    if (!shader_check_linked(shader))
        ar_shader_link(shader);
} 

void ar_shader_use(ar_shader_t *shader) {
    link_shader_check(shader);
    glUseProgram(shader->program);
}

void ar_shader_link(ar_shader_t *shader) {
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

void ar_shader_destroy(ar_shader_t *shader) {
    if (shader->program != SHADER_NONE)
        glDeleteProgram(shader->program);
}

static long load_shader(const char *path, int type) {
    unsigned shader_id = glCreateShader(type);
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        ar_log(AR_LOG_ERROR, "%s shader could not be opened\n", get_shader_type_name(type));
        return SHADER_NONE;
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
    check_status(shader_id, type);
    return shader_id;
}

ar_shader_t *ar_shader_get(const char *name) {
    int i;
    hash_t hash = util_hash_str(name);
    for (i = 0; i < shaders_index; i++) {
        if (hash == shaders[i].hash)
            return &shaders[i];
    }
    return NULL;
}

ar_shader_t *ar_shader_new(const char *name) {
    int index = shaders_index++;
    ar_shader_t *shader = &shaders[index];
    shader->hash = util_hash_str(name);
    
    shader->vertex = SHADER_NONE;
    shader->fragment = SHADER_NONE;
    shader->geometry = SHADER_NONE;
    shader->program = SHADER_NONE;
    shader->uniform_index = 0;
    
    strcpy(shader->name, name);
    return shader;
}

