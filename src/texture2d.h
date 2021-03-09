#ifndef GZZ_TEXTURE2D_H
#define GZZ_TEXTURE2D_H

#include "common.h"

typedef struct gzz_texture2d
{
    u32 id;
    u32 width;
    u32 height;
    b32 has_alpha;

}gzz_texture2d;


gzz_texture2d gzz_texture2d_create(const char *path);


#endif //GZZ_TEXTURE2D_H
