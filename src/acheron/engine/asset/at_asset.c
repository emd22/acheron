#include <acheron/engine/limits.h>
#include <acheron/engine/asset/at_asset.h>

static ar_asset_type_t asset_types[AR_ASSET_TYPE_N];

ar_asset_type_t *ar_asset_type_get(ar_asset_type_e_t type_index) {
    return &(asset_types[type_index]);
}

void ar_asset_load_image(ar_asset_type_t *type, ar_asset_t *asset) {
    
}

void ar_asset_load(const char *path) {
    
}
