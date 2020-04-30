#version 420 core

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec2 vert_uv;
layout(location = 2) in vec3 vert_normal;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec2 frag_uv;
out vec3 frag_normal;
out vec3 frag_vertex;

void main() {
    mat4 mvp = p*v*m;
    // vertex position
    frag_vertex = (m*vec4(vert_position, 1.0)).xyz;
    frag_uv = vert_uv;
    
    frag_normal = (transpose(inverse(m)) * vec4(vert_normal, 0.0)).xyz;

    gl_Position = mvp * vec4(vert_position, 1.0);
}
