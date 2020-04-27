#version 420 core

in vec4 frag_pos;

uniform vec3 light_pos;
uniform float far_plane;

void main() {
    float light_distance = length(frag_pos.xyz - light_pos);
    light_distance /= 50.0;
    gl_FragDepth = light_distance;
}
