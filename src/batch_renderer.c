#include "batch_renderer.h"

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <stddef.h>
#include <memory.h>

#include "shader.h"
#include "log.h"




struct vertex
{
    vec2 position;
    vec2 uv;
    f32 texture_index;
};




gzz_batch_renderer *gzz_batch_renderer_create(u32 max_quads, gzz_batch_renderer_performance_info *performance_info)
{
    gzz_batch_renderer *batch;
    batch = GZZ_MALLOC(sizeof(gzz_batch_renderer));
    memset(batch, 0, sizeof(gzz_batch_renderer));

    if(batch == NULL)
    {
        return NULL;
    }

    batch->quad_count = 0;
    batch->performance_info = performance_info;
    batch->max_quads = max_quads;
    batch->shader = gzz_shader_create("../shaders/batch_renderer_vertex.glsl", "../shaders/batch_renderer_fragment.glsl");


    size_t ebo_size = batch->max_quads * 6 * sizeof(u32);

    u32 *indices = GZZ_MALLOC(ebo_size);
    u32 offset = 0;

    for(u32 i=0; i<max_quads * 6; i+= 6)
    {
        indices[i]   = 0 + offset;
        indices[i+1] = 1 + offset;
        indices[i+2] = 2 + offset;
        indices[i+3] = 2 + offset;
        indices[i+4] = 3 + offset;
        indices[i+5] = 0 + offset;

        offset += 4;
    }

    glGenVertexArrays(1, &batch->vao);
    glGenBuffers(1, &batch->vbo);
    glGenBuffers(1, &batch->ebo);

    glBindVertexArray(batch->vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_size, indices, GL_STATIC_DRAW);


    size_t vbo_size = batch->max_quads * 4 * sizeof(vertex);
    batch->buffer = GZZ_MALLOC(vbo_size);


    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
    glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, GL_DYNAMIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, batch->max_quads * 4 * sizeof(vertex), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, sizeof(vertex),(const void *)offsetof(vertex,position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(vertex),(const void *)offsetof(vertex,uv));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,1, GL_FLOAT, GL_FALSE, sizeof(vertex),(const void *)offsetof(vertex,texture_index));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    i32 samplers[32];
    for(i32 i=0; i<32 ; i++)
    {
        samplers[i] = i;
    }


    glm_ortho(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f, batch->projection);

    gzz_shader_use(batch->shader);
    glUniform1iv(glGetUniformLocation(batch->shader, "textures"), 32, samplers);

    glUniformMatrix4fv(glGetUniformLocation(batch->shader, "projection"),1, GL_FALSE, &batch->projection[0][0]);
    gzz_shader_use(0);


    return batch;
}


void gzz_batch_renderer_begin(gzz_batch_renderer *batch)
{
    batch->quad_count = 0;
    batch->buffer_ptr = batch->buffer;
}


void gzz_batch_renderer_add_sprite(gzz_batch_renderer *batch, gzz_sprite *sprite)
{
    if(batch->quad_count == batch->max_quads)
    {
        gzz_batch_renderer_end(batch);
        gzz_batch_renderer_flush(batch);
        gzz_batch_renderer_begin(batch);
    }

    f32 texture_index = -1.0f;



    for(u32 i=0; i<32; i++)
    {
        if(batch->textures[i] == sprite->texture_region->texture->id)
        {
            texture_index = (f32)i;
            break;
        }
    }

    if(texture_index == -1.0f)
    {
        if(batch->texture_count < 32)
        {
            batch->textures[batch->texture_count] = sprite->texture_region->texture->id;
            texture_index = (f32)(batch->texture_count++);
        }
        else
        {
            /* TODO: handle this */
        }
    }


    batch->buffer_ptr->position[0] = sprite->position[0];
    batch->buffer_ptr->position[1] = sprite->position[1];
    batch->buffer_ptr->uv[0] = sprite->texture_region->uv[0];
    batch->buffer_ptr->uv[1] = sprite->texture_region->uv[0];
    batch->buffer_ptr->texture_index = texture_index;

    batch->buffer_ptr++;

    batch->buffer_ptr->position[0] = sprite->position[0] + sprite->size[0];
    batch->buffer_ptr->position[1] = sprite->position[1];
    batch->buffer_ptr->uv[0] = sprite->texture_region->uv2[0];
    batch->buffer_ptr->uv[1] = sprite->texture_region->uv[1];
    batch->buffer_ptr->texture_index = texture_index;

    batch->buffer_ptr++;

    batch->buffer_ptr->position[0] = sprite->position[0] + sprite->size[0];
    batch->buffer_ptr->position[1] = sprite->position[1] + sprite->size[1];
    batch->buffer_ptr->uv[0] = sprite->texture_region->uv2[0];
    batch->buffer_ptr->uv[1] = sprite->texture_region->uv2[1];
    batch->buffer_ptr->texture_index = texture_index;

    batch->buffer_ptr++;

    batch->buffer_ptr->position[0] = sprite->position[0];
    batch->buffer_ptr->position[1] = sprite->position[1] + sprite->size[1];
    batch->buffer_ptr->uv[0] = sprite->texture_region->uv[0];
    batch->buffer_ptr->uv[1] = sprite->texture_region->uv2[1];
    batch->buffer_ptr->texture_index = texture_index;

    batch->buffer_ptr++;

    batch->quad_count++;
    batch->performance_info->quad_count++;
}


void gzz_batch_renderer_end(gzz_batch_renderer *batch)
{

    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, batch->quad_count * 4 * sizeof(vertex), batch->buffer);

}

void gzz_batch_renderer_flush(gzz_batch_renderer *batch)
{
    for(u32 i=0; i<batch->texture_count; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, batch->textures[i]);
    }

    gzz_shader_use(batch->shader);
    glBindVertexArray(batch->vao);
    glDrawElements(GL_TRIANGLES, batch->quad_count * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    gzz_shader_use(0);

    batch->performance_info->draw_call_count++;
}