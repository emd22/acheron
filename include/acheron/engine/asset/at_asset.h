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
    ar_asset_t *asset;
    ar_asset_operation_t operation;
} ar_asset_queue_item_t;

ar_buffer_t *ar_assetman_get_asset_buffer(void);
ar_thread_t **ar_assetman_get_thread(void);

void *ar_asset_thread_func(void *arg);

ar_asset_t *ar_asset_load(ar_asset_type_t type, char *path);
void ar_asset_destroy(ar_asset_t *asset);

#if 0

//#include <acheron/engine/asset/type/at_image.h>
#include <acheron/engine/asset/type/at_mesh.h>

#include <ar_image/ar_image.h>

typedef void (*ar_asset_load_func_t)(ar_asset_type_t *, ar_asset_t *);

typedef enum {
    AR_ASSET_IMAGE,
    AR_ASSET_MODEL,
    AR_ASSET_MATERIAL,
    AR_ASSET_SHADER,
    AR_ASSET_CONFIG,
    
    AR_ASSET_USER0,
    AR_ASSET_USER1,
    AR_ASSET_USER2,
    AR_ASSET_USER3,
    AR_ASSET_USER4,
    
    AR_ASSET_TYPE_N
} ar_asset_type_e_t;

typedef struct {
    char path[AR_PATH_LENGTH];
    ar_asset_load_func_t load_func;
} ar_asset_type_t;

typedef enum {
    AR_ASSET_QUEUED,
    AR_ASSET_LOADED,
    AR_ASSET_DELETED,
} ar_asset_status_t;

typedef struct {
    char path[AR_PATH_LENGTH];

    ar_asset_type_e_t type;
    ar_asset_status_t status;
    
    union {
        ari_image_t image;
        ar_mesh_t *mesh;
    } data;
} ar_asset_t;

ar_asset_type_t *ar_asset_type_get(ar_asset_type_e_t type_index);


#endif
#endif
