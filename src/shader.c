#include "shader.h"
#include "log.h"

#include <glad/glad.h>

#include <stdio.h>



typedef enum gzz_shader_type
{
    GZZ_SHADER_VERTEX,
    GZZ_SHADER_FRAGMENT,
    GZZ_SHADER_PROGRAM
}gzz_shader_type;



static char *gzz_shader_read_file_to_str(const char *path)
{
    FILE *fp;
    fp = fopen(path, "r");

    if(fp == NULL)
    {
        GZZ_LOG_ERROR("Failed to open file : %s", path);
        return NULL;
    }

    size_t size;
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char *str;
    str = GZZ_MALLOC(size + 1);
    str[size] = '\0';

    fread(str, size, 1, fp);

    fclose(fp);

    return str;
}

static b32 gzz_shader_check_errors(gzz_shader_type type, u32 id, const char *path)
{
    i32 success = 0;
    i32 info_log_length = 0;
    char *info_log = NULL;

    /* If it is a shader program */
    if (type == GZZ_SHADER_PROGRAM)
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
            info_log = GZZ_MALLOC(info_log_length+1);
            info_log[info_log_length] = '\0';
            glGetProgramInfoLog(id, info_log_length, NULL, info_log);
            GZZ_LOG_ERROR("Shader program linking error\n%s",info_log);
            GZZ_FREE(info_log);
            return GZZ_FALSE;
        }
    }
    else
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
            info_log = GZZ_MALLOC(info_log_length+1);
            info_log[info_log_length] = '\0';
            glGetShaderInfoLog(id, info_log_length, NULL, info_log);
            if(type == GZZ_SHADER_VERTEX)
            {
                GZZ_LOG_ERROR("Vertex shader error : %s\n%s", path, info_log);
            }
            else if(type == GZZ_SHADER_FRAGMENT)
            {
                GZZ_LOG_ERROR("Fragment shader error : %s\n%s", path, info_log);
            }
            GZZ_FREE(info_log);
            return GZZ_FALSE;
        }

    }

    return GZZ_TRUE;
}

u32 gzz_shader_create(const char *vertex_path, const char *fragment_path) {
    char *vertex_str;
    char *fragment_str;

    vertex_str = gzz_shader_read_file_to_str(vertex_path);
    fragment_str = gzz_shader_read_file_to_str(fragment_path);


    /* If shader sources are NULL */
    if (vertex_str == NULL) {
        if (fragment_str != NULL)
            GZZ_FREE(fragment_str);
        return 0;
    }
    if (fragment_str == NULL) {
        if (vertex_str != NULL)
            GZZ_FREE(vertex_str);
        return 0;
    }

    /* Vertex shader creation */
    u32 vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar *const *) &vertex_str, NULL);
    glCompileShader(vertex);


    /* Fragment shader creation */
    u32 fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar *const *) &fragment_str, NULL);
    glCompileShader(fragment);

    if (!gzz_shader_check_errors(GZZ_SHADER_VERTEX, vertex, vertex_path)) {
        glDeleteShader(vertex);
        GZZ_FREE(vertex_str);
        GZZ_FREE(fragment_str);
        return 0;
    }

    if (!gzz_shader_check_errors(GZZ_SHADER_FRAGMENT, fragment, fragment_path)) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        GZZ_FREE(vertex_str);
        GZZ_FREE(fragment_str);
        return 0;
    }

    u32 program;
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    if (!gzz_shader_check_errors(GZZ_SHADER_PROGRAM, program, NULL)) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        GZZ_FREE(vertex_str);
        GZZ_FREE(fragment_str);
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    GZZ_FREE(vertex_str);
    GZZ_FREE(fragment_str);

    return program;
}

void gzz_shader_destroy(u32 shader)
{
    glDeleteProgram(shader);
}

void gzz_shader_use(u32 shader)
{
    glUseProgram(shader);
}

void gzz_shader_set_mat4(u32 shader, const char *name, mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, &mat[0][0]);
}
