#include <gl/model/obj.h>
#include <gl/log.h>

#include <stdio.h>
#include <stdlib.h>

model_obj_t obj_load(const char *path) {
    model_obj_t model;
    
    // https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        log_msg(LOG_ERROR, "Cannot load '%s'\n", path);
        return model;
    }
    // TODO: increase buffer size, change infinite loop
    char line[128];
    vec3f_t *vertex;
    int ret;
    for (;;) {
        ret = fscanf(fp, "%s", line);
        if (ret == EOF)
            break;
            
        if (!strcmp(line, "v")) {
            
        }
    }
}
