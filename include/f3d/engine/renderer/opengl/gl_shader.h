#ifndef AR_GL_SHADER_H
#define AR_GL_SHADER_H

#define SHADER_VERTEX   GL_VERTEX_SHADER
#define SHADER_FRAGMENT GL_FRAGMENT_SHADER
#define SHADER_GEOMETRY GL_GEOMETRY_SHADER

typedef enum {
    AR_SHADER_VEC3F,
    AR_SHADER_MAT4,
    AR_SHADER_FLOAT,
    AR_SHADER_INT,
} ar_shader_uniform_type_t;

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

void ar_shader_attach(ar_shader_t *shader, int type, const char *path);
void ar_shader_use(ar_shader_t *shader);
void ar_shader_link(ar_shader_t *shader);

void ar_shader_set_uniform(ar_shader_t *shader, ar_shader_uniform_type_t uniform_type, const char *uniform_name, void *value);

#endif
