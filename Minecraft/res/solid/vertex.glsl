#version 330 core

layout (location = 0) in vec4 position;

out vec4 pass_vertex_color;
out vec4 pass_position;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(void) {
    pass_vertex_color = mod(position.zxyw, 2);
    pass_position = position;
    gl_Position = projection_matrix * view_matrix * position;
}
