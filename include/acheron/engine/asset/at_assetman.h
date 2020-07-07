#ifndef AR_AT_ASSETMAN_H
#define AR_AT_ASSETMAN_H

#include <acheron/engine/object/ob_mesh.h>
#include <acheron/engine/image/image.h>

typedef enum {
    AR_ASSET_NONE,
    AR_ASSET_LOAD,    // waiting to be loaded
    AR_ASSET_PARTIAL, // in system memory, needs to be setup for renderer
    AR_ASSET_DONE,    // asset is loaded, ready to be drawn
    AR_ASSET_ERROR,
} ar_asset_status_t;

typedef enum {
    AR_ASSET_TYPE_NONE,
    AR_ASSET_TYPE_IMAGE,
    AR_ASSET_TYPE_MESH,
    AR_ASSET_TYPE_SHADER,
    AR_ASSET_TYPE_BINARY,
    
    AR_ASSET_TYPE_AMT
} ar_asset_type_t;

typedef enum {
    AR_ASSET_PREC_LOW,
    AR_ASSET_PREC_MEDIUM,
    AR_ASSET_PREC_HIGH,
    AR_ASSET_PREC_IMMEDIATE,
} ar_asset_precedence_t;

typedef struct ar_asset_s {
    char path[AR_PATH_LENGTH];

    ar_asset_type_t type;
    ar_asset_status_t status;
    ar_asset_precedence precedence;    
    
    void (*on_loaded)(struct ar_asset_s *);
    
    union {
        ar_mesh_t *mesh;
        image_t image;
    } data;
} ar_asset_t;

#endif
