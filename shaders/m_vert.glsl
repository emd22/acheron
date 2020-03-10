#version 400 core

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec2 vertUV;
layout(location = 2) in vec3 vertNormal;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;

out vec2 fragUV;
out vec3 fragNormal;
out vec3 fragPos;
out vec3 fragLightPos_world;
out vec3 fragPos_world;

void main() {
    gl_Position = MVP * vec4(vertPosition, 1.0);
    fragPos = vertPosition;
    fragLightPos_world = vec3(5, 5, 5);
    fragPos_world = (M*vec4(vertPosition, 1.0f)).xyz;
    fragUV = vertUV;
    fragNormal = normalize(vertNormal);
}
