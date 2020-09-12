#include <acheron/engine/asset/type/at_mesh.h>
#include <acheron/engine/model/obj.h>

#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/engine/math/mt_math.h>
#include <acheron/engine/limits.h>

#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/cr_log.h>

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>

static ar_mesh_t *meshes;
static int meshes_index = 0;

void calculate_tangents(ar_mesh_t *mesh);

void ar_meshes_init(void) {
    meshes = ar_memory_alloc(sizeof(ar_mesh_t)*MAX_MESHES);
}

bool is_near(float v0, float v1) {
    return (fabs(v0-v1) < 0.01f);
}

int vertcmp(const void *a, const void *b) {
    vertex_t *v0 = (vertex_t *)a;
    vertex_t *v1 = (vertex_t *)b;
    
    int total0 = v0->position.x+v0->position.y+v0->position.z;
    int total1 = v1->position.x+v1->position.y+v1->position.z;
    
    if (is_near(v0->position.x, v1->position.x) &&
        is_near(v0->position.y, v1->position.y) &&
        is_near(v0->position.z, v1->position.z) &&
        is_near(v0->uv.x, v1->uv.x) &&
        is_near(v0->uv.y, v1->uv.y) &&
        is_near(v0->normal.x, v1->normal.x) &&
        is_near(v0->normal.y, v1->normal.y) &&
        is_near(v0->normal.z, v1->normal.z))
    {
        return 0;
    }
    else if (total0 > total1) {
        return 1;
    }
    else {
        return -1;
    }
}

int check_vertex_matches(ar_buffer_t *vertices, vertex_t *vertex1) {
    vertex_t *vertex = bsearch(vertex1, (vertex_t *)vertices->data, vertices->index, sizeof(vertex_t), &vertcmp);
    
    if (vertex == NULL)
        return -1;
    // return index of vertex
    return (vertex-(vertex_t *)vertices->data);
}


static void generate_indices(ar_mesh_t *mesh) {
    unsigned i;
    ar_buffer_init(&mesh->indices, AR_BUFFER_STATIC, sizeof(unsigned), mesh->vertices.index, 0);
    
    ar_buffer_t new_verts;
    ar_buffer_init(&new_verts, AR_BUFFER_STATIC, sizeof(vertex_t), mesh->vertices.index, 0);
    vertex_t *vertex;

    int amt_created = 0;
    int index;
    int amt_reused = 0;
    
    for (i = 0; i < mesh->vertices.index; i++) {
        vertex = &((vertex_t *)mesh->vertices.data)[i];
        if ((index = check_vertex_matches(&new_verts, vertex)) != -1) {
            ar_buffer_push(&mesh->indices, &index);
            amt_reused++;
        }
        else {
            ar_buffer_push(&mesh->indices, &new_verts.index);
            ar_buffer_push(&new_verts, vertex);
            amt_created++;
        }
    }
    ar_buffer_destroy(&mesh->vertices);
    mesh->vertices = new_verts;
    ar_log(AR_LOG_INFO, "Saved %.02f KB of VRAM\n", ((float)amt_reused*sizeof(vertex_t))/1024.0f);
}

static void generate_packed_vertices(ar_mesh_t *mesh, ar_buffer_t *vertices, ar_buffer_t *uvs, ar_buffer_t *normals) {
    unsigned i;
    ar_buffer_init(&mesh->vertices, AR_BUFFER_DYNAMIC, sizeof(vertex_t), vertices->index+1, 0);
    vertex_t vertex;
    for (i = 0; i < vertices->index; i++) {
        if (vertices != NULL)
            vertex.position = ((ar_vector3f_t *)vertices->data)[i];
        else
            vertex.position = (ar_vector3f_t){0, 0, 0};
            
        if (uvs != NULL)
            vertex.uv = ((ar_vector2f_t *)uvs->data)[i];
        else
            vertex.uv = (ar_vector2f_t){0, 0};
            
        if (normals != NULL)
            vertex.normal = ((ar_vector3f_t *)normals->data)[i];
        else
            vertex.normal = (ar_vector3f_t){0, 0, 0};
            
        ar_buffer_push(&mesh->vertices, &vertex);
    }
    ar_buffer_destroy(vertices);
    ar_buffer_destroy(uvs);
    ar_buffer_destroy(normals);
}

ar_mesh_t *ar_mesh_new(void) {
    int index = meshes_index++;
    ar_mesh_t *mesh = &meshes[index];
    mesh->type = MODEL_NONE;
    mesh->index = index;
    mesh->primative = GL_TRIANGLES;
    /*mesh->normals = NULL;
    mesh->uvs = NULL;
    mesh->vertices = NULL;*/
    return mesh;    
}

void ar_mesh_init(ar_mesh_t *mesh, int flags) {
    // generate mesh buffers
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->ibo);
    glGenBuffers(1, &mesh->vbo);
    
    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    // TODO: implement methods with dynamic drawing instead of just static
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.index*sizeof(vertex_t), mesh->vertices.data, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.index*sizeof(unsigned), mesh->indices.data, GL_STATIC_DRAW);
    
    mesh->flags = flags;
    
    if (!(flags & AR_MESH_NO_TANGENTS)) {
        calculate_tangents(mesh);
    }
    
    int stride = sizeof(vertex_t);
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, stride, NULL);
    // uv
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, stride, (void *)offsetof(vertex_t, uv));
    // normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, 0, stride, (void *)offsetof(vertex_t, normal));
    // tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, 0, stride, (void *)offsetof(vertex_t, tangent));
    // bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, 0, stride, (void *)offsetof(vertex_t, bitangent));
}

