#ifdef AR_USE_OPENGL

#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/renderer/opengl/gl_texture.h>
#include <acheron/engine/renderer/rr_texture.h>

#define AR_TEXTURE_BAD_ID -1

typedef enum {
    AR_TEXTURE_MIN_FILTER,
    AR_TEXTURE_MAG_FILTER,
    AR_TEXTURE_WRAP_S,
    AR_TEXTURE_WRAP_T,
} ar_texture_parameter_t;

enum {

};

ar_texture_t ar_texture_new(ar_texture_bind_type_t bind_type) {
    ar_texture_t texture;
    // Generate OpenGL texture handle
    glGenTextures(1, &texture.id);
    // set rgb+rgb by default
    texture.data_type = AR_TEXTURE_RGB;
    texture.draw_type = AR_TEXTURE_RGB;

    texture.bind_type = bind_type;
    
    return texture;
}

void ar_texture_set_parameter(ar_texture_t *texture) {

}

void ar_texture_destroy(ar_texture_t *texture) {
    if (texture->id < 0) {
        ar_log(AR_LOG_ERROR, "Texture id not valid\n", 0);
        return;
    }
    glDeleteTextures(1, &texture->id);
}


#endif