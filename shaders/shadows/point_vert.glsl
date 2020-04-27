#version 420 core

layout(location = 0) in vec3 vert_position;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main() {
    gl_Position = m*vec4(vert_position, 1.0f);
}
