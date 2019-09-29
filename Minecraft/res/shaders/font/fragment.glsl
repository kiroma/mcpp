#version 330 core

layout (location = 0) out vec4 color;

in vec2 pass_texture_coords;
uniform sampler2D main_texture;

void main(void)
{
    color = texture(main_texture, pass_texture_coords);
}
