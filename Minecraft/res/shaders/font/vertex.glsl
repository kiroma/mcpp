#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoords;

out vec2 pass_texture_coords;
uniform mat4 matrix;

void main(void)
{
    pass_texture_coords = textureCoords;
    gl_Position = matrix * vec4(position.xy, 0, 1);
}
