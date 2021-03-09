#ifndef GZZ_BATCH_RENDERER_H
#define GZZ_BATCH_RENDERER_H

#include "common.h"
#include "sprite.h"

#include <cglm/cglm.h>

typedef struct gzz_batch_renderer_performance_info
{
    u32 quad_count;
    u32 draw_call_count;

}gzz_batch_renderer_performance_info;


typedef struct vertex vertex;

struct gzz_batch_renderer
{
    u32 max_quads;
    u32 quad_count;
    u32 vao;
    u32 vbo;
    u32 ebo;

    u32 shader;

    u32 textures[32];
    u32 texture_count;

    gzz_batch_renderer_performance_info *performance_info;

    mat4 projection;

    vertex *buffer;
    vertex *buffer_ptr;
};

typedef struct gzz_batch_renderer gzz_batch_renderer;


gzz_batch_renderer *gzz_batch_renderer_create(u32 max_quads, gzz_batch_renderer_performance_info *performance_info);

void gzz_batch_renderer_begin(gzz_batch_renderer *batch);
void gzz_batch_renderer_add_sprite(gzz_batch_renderer *batch, gzz_sprite *sprite);
void gzz_batch_renderer_end(gzz_batch_renderer *batch);
void gzz_batch_renderer_flush(gzz_batch_renderer *batch);




#endif //GZZ_BATCH_RENDERER_H
