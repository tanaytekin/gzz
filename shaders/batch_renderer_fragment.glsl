#version 460 core

in vec2 uv;
in float texture_index;

uniform sampler2D textures[32];

out vec4 frag_color;

void main()
{
    int index = int(texture_index);

    frag_color = texture(textures[index], uv);
}