#ifndef GZZ_BATCH_H
#define GZZ_BATCH_H

#include "common.h"

typedef struct gzz_batch gzz_batch;



gzz_batch *gzz_batch_create(u32 max_quads);
void gzz_batch_add_rectangle(gzz_batch *batch, f32 x, f32 y, f32 w, f32 h,
                             f32 r, f32 g, f32 b, f32 a);

void gzz_batch_draw(gzz_batch *batch);

#endif //GZZ_BATCH_H
