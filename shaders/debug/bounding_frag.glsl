#version 420 core

in vec3 frag_vertex;

layout(location = 0) out vec4 output_colour;

uniform vec3 view_pos;

void main() {
    vec3 result = vec3(1, 0, 0);
    output_colour = vec4(result, 1.0f);
}
