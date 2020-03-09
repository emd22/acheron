#include <gl/model/obj.h>
#include <gl/log.h>

#include <gl/type/buffer.h>
#include <gl/type/vec.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

model_obj_t obj_load(const char *path) {
    // https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

    model_obj_t model;
    
    buffer_init(&model.vertices, sizeof(vector3f_t), 4096);
    buffer_init(&model.uvs,      sizeof(vector2f_t), 2048);
    buffer_init(&model.normals,  sizeof(vector3f_t), 2048);
    
    buffer_t temp_vertices;
    buffer_t temp_uvs;
    buffer_t temp_normals;
    buffer_init(&temp_vertices, sizeof(vector3f_t), 4096);
    buffer_init(&temp_uvs,      sizeof(vector2f_t), 2048);
    buffer_init(&temp_normals,  sizeof(vector3f_t), 2048);
    
    buffer_t vertex_indices, uv_indices, normal_indices;
    buffer_init(&vertex_indices, sizeof(unsigned), 4096);
    buffer_init(&uv_indices,     sizeof(unsigned), 4096);
    buffer_init(&normal_indices, sizeof(unsigned), 2048);

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        log_msg(LOG_ERROR, "Could not load '%s'\n", path);
        return model;
    }
    // TODO: up line buffer size, use non infinite loop
    char line[128];
    int res;
    for (;;) {
        res = fscanf(fp, "%s", line);
        if (res == EOF)
            break;
        if (!strcmp(line, "v")) {
            vector3f_t vertex;
            fscanf(fp, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            buffer_push(&temp_vertices, &vertex);
        }
        else if (!strcmp(line, "vt")) {
            vector2f_t uv;
            fscanf(fp, "%f %f\n", &uv.x, &uv.y);
            buffer_push(&temp_uvs, &uv);
        }
        else if (!strcmp(line, "vn")) {
            vector3f_t normal;
            fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            buffer_push(&temp_normals, &normal);
            log_msg(LOG_INFO, "NRML: %f %f %f\n", normal.x, normal.y, normal.z);
        }
        else if (!strcmp(line, "f")) {
            unsigned vertex_index[3], uv_index[3], normal_index[3];
            int matches = fscanf(
                fp, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
                &vertex_index[0], &uv_index[0], &normal_index[0],
                &vertex_index[1], &uv_index[1], &normal_index[1],
                &vertex_index[2], &uv_index[2], &normal_index[2]
            );
            (void)matches;
            int i;
            for (i = 0; i < 3; i++) {
                buffer_push(&vertex_indices, &vertex_index[i]);
                buffer_push(&uv_indices,     &uv_index[i]);
                buffer_push(&normal_indices, &normal_index[i]);
            }
        }
    }
    unsigned i;
    vector3f_t vertex;
    for (i = 0; i < vertex_indices.index; i++) {
        unsigned vertex_index = ((unsigned *)vertex_indices.data)[i];
        vertex = ((vector3f_t *)temp_vertices.data)[vertex_index-1];
        buffer_push(&model.vertices, &vertex);
    }
    vector2f_t uv;
    for (i = 0; i < uv_indices.index; i++) {
        unsigned uv_index = ((unsigned *)uv_indices.data)[i];
        uv = ((vector2f_t *)temp_uvs.data)[uv_index-1];
        buffer_push(&model.uvs, &uv);
    }
    vector3f_t normal;
    for (i = 0; i < normal_indices.index; i++) {
        unsigned normal_index = ((unsigned *)normal_indices.data)[i];
        normal = ((vector3f_t *)temp_normals.data)[normal_index-1];
        buffer_push(&model.normals, &normal);
    }
    buffer_destroy(&temp_vertices);
    buffer_destroy(&temp_uvs);
    buffer_destroy(&temp_normals);
    
    buffer_destroy(&vertex_indices);
    buffer_destroy(&uv_indices);
    buffer_destroy(&normal_indices);
    fclose(fp);
    return model;
}

void obj_destory(model_obj_t *model) {
    buffer_destroy(&model->vertices);
    buffer_destroy(&model->uvs);
    buffer_destroy(&model->normals);
}
