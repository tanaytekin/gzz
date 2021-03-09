#ifndef GZZ_SPRITE_H
#define GZZ_SPRITE_H

#include "common.h"
#include "texture_region.h"

#include <cglm/cglm.h>



typedef struct gzz_sprite
{
    vec2 position;
    vec2 size;
    gzz_texture_region *texture_region;
}gzz_sprite;




#endif //GZZ_SPRITE_H
