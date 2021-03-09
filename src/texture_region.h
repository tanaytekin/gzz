#ifndef GZZ_TEXTURE_REGION_H
#define GZZ_TEXTURE_REGION_H

#include "common.h"
#include "texture2d.h"


#include <cglm/cglm.h>


typedef struct gzz_texture_region
{
    vec2 uv;
    vec2 uv2;
    gzz_texture2d *texture;
}gzz_texture_region;




#endif //GZZ_TEXTURE_REGION_H
