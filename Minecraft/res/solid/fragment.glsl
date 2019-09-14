#version 330 core

layout (location = 0) out vec4 color;

in vec4 pass_vertex_color;
in vec4 pass_position;

void main() {
    color = pass_vertex_color;
}
