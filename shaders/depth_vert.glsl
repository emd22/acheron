#version 420 core

layout(location = 0) in vec3 vert_position;

uniform mat4 depth_mvp;

void main() {
    gl_Position = depth_mvp*vec4(vert_position, 1.0f);
}
