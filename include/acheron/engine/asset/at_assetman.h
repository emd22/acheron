#ifndef AR_AT_ASSETMAN_H
#define AR_AT_ASSETMAN_H

#include <acheron/engine/object/ob_mesh.h>
#include <acheron/engine/image/image.h>

typedef enum {
    AR_ASSET_NONE,
    AR_ASSET_LOAD,
    AR_ASSET_DONE,
    AR_ASSET_ERROR,
} ar_asset_status_t;

typedef enum {
    AR_ASSET_TYPE_IMAGE,
    AR_ASSET_TYPE_MESH,
    AR_ASSET_TYPE_RAW,
} ar_asset_type_t;

typedef struct ar_asset_s {
    ar_asset_type_t type;
    ar_asset_status_t status;
    void (*on_loaded)(struct ar_asset_s *);
    union {
        ar_mesh_t *mesh;
        image_t image;
    } data;
} ar_asset_t;

#endif
