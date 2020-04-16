#version 420 core

layout (location = 0) in vec3 vert_pos;

out vec3 frag_uv;

uniform mat4 projection;
uniform mat4 view;

void main() {
    frag_uv = vert_pos;
    vec4 pos = projection * (mat4(mat3(view))) * -1.0 * vec4(vert_pos, 1.0f);
    gl_Position = pos.xyww;
}
