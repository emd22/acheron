#version 330 core

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec2 vertUV;
layout(location = 2) in vec3 vertNormal;

uniform mat4 MVP;
uniform mat4 M;

out vec3 fragPos;
out vec2 fragUV;
out vec3 fragNormal;

void main() {
    fragPos = vec3(M * vec4(vertPosition, 1.0));
    fragNormal = mat3(transpose(inverse(M))) * vertNormal;
    fragUV = vertUV;

    gl_Position = MVP * vec4(vertPosition, 1.0);
}
