#include <gl/model/obj.h>
#include <gl/log.h>

#include <stdio.h>
#include <stdlib.h>

model_obj_t obj_load(const char *path) {
    model_obj_t model;
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        log_msg(LOG_ERROR, "Could not load '%s'\n", path);
        return model;
    }
    //model.vertices = malloc(sizeof(vector3f_t)*);
    // TODO: up line buffer size, use non infinite loop
    char line[128];
    int res;
    for (;;) {
        res = fscanf(fp, "%s", line);
        if (res == EOF)
            break;
    }
    fclose(fp);
    return model;
}
