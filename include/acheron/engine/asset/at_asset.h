#ifndef AR_AT_ASSET_H
#define AR_AT_ASSET_H

typedef int iso_warning2_t;

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
