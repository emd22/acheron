#include <acheron/engine/limits.h>
#include <acheron/engine/asset/at_asset.h>
#include <ar_image/ar_image.h>

void ar_asset_load_image(const char *path, ar_asset_t *asset, ari_image_type_t image_type) {
    ar_image_load(path, &asset->data.image, image_type);
}

ar_asset_t ar_asset_load(const char *path, ar_asset_type_t type) {
    if (type == AR_ASSET_IMAGE) {
        ar_image_load(path, &asset->data.image, ARI_TYPE_AUTO);
        asset->type = AR_ASSET_IMAGE;
    }
    else if (type == AR_ASSET_MODEL) {
        
    }
     
}
