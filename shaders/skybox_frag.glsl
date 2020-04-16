#version 420 core

out vec4 output_colour;

in vec3 frag_uv;

uniform samplerCube skybox;

void main() {  
    output_colour = texture(skybox, frag_uv);
}
