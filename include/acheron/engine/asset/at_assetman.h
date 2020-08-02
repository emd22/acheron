#ifndef AR_AT_ASSETMAN_H
#define AR_AT_ASSETMAN_H

#include <acheron/engine/object/ob_mesh.h>
#include <acheron/engine/image/image.h>

#if 0

typedef enum {
    AR_ASSET_TYPE_NONE,
    AR_ASSET_TYPE_IMAGE,
    AR_ASSET_TYPE_MESH,
    AR_ASSET_TYPE_SHADER,
    AR_ASSET_TYPE_BINARY,
    
    AR_ASSET_TYPE_AMT
} ar_asset_type_t;

typedef struct ar_asset_s {
    char path[AR_PATH_LENGTH];

    ar_asset_type_t type;
    ar_asset_status_t status;  
    
    void (*on_loaded)(struct ar_asset_s *);
    
    union {
        uint8_t *raw;
        ar_mesh_t *mesh;
        ari_image_t image;
    } data;
} ar_asset_t;

typedef enum {
    AR_ASSET_OP_DONE,
    AR_ASSET_OP_LOAD,
    AR_ASSET_OP_FREE,
} ar_asset_operation_type_t;

typedef enum {
    AR_ASSET_PREC_LOW,
    AR_ASSET_PREC_MED,
    AR_ASSET_PREC_HIGH,
} ar_asset_operation_prec_t;
    
typedef enum {
    AR_ASSET_STAT_NONE,
    AR_ASSET_STAT_LOADING,
    AR_ASSET_STAT_LOADED,
    AR_ASSET_STAT_ERROR,
} ar_asset_operation_status_t;

typedef struct {
    ar_asset_operation_type_t type;
    ar_asset_operation_prec_t prec;
    ar_asset_type_t asset_type;
    ar_asset_operation_status_t status;
    
    char path[AR_PATH_LENGTH];
    ar_asset_t *asset;
} ar_asset_operation_t;

#endif
#endif
