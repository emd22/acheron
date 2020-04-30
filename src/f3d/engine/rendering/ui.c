#include <engine/rendering/mesh.h>

typedef struct {
    mesh_t mesh;
} ui_node_t;

void ui_init() {
    float quadVertices[] = {
        // positions 
        -0.05f,  0.05f, 
         0.05f, -0.05f, 
        -0.05f, -0.05f, 

        -0.05f,  0.05f,
         0.05f, -0.05f, 
         0.05f,  0.05f,	
    };
    (void)quadVertices;
}

void ui_render() {

}
