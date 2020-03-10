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
out vec3 fragEyeDirection;
out vec3 fragLightDirection;
out vec3 fragLightPos_world;
out vec3 fragPos_world;

void main() {
    gl_Position = MVP * vec4(vertPosition, 1.0);
    fragPos = vec3(vec4(vertPosition, 1.0));
    vec3 vertexPosition = (V * M * vec4(vertPosition, 1)).xyz;
    fragEyeDirection = vec3(0, 0, 0)-vertexPosition;
    fragLightPos_world = vec3(5, 5, 5);
    vec3 lightPosition = (V * vec4(fragLightPos_world, 1)).xyz;
    fragPos_world = (M*vec4(vertPosition, 1.0f)).xyz;
    fragLightDirection = lightPosition + fragEyeDirection;
    fragUV = vertUV;
    fragNormal = normalize(vertNormal);
}
