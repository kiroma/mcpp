#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoords;

out vec4 pass_vertex_color;
out vec4 pass_position;
out vec2 pass_texture_coords;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(void) {
    pass_vertex_color = mod(position.zxyw, 2);
    pass_position = position;
    pass_texture_coords = textureCoords;
    gl_Position = projection_matrix * view_matrix * position;
}
