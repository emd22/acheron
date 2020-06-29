#ifndef AR_AT_ASSETMAN_H
#define AR_AT_ASSETMAN_H

typedef enum {
    AR_ASSET_NONE,
    AR_ASSET_INCOMPLETE,
    AR_ASSET_LOADED,
    AR_ASSET_ERROR,
} ar_asset_status_t;

typedef struct {
    AR_ASSET_IMAGE,
    AR_ASSET_MESH,
} ar_asset_type_t;

typedef struct ar_asset_s {
    ar_asset_type_t type;
    ar_asset_status_t status;
    void (*on_loaded)(struct ar_asset_s *);
    union {
        mesh_t *mesh;
        image_t image;
    } data;
} ar_asset_t;

#endif
