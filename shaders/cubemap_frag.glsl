#version 420 core

in vec2 frag_uv;

out vec4 output_colour;

uniform sampler2D cubemap_texture;

void main() {
    output_colour = texture(cubemap_texture, frag_uv);
}
