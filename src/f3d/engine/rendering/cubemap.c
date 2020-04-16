#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/image/texture.h>
#include <f3d/engine/core/log.h>

#include <GL/glew.h>
#include <GL/gl.h>

shader_t shader_cubemap;

void cubemaps_init() {
    shader_cubemap = shaders_link(
        "Cubemap",
        shader_load("../shaders/cubemap_vert.glsl", SHADER_VERTEX),
        shader_load("../shaders/cubemap_frag.glsl", SHADER_FRAGMENT)
    );

}

void cubemap_init(cubemap_t *cubemap, texture_t **textures) {
    const float cubemap_vertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    cubemap->map = texture_new();
    cubemap->textures = textures;
    texture_init(cubemap->map);

    glGenVertexArrays(1, &cubemap->vao);
    glGenBuffers(1, &cubemap->vbo);
    
    glBindVertexArray(cubemap->vao);
    glBindBuffer(GL_ARRAY_BUFFER, cubemap->vbo);
    glBufferData(GL_ARRAY_BUFFER, 36*(sizeof(float)*3), cubemap_vertices, GL_STATIC_DRAW);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->map->id);
    int i;
    texture_t *texture;
    for (i = 0; i < 6; i++) {
        texture = cubemap->textures[i];
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
            0, GL_RGB, texture->image.width, texture->image.height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, texture->image.data
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void cubemap_render(cubemap_t *cubemap, camera_t *camera) {
    (void)cubemap;
    (void)camera;
    //mat4_t model_mat;
    //mat4_set(&model_mat, MAT4_IDENTITY);

    //glDepthMask(0);
    //glDisable(GL_CULL_FACE);
    //glDepthFunc(GL_LEQUAL);
    //shader_use(&shader_cubemap);
    //glBindVertexArray(cubemap->vao);
    
    
    //glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, cubemap->vbo);
    //glVertexAttribPointer(0, 3, GL_FLOAT, 0, 5*sizeof(float), NULL);
    
    //glEnableVertexAttribArray(1);
    //glBindBuffer(GL_ARRAY_BUFFER, cubemap->vbo);
    //glVertexAttribPointer(1, 2, GL_FLOAT, 0, 5*sizeof(float), (void *)(3*sizeof(float)));
    //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->map->id);
    
    //shader_set_mat4(&shader_cubemap, "projection", &camera->mat_projection);
    //shader_set_mat4(&shader_cubemap, "view", &camera->mat_view);
    //shader_set_mat4(&shader_cubemap, "model", &model_mat);
    
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    //glEnable(GL_CULL_FACE);
    //glDepthMask(1);
    //glDepthFunc(GL_LESS);
}
