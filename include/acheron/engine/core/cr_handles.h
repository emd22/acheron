#ifndef AR_CR_HANDLES_H
#define AR_CR_HANDLES_H

typedef enum {
    AR_HANDLE_INIT,
    AR_HANDLE_END,
    AR_HANDLE_DRAW,
    AR_HANDLE_RENDER_MESHES,
    
    AR_HANDLE_ON_TEXTURE_LOAD,  
    AR_HANDLE_ON_SHADER_LOAD,
    AR_HANDLE_ON_MATERIAL_LOAD,
    
    AR_HANDLE_SYS_SIGNAL,
    
    AR_HANDLE_MOUSE_MOVE,
    
    AR_HANDLES_AMT
} ar_handle_type_t;

typedef int (*ar_handle_t)(void *);

void ar_handles_init(void);
void ar_handle_set(ar_handle_type_t handle_type, ar_handle_t handle);
ar_handle_t ar_handle_get(ar_handle_type_t handle_type);
int ar_handle_call(ar_handle_type_t handle_type, void *arg);


#endif
