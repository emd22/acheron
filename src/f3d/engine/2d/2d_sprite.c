#include <f3d/engine/2d/2d_sprite.h>
#include <f3d/engine/model/mesh.h>
#include <f3d/engine/image/texture.h>
#include <f3d/engine/math/mt_math.h>
#include <f3d/engine/model/mesh.h>

/*
static mesh_t *sprite_mesh = NULL;
static ar_buffer_t sprite_buffer;

ar_sprite_t *ar_sprite_new(void) {
    if (!ar_buffer_is_initialized(&sprite_buffer)) {
        ar_buffer_init(&sprite_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_sprite_t), 32, 0);
    }
    ar_sprite_t *sprite = ar_buffer_new_item(&sprite_buffer);
    sprite->position = (ar_vector3f_t){0, 0, 0};
    sprite->rotation = (ar_vector3f_t){0, 0, 0};
    sprite->texture = NULL;
}

static void generate_sprite_mesh() {
    const float vertices[] = {
    };
    
    const float uvs[] = {
                
    };
    
    const float normals[] = {
    
    };
    
    ar_buffer_t vert_buf = ar_buffer_from_data(AR_BUFFER_STATIC, vertices, sizeof(float), sizeof(vertices)/sizeof(vertices[0]), 0);
    ar_buffer_t uv_buf = ar_buffer_from_data(AR_BUFFER_STATIC, uvs, sizeof(float), sizeof(uvs)/sizeof(uvs[0]), 0);
    ar_buffer_t norm_buf = ar_buffer_from_data(AR_BUFFER_STATIC, normals, sizeof(float), sizeof(normals)/sizeof(normals[0]), 0);
    
    sprite_mesh = ar_mesh_from_data(NULL, &vert_buf, &uv_buf, &norm_buf);
    
    ar_buffer_destroy(&vert_buf);
    ar_buffer_destroy(&uv_buf);
    ar_buffer_destroy(&norm_buf);
}

ar_sprite_t *ar_sprite_load(ar_sprite_t *sprite, const char *path, ar_image_type_t image_type) {
    if (sprite == NULL)
        sprite = ar_sprite_new();
        
    texture_load(sprite->texture, path, image_type);
    if (sprite_mesh == NULL) {
        
    }
}*/
