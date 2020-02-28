#include <gl/c3d.h>

c3d_error_t errors[C3D_ERROR_HISTORY];

void c3d_error_set(int code) {
    (void)code;
}

int c3d_error_get(void) {
    return 0;
}
