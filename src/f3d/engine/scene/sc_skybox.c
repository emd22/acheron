#include <f3d/engine/scene/sc_skybox.h>
#include <f3d/engine/renderer/rr_shaderman.h>
#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/core/cr_log.h>

ar_shader_t *shader_skybox = NULL;
//unsigned skybox_vao;

skybox_t skybox_new(texture_t **textures) {
    if (shader_skybox == NULL) {
        shader_skybox = ar_shader_new("Skybox");
        ar_shader_attach(shader_skybox, AR_SHADER_VERTEX, "../shaders/skybox_vert.glsl");
        ar_shader_attach(shader_skybox, AR_SHADER_FRAGMENT, "../shaders/skybox_frag.glsl");
    }
    ar_shader_use(shader_skybox);
    skybox_t skybox;
    cubemap_init(&skybox.cubemap, textures);
    skybox.mesh = mesh_load(NULL, "../models/basiccube.obj", MODEL_OBJ, MESH_NO_TANGENTS);   
    return skybox;
}

void skybox_render(skybox_t *skybox, camera_t *camera) {
    if (shader_skybox == NULL) {
        ar_log(AR_LOG_ERROR, "shader null\n", 0);
    }
    //glDepthMask(0);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    ar_shader_use(shader_skybox);
    
    ar_shader_set_uniform(shader_skybox, AR_SHADER_MAT4, "projection", &camera->mat_projection);
    ar_shader_set_uniform(shader_skybox, AR_SHADER_MAT4, "view", &camera->mat_view);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemap.map->id);
    ar_shader_set_uniform(shader_skybox, AR_SHADER_INT, "skybox", 0);
    
    mesh_draw(skybox->mesh, NULL, NULL, NULL);
    
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    //glDepthMask(1);
}

void skybox_destroy() {

}
