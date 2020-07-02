#include <acheron/engine/model/obj.h>
#include <acheron/engine/model/mtl.h>

#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/core/cr_handles.h>

#include <acheron/engine/math/mt_math.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

obj_model_t obj_load(const char *path) {
    // https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

    obj_model_t model;
    model.inited = 0;
    
    ar_buffer_init(&model.vertices, AR_BUFFER_DYNAMIC, sizeof(ar_vector3f_t), 4096, 0);
    ar_buffer_init(&model.uvs,      AR_BUFFER_DYNAMIC, sizeof(ar_vector2f_t), 4096, 0);
    ar_buffer_init(&model.normals,  AR_BUFFER_DYNAMIC, sizeof(ar_vector3f_t), 4096, 0);
    
    ar_buffer_t temp_vertices, temp_uvs, temp_normals;
    ar_buffer_init(&temp_vertices, AR_BUFFER_DYNAMIC, sizeof(ar_vector3f_t), 4096, 0);
    ar_buffer_init(&temp_uvs,      AR_BUFFER_DYNAMIC, sizeof(ar_vector2f_t), 4096, 0);
    ar_buffer_init(&temp_normals,  AR_BUFFER_DYNAMIC, sizeof(ar_vector3f_t), 4096, 0);
    
    ar_buffer_t vertex_indices, uv_indices, normal_indices;
    ar_buffer_init(&vertex_indices, AR_BUFFER_DYNAMIC, sizeof(unsigned), 8192, 0);
    ar_buffer_init(&uv_indices,     AR_BUFFER_DYNAMIC, sizeof(unsigned), 8192, 0);
    ar_buffer_init(&normal_indices, AR_BUFFER_DYNAMIC, sizeof(unsigned), 8192, 0);

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        ar_log(AR_LOG_ERROR, "Could not load '%s'\n", path);
        return model;
    }
    // TODO: up line buffer size, use non infinite loop
    char line[128];
    unsigned line_number = 0;
    int res;
    
    for (;; line_number++) {
        res = fscanf(fp, "%s", line); 
        if (res == EOF)
            break;
        if (!strcmp(line, "v")) {
            ar_vector3f_t vertex;
            fscanf(fp, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            ar_buffer_push(&temp_vertices, &vertex);
        }
        else if (!strcmp(line, "vt")) {
            ar_vector2f_t uv;
            fscanf(fp, "%f %f\n", &uv.x, &uv.y);
            ar_buffer_push(&temp_uvs, &uv);
        }
        else if (!strcmp(line, "vn")) {
            ar_vector3f_t normal;
            fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            ar_buffer_push(&temp_normals, &normal);
        }
        else if (!strcmp(line, "f")) {
            unsigned vertex_index[4] = { 0 },
                uv_index[4] = { 0 },
                normal_index[4] = { 0 };
            char newl[128];
            fgets(newl, 128, fp);
            int matches = sscanf(
                newl, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
                &vertex_index[0], &uv_index[0], &normal_index[0],
                &vertex_index[1], &uv_index[1], &normal_index[1],
                &vertex_index[2], &uv_index[2], &normal_index[2]
            );
            int i;

            if (matches != 9) {
                uv_index[0] = 1;
                uv_index[1] = 1;
                uv_index[2] = 1;
                matches = sscanf(
                    newl, "%u//%u %u//%u %u//%u\n",
                    &vertex_index[0], &normal_index[0],
                    &vertex_index[1], &normal_index[1],
                    &vertex_index[2], &normal_index[2]
                );
                if (matches != 6) {
                    ar_log(AR_LOG_ERROR, "Model not supported\n", 0);
                    // TODO: fail gracefully
                    exit(1);                
                }
            }
            for (i = 0; i < 3; i++) {
                ar_buffer_push(&vertex_indices, &vertex_index[i]);
                ar_buffer_push(&uv_indices,     &uv_index[i]);
                ar_buffer_push(&normal_indices, &normal_index[i]);
            }
        }
        else if (!strcmp(line, "mtllib")) {
            char mtlpath[64];
            strcpy(mtlpath, "../models/");
            
            char temp[32];
            fscanf(fp, "%s\n", temp);
            strcat(mtlpath, temp);
            
            //mtl_load(mtlpath);
        }
        else {
            char fut[2048];
            fgets(fut, 2048, fp);
        }
    }
    unsigned i;
    for (i = 0; i < vertex_indices.index; i++) {
        unsigned vert_index = ((unsigned *)vertex_indices.data)[i];
        unsigned uv_index   = ((unsigned *)uv_indices.data)[i];
        unsigned norm_index = ((unsigned *)normal_indices.data)[i];
        ar_vector3f_t vertex = ((ar_vector3f_t *)temp_vertices.data)[vert_index-1];
        ar_vector2f_t uv     = ((ar_vector2f_t *)temp_uvs.data)[uv_index-1];
        ar_vector3f_t normal = ((ar_vector3f_t *)temp_normals.data)[norm_index-1];
        ar_buffer_push(&model.vertices, &vertex);
        ar_buffer_push(&model.uvs, &uv);
        ar_buffer_push(&model.normals, &normal);    
    }
    
    //ar_buffer_reduce_to_data(&model.vertices);
    //ar_buffer_reduce_to_data(&model.uvs);
    //ar_buffer_reduce_to_data(&model.normals);
    
    ar_buffer_destroy(&temp_vertices);
    ar_buffer_destroy(&temp_normals);
    ar_buffer_destroy(&temp_uvs);  
    ar_buffer_destroy(&normal_indices);
    ar_buffer_destroy(&vertex_indices);
    ar_buffer_destroy(&uv_indices);
    
    fclose(fp);
    model.inited = 1;
    return model;
}

void obj_destroy(obj_model_t *model) {
    if (!model->inited)
        return;
    // vertex
    ar_buffer_destroy(&model->vertices);
    ar_buffer_destroy(&model->uvs);
    ar_buffer_destroy(&model->normals);
}
