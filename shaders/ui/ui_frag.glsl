#version 420 core

in vec3 frag_vertex;
in vec2 frag_uv;
in vec3 frag_normal;

uniform texture2D image;

layout(location = 0) out vec4 output_colour;

void main() {
    vec4 result = texture(image, frag_uv);
    output_colour = vec4(result, 1.0f);
}
