#version 400 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 MVP;
out vec2 UV;
out vec3 Normal;
out vec3 fragPos;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1.0);
    fragPos = vec3(vec4(vertexPosition, 1.0));
    UV = vertexUV;
    Normal = vertexNormal;
}
