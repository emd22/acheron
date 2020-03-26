#version 330 core

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec2 vertUV;
layout(location = 2) in vec3 vertNormal;
layout(location = 3) in vec3 vertTangent;
layout(location = 4) in vec3 vertBitangent;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragPos;

out vec3 fragLightDirection;
out vec3 fragEyeDirection;

out vec2 fragUV;
out vec3 fragNormal;
out mat3 fragTBN;

void main() {
    mat4 MVP = P*V*M;
    
    vec3 vertexPosition = (V*M*vec4(vertPosition, 1)).xyz;
    vec3 eyeDirection = vec3(0.0f, 0.0f, 0.0f)-vertexPosition;
    
    vec3 lightPosition = (V*vec4(0.0f, 3.0f, 0.0f, 1.0f)).xyz;
    vec3 lightDirection = lightPosition+eyeDirection;
    
    vec3 T = (V*M*vec4(vertTangent, 1.0f)).xyz;
    vec3 N = (V*M*vec4(vertNormal, 1.0f)).xyz;
    vec3 B = (V*M*vec4(vertBitangent, 1.0f)).xyz;
    mat3 TBN = transpose(mat3(T, B, N));

    fragPos = (M * vec4(vertPosition, 1)).xyz;
    fragNormal = mat3(transpose(inverse(M))) * vertNormal;
    fragUV = vertUV;
    
    fragEyeDirection = TBN*eyeDirection;
    fragLightDirection = lightDirection;

    gl_Position = MVP * vec4(vertPosition, 1.0);
}
