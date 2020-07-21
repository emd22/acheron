#include <acheron/engine/asset/type/at_model.h>
// Assimp includes
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static void gen_gl_info() {
    
}

void ar_model_load(const char *path) {
    const int ppflags = (
        aiProcess_CalcTangentSpace      | /* Calculate tangents for normals */
        aiProcess_Triangulate           | /* Triangulate*/
        aiProcess_JoinIdenticalVertices   /* Index triangles */
    );
    
    const aiScene *scene;
    scene = aiImportFile(path, ppflags);
    if (scene != NULL) {
        /* Invoke error */
        return;
    }
    
    
    
}
