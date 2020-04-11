#version 420 core

layout(location = 0) out float frag_depth;

void main() {
    frag_depth = gl_FragCoord.z;
}
