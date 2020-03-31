#version 420 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;
layout (location = 2) in vec3 vert_normal;

uniform mat4 depth_mvp;
uniform mat4 depth_bias;

out vec4 frag_shadow_coord;
out vec2 frag_uv;

void main() {
    gl_Position = depth_mvp * vec4(vert_pos, 1);
    frag_shadow_coord = depth_bias * vec4(vert_pos, 1);
    frag_uv = vert_uv;
}
