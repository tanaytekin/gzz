#include "texture2d.h"
#include "log.h"


#include <glad/glad.h>
#include <stb_image.h>


gzz_texture2d gzz_texture2d_create(const char *path)
{
    stbi_set_flip_vertically_on_load(GZZ_TRUE);

    gzz_texture2d texture = {0};
    i32 width = 0;
    i32 height = 0;
    i32 channel_count = 0;

    unsigned char *data = stbi_load(path, &width, &height, &channel_count, 0);
    if(!data)
    {
        GZZ_LOG_ERROR("Failed to load texture : %s", path);
        return texture;
    }

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    if(channel_count == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        texture.has_alpha = GZZ_FALSE;
    }
    else if(channel_count == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        texture.has_alpha = GZZ_TRUE;
    }
    else
    {
        /* TODO: Handle this. */
    }


    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);


    texture.width = (u32) width;
    texture.height = (u32) height;

    stbi_image_free(data);

    GZZ_LOG_INFO("Texture : %s is created.", path);
    return texture;
}
