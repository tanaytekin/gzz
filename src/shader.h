#ifndef GZZ_SHADER_H
#define GZZ_SHADER_H

#include "common.h"


u32 gzz_shader_create(const char *vertex_path, const char *fragment_path);
void gzz_shader_destroy(u32 shader);

#endif //GZZ_SHADER_H
