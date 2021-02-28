#ifndef GZZ_SHADER_H
#define GZZ_SHADER_H

#include "common.h"

#include <cglm/cglm.h>


u32 gzz_shader_create(const char *vertex_path, const char *fragment_path);
void gzz_shader_destroy(u32 shader);
void gzz_shader_use(u32 shader);
void gzz_shader_set_mat4(u32 shader, const char *name, mat4 mat);

#endif //GZZ_SHADER_H
