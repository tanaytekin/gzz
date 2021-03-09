#ifndef GZZ_SHADER_H
#define GZZ_SHADER_H

#include "common.h"


u32 gzz_shader_create(const char *vertex_path, const char *fragment_path);
void gzz_shader_destroy(u32 shader);


void gzz_shader_use(u32 shader);
void gzz_shader_set_1i(u32 shader, const char *name, i32 v);

#endif //GZZ_SHADER_H
