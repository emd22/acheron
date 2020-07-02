#ifndef AR_RR_SHADERMAN_H
#define AR_RR_SHADERMAN_H

#include <acheron/engine/renderer/rr_shader.h>

ar_shader_t *ar_shader_new(const char *name);
ar_shader_t *ar_shader_get(const char *name);
void ar_shader_buffer_destroy(void);

#endif