ar_mesh_t *ar_mesh_from_data(ar_mesh_t *mesh, ar_buffer_t *vertices, ar_buffer_t *uvs, ar_buffer_t *normals) {
    if (mesh == NULL)
        mesh = ar_mesh_new();
        
    generate_packed_vertices(mesh, vertices, uvs, normals);
    generate_indices(mesh);
    return mesh;
}

ar_mesh_t *ar_mesh_load_raw(ar_mesh_t *mesh, const char *path, int type, int flags) {
    (void)flags;
    if (mesh == NULL)
        mesh = ar_mesh_new();
        
    if (type == MODEL_OBJ) {
        mesh->obj = ar_memory_alloc(sizeof(obj_model_t));
        obj_model_t obj = obj_load(path);
        memcpy(mesh->obj, &obj, sizeof(obj_model_t));
        
        generate_packed_vertices(mesh, &obj.vertices, &obj.uvs, &obj.normals);
        generate_indices(mesh);
        mesh->type = MODEL_OBJ;
        ar_memory_free(mesh->obj);
    }
    else {
        ar_log(AR_LOG_ERROR, "Cannot load mesh of unknown type\n", 0);
        return NULL;
    }
    return mesh;
}

ar_mesh_t *ar_mesh_load(ar_mesh_t *mesh, const char *path, int type, int flags) {
    ar_mesh_t *tmesh = ar_mesh_load_raw(mesh, path, type, flags);
    ar_mesh_init(tmesh, flags);
    return tmesh;
}

void ar_mesh_draw(ar_mesh_t *mesh, ar_mat4_t *matrix, ar_quat_t *rotation, ar_camera_t *camera, ar_shader_t *shader) {
    if (mesh == NULL || mesh->type == MODEL_NONE)
        return;
        
    if (camera != NULL && matrix != NULL && shader != NULL) {
        ar_mat4_t full_mat = (*matrix);
        ar_mat4_t rot_mat;
        ar_quat_to_mat4(rotation, &rot_mat);
        full_mat = ar_mat4_mul(full_mat, rot_mat);


        ar_mat4_t mvp = ar_mat4_mul(camera->projection, camera->view);
        mvp = ar_mat4_mul(mvp, full_mat);

        ar_shader_set_uniform(shader, AR_SHADER_MAT4, "mvp", &mvp);
        ar_shader_set_uniform(shader, AR_SHADER_MAT4, "m", &full_mat);
    }
    
    glBindVertexArray(mesh->vao);
    glDrawElements(mesh->primative, mesh->indices.index, GL_UNSIGNED_INT, NULL);
}

void calculate_tangents(ar_mesh_t *mesh) {
    ar_vector3f_t v0, v1, v2;
    ar_vector2f_t uv0, uv1, uv2;

    vertex_t *vertices = (vertex_t *)mesh->vertices.data;
    
    ar_vector3f_t delta_pos0, delta_pos1;
    ar_vector2f_t delta_uv0, delta_uv1;
    ar_vector3f_t tangent, bitangent;
    float r;
        
    ar_vector3f_t tmp;
    
    unsigned i;
    unsigned bufidx = 0;
    for (i = 0; i < mesh->vertices.index; i += 3) {
        v0 = vertices[i].position;
        v1 = vertices[i+1].position;
        v2 = vertices[i+2].position;
        
        uv0 = vertices[i].uv;
        uv1 = vertices[i+1].uv;
        uv2 = vertices[i+2].uv;
        
        ar_vector_sub(AR_VEC3F, &v1, &v0, &delta_pos0);  // v1-v0 = delta_pos0
        ar_vector_sub(AR_VEC3F, &v2, &v0, &delta_pos1);  // v2-v0 = delta_pos1
        ar_vector_sub(AR_VEC2F, &uv1, &uv0, &delta_uv0); // uv1-uv0 = delta_uv0
        ar_vector_sub(AR_VEC2F, &uv2, &uv0, &delta_uv1); // uv2-uv0 = delta_uv1
        
        r = 1.0f/(delta_uv0.x*delta_uv1.y-delta_uv0.y*delta_uv1.x);
        
        ar_vector3f_t temp0, temp1;
        
        ar_vector_mul_value(AR_VEC3F, &delta_pos0, delta_uv1.y, &temp0);
        ar_vector_mul_value(AR_VEC3F, &delta_pos1, delta_uv0.y, &temp1);
        ar_vector_sub(AR_VEC3F, &temp0, &temp1, &tmp);
        ar_vector_mul_value(AR_VEC3F, &tmp, r, &tangent);
        
        ar_vector_mul_value(AR_VEC3F, &delta_pos1, delta_uv0.x, &temp0);
        ar_vector_mul_value(AR_VEC3F, &delta_pos0, delta_uv1.x, &temp1);
        ar_vector_sub(AR_VEC3F, &temp0, &temp1, &tmp);
        ar_vector_mul_value(AR_VEC3F, &tmp, r, &bitangent);
        
        // set triangle bitangents and tangents
        
        // set vertices[bufidx]
        vertices[bufidx].bitangent = bitangent;
        vertices[bufidx++].tangent = tangent;
        // set vertices[bufidx+1]
        vertices[bufidx].bitangent = bitangent;
        vertices[bufidx++].tangent = tangent;
        // set vertices[bufidx+2]
        vertices[bufidx].bitangent = bitangent;
        vertices[bufidx++].tangent = tangent;
    }
}

void ar_meshes_cleanup(void) {
    int i;
    for (i = 0; i < meshes_index; i++) {
        ar_mesh_destroy(&meshes[i]);
    }
    ar_memory_free(meshes);
}

void ar_mesh_destroy(ar_mesh_t *mesh) {
    if (mesh == NULL || mesh->type == MODEL_NONE)
        return;
        
    ar_buffer_destroy(&mesh->indices);
    ar_buffer_destroy(&mesh->vertices);
    mesh->type = MODEL_NONE;
}
