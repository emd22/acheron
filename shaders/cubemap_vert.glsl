#version 420 core

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec2 vert_uv;

out vec2 frag_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    frag_uv = vert_uv;
    gl_Position = projection * view * model * vec4(vert_position, 1.0f);
}
