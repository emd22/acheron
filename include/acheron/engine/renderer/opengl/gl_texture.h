#ifndef AR_GL_TEXTURE_H
#define AR_GL_TEXTURE_H
#ifdef AR_USE_OPENGL

#include <ar_image/ar_image.h>

typedef struct {
    unsigned id;
} ar_intern_texture_t;

#define AR_TEXTURE_TYPE_RGB     GL_RGB
#define AR_TEXTURE_TYPE_RGBA    GL_RGBA

//typedef struct {
//    unsigned id;
    // type of data stored e.g RGB, RGBA, etc.
//    int data_type;
//    int draw_type;
//    int bind_type;
//    int index;
//    
//    ari_image_t image;
//} ar_texture_t;

//typedef struct {
//    unsigned id;
//    ari_image_t image;
//} ar_texture_t;




#endif
#endif