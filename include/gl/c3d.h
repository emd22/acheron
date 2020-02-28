#ifndef C3D_H
#define C3D_H

#define C3D_ERROR_HISTORY 64

#define C3D_FAILURE -1
#define C3D_SUCCESS 0

typedef struct {
    int type;
    char function[64];
    char msg[64];
} c3d_error_t;

#endif
