#include <f3d/engine/model/mesh.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/image/texture.h>
#include <f3d/engine/rendering/ui.h>
#include <f3d/engine/math/mt_math.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/types.h>

typedef struct {
    mesh_t *mesh;
} ui_node_t;

static ui_node_t node;
ar_shader_t *shader_ui;
//static mat4_t matrix;
//static camera_t ui_cam;
//static texture_t *tex;

void ui_init() {
    shader_ui = ar_shader_new("UI");
    ar_shader_attach(shader_ui, SHADER_VERTEX, "../shaders/ui/ui_vert.glsl");
    ar_shader_attach(shader_ui, SHADER_FRAGMENT, "../shaders/ui/ui_frag.glsl");
    
    //node.mesh = mesh_load(NULL, "../models/cube.obj", MODEL_OBJ, 0);
    node.mesh = mesh_new();
    ar_vector3f_t quad_vertices[] = {
        (ar_vector3f_t){-1, -1, 0},
        (ar_vector3f_t){-1, 1, 0},
        (ar_vector3f_t){1, 1, 0},
        (ar_vector3f_t){1, -1, 0}
    };
    (void)quad_vertices;
    
/*    float quad_vertices[] = {*/
/*        // positions */
/*        -0.05f,  0.05f,*/
/*         0.05f, -0.05f, */
/*        -0.05f, -0.05f, */

/*        -0.05f,  0.05f,*/
/*         0.05f, -0.05f, */
/*         0.05f,  0.05f,	*/
/*    };*/
    
    //mesh_set_data(node.mesh, quad_vertices, 4, NULL, 0, NULL, 0, MESH_NO_TANGENTS);
    //tex = texture_load(NULL, "../images/melon.bmp", IMAGE_BMP);
    
    //mat4_set(&matrix, MAT4_IDENTITY);
    //math_ortho(&ui_cam.mat_projection, -10, 10, -10, 10, -10, 10);
    //mat4_translate(&matrix, (vector3f_t){4, 0, 0});
    //matrix = mat4_rotate_x(matrix, 2);
    //mat4_scale(&matrix, matrix, 5);
}

void ui_render() {
    //glDisable(GL_CULL_FACE);
    //shader_use(shader_ui);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, tex->id);
    //shader_set_int(shader_ui, "image", 0);
    //mesh_draw(node.mesh, &matrix, &ui_cam, shader_ui);
    //glEnable(GL_CULL_FACE);
}
