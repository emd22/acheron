#include <f3d/engine/cubemap.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/texture.h>
#include <f3d/engine/log.h>

#include <GL/glew.h>
#include <GL/gl.h>

void cubemap_init(cubemap_t *cubemap, texture_t **textures) {

    const float skyboxVertices[] = {
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
    
    cubemap->map = texture_new();
    cubemap->textures = textures;
    texture_init(cubemap->map);
    glGenVertexArrays(1, &cubemap->vao);
    glBindVertexArray(cubemap->vao);
    
    glGenBuffers(1, &cubemap->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cubemap->vbo);
    
    glBufferData(GL_ARRAY_BUFFER, 36*(sizeof(float)*3), skyboxVertices, GL_STATIC_DRAW);
    
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

void cubemap_render(cubemap_t *cubemap, shader_t *skybox_shader, camera_t *camera) {
    glDepthMask(0);
    glDisable(GL_CULL_FACE);
    shader_use(skybox_shader);
    glBindVertexArray(cubemap->vao);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cubemap->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, NULL);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->map->id);
    
    shader_set_mat4(skybox_shader, "projection", &camera->mat_projection);
    shader_set_mat4(skybox_shader, "view", &camera->mat_view);
    
    shader_set_int(skybox_shader, "skybox", 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisableVertexAttribArray(0);
    glEnable(GL_CULL_FACE);
    glDepthMask(1);
}
