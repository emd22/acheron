#include <f3d/engine/rendering/skybox.h>
#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/core/log.h>

struct {
    cubemap_t *cubemap;
    unsigned vao;
    unsigned vbo;
} skybox;

shader_t shader_skybox;
//unsigned skybox_vao;

void skybox_init(cubemap_t *cubemap) {
    skybox.cubemap = cubemap;
    shader_skybox = shaders_link(
        "Skybox",
        shader_load("../shaders/skybox_vert.glsl", SHADER_VERTEX),
        shader_load("../shaders/skybox_frag.glsl", SHADER_FRAGMENT)
    );
    const float skybox_vertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    
    shader_use(&shader_skybox);
    glGenVertexArrays(1, &skybox.vao);
    glGenBuffers(1, &skybox.vbo);
    glBindVertexArray(skybox.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo);
    glBufferData(GL_ARRAY_BUFFER, 36*(sizeof(float)*3), skybox_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, NULL);

}

void skybox_render(camera_t *camera) {
    //glDepthMask(0);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    shader_use(&shader_skybox);
    shader_set_mat4(&shader_skybox, "projection", &camera->mat_projection);
    shader_set_mat4(&shader_skybox, "view", &camera->mat_view);
    
    //cubemap_render(skybox.cubemap, camera);
    glBindVertexArray(skybox.vao);
    glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap->map->id);
    shader_set_int(&shader_skybox, "skybox", 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    //glDepthMask(1);
}

void skybox_destroy() {

}
