#version 420 core

layout(location = 0) in vec3 vert_position;

out vec3 frag_uv;

uniform mat4 projection;
uniform mat4 view;

void main() {
    frag_uv = vert_position;
    gl_Position = projection * mat4(mat3(view)) * vec4(vert_position, 1.0f);
}
