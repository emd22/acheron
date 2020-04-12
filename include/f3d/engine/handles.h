#ifndef F3D_HANDLES_H
#define F3D_HANDLES_H

enum {
    HANDLE_INIT,
    HANDLE_END,
    HANDLE_DRAW,
    HANDLE_RENDER_MESHES,
    
    HANDLES_AMT
};

typedef int (*handle_t)(void *);

void handles_init();
void handle_set(int index, handle_t handle);
handle_t handle_get(int index);
int handle_call(int index, void *arg);


#endif
