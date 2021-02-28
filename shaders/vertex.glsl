#version 460 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_color;

out vec4 o_color;


uniform mat4 projection;





void main()
{
    o_color = a_color;
    gl_Position = projection * vec4(a_position,0.0,1.0);
}