#include "batch.h"

#include <glad/glad.h>
#include <stddef.h>

typedef struct vertex
{
    f32 position[2];
    f32 color[4];
}vertex;


struct gzz_batch
{
    u32 count;
    u32 max_count;
    u32 vao;
    u32 vbo;
    u32 ebo;
    f32 *vertices;
};

gzz_batch *gzz_batch_create(u32 max_quads)
{
    gzz_batch *batch;
    batch = GZZ_MALLOC(sizeof(gzz_batch));
    if(batch == NULL)
    {
        return NULL;
    }

    batch->count = 0;
    batch->max_count = max_quads;

    u32 indices[max_quads * 6];

    u32 offset = 0;
    for(u32 i=0; i<max_quads * 6; i+= 6)
    {
        indices[i]     = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    glGenVertexArrays(1, &batch->vao);
    glGenBuffers(1, &batch->vbo);
    glGenBuffers(1, &batch->ebo);

    glBindVertexArray(batch->vao);
    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
    glBufferData(GL_ARRAY_BUFFER, max_quads * 4 * sizeof(vertex), NULL, GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (const void *) offsetof(vertex,position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (const void *) offsetof(vertex,color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    batch->vertices = GZZ_MALLOC(max_quads * 4 * sizeof(vertex));



    return batch;
}



void gzz_batch_add_rectangle(gzz_batch *batch, f32 x, f32 y, f32 w, f32 h,
                             f32 r, f32 g, f32 b, f32 a)
{
    u32 index = batch->count * 4 *6;

    batch->vertices[index]    = x;
    batch->vertices[++index]  = y;
    batch->vertices[++index]  = r;
    batch->vertices[++index]  = g;
    batch->vertices[++index]  = b;
    batch->vertices[++index]  = a;

    batch->vertices[++index]  = x + w;
    batch->vertices[++index]  = y;
    batch->vertices[++index]  = r;
    batch->vertices[++index]  = g;
    batch->vertices[++index]  = b;
    batch->vertices[++index]  = a;

    batch->vertices[++index]  = x + w;
    batch->vertices[++index]  = y + h;
    batch->vertices[++index]  = r;
    batch->vertices[++index]  = g;
    batch->vertices[++index]  = b;
    batch->vertices[++index]  = a;

    batch->vertices[++index]  = x;
    batch->vertices[++index]  = y + h;
    batch->vertices[++index]  = r;
    batch->vertices[++index]  = g;
    batch->vertices[++index]  = b;
    batch->vertices[++index]  = a;

    batch->count++;
}

void gzz_batch_draw(gzz_batch *batch)
{
    glBindVertexArray(batch->vao);
    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, batch->count * 4 * sizeof(vertex), batch->vertices);
    glDrawElements(GL_TRIANGLES, batch->count * 6, GL_UNSIGNED_INT, 0);
    batch->count = 0;
}