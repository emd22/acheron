#ifndef AR_OB_MESH_H
#define AR_OB_MESH_H

#include <acheron/math/mt_vector.h>

typedef struct {
    ar_vector3f_t position;
    ar_vector2f_t uv;
    ar_vector3f_t normal;
    ar_vector3f_t tangent;
    ar_vectro3f_t bitangent;
} __attribute__((packed)) ar_vertex_t;

typedef struct {
    
} ar_mesh_t;





#endif
