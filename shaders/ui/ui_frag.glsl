#version 420 core

in vec3 frag_vertex;
in vec2 frag_uv;
in vec3 frag_normal;

uniform sampler2D image;

layout(location = 0) out vec4 output_colour;

void main() {
    vec3 result = vec3(1);
    output_colour = vec4(result, 1.0f);
}
