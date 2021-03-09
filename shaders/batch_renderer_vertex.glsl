#version 460 core

layout (location = 0) in vec2  a_position;
layout (location = 1) in vec2  a_uv;
layout (location = 2) in float a_texture_index;

uniform mat4 projection;

out vec2 uv;
out float texture_index;

void main()
{
    uv = a_uv;
    texture_index = a_texture_index;

    gl_Position = projection * vec4(a_position,1.0,1.0);
}