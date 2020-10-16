#ifndef AR_AT_ASSET_H
#define AR_AT_ASSET_H

#include <ar_image/ar_image.h>
#include <acheron/engine/asset/type/at_mesh.h>
#include <acheron/engine/core/memory/mm_buffer.h>
#include <acheron/engine/core/threads/cr_threadman.h>
#include <stdbool.h>

typedef enum {
    AR_ASSET_NONE,
    AR_ASSET_IMAGE,
    AR_ASSET_MESH,
    AR_ASSET_SHADER,
    AR_ASSET_BINARY,
} ar_asset_type_t;

typedef enum {
    AR_ASSET_IN_MEM,
    AR_ASSET_QUEUED,
    AR_ASSET_LOADED,
} ar_asset_status_t;

typedef enum {
    AR_ASSET_THREAD_END,
    AR_ASSET_THREAD_RUNNING,
    AR_ASSET_THREAD_WAIT,
} ar_asset_thread_state_t;

typedef enum {
    AR_ASSET_OP_NONE,
    AR_ASSET_OP_LOAD,
    AR_ASSET_OP_FREE,
} ar_asset_operation_t;

typedef struct {
    ar_asset_type_t type;
    ar_asset_status_t status;
    union {
        void *raw;
        ar_mesh_t *mesh;
        ari_image_t image;
    } data;
} ar_asset_t;

typedef struct {
    char *path;
    ar_asset_operation_t operation;
    ar_asset_t *asset;
} ar_asset_queue_item_t;

ar_buffer_t *ar_assetman_get_asset_buffer(void);
ar_buffer_t *ar_assetman_get_queue_buffer(void);
ar_thread_t **ar_assetman_get_thread(void);
void ar_assetman_thread_set_state(ar_asset_thread_state_t state);
void *ar_asset_thread_func(void *arg);

ar_mesh_t   *ar_asset_get_mesh(ar_asset_t *asset);
ari_image_t *ar_asset_get_image(ar_asset_t *asset);

ar_asset_t *ar_asset_load(ar_asset_type_t type, char *path);
void ar_asset_wait(ar_asset_t *asset);
void ar_asset_resolve(ar_asset_t *asset);
void ar_asset_destroy(ar_asset_t *asset);

#endif
