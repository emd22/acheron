#version 420 core

in vec3 frag_uv;

out vec4 output_colour;

uniform samplerCube skybox;

void main() {
    output_colour = vec4(texture(skybox, frag_uv).rgb, 1.0f);
}
