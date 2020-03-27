#version 330 core

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec2 vertUV;
layout(location = 2) in vec3 vertNormal;
layout(location = 3) in vec3 vertTangent;
layout(location = 4) in vec3 vertBitangent;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragEyeDirection;
out vec2 fragUV;
out vec3 fragNormal;
out vec3 fragVertex;
out vec3 fragTangent;
out vec3 fragBitangent;

void main() {
    mat4 MVP = P*V*M;
    // vertex position
    fragVertex = (M*vec4(vertPosition, 1.0)).xyz;
    vec3 eyeDirection = vec3(0.0f, 0.0f, 0.0f)-fragVertex;

    fragUV = vertUV;
    
    fragEyeDirection = (vec4(eyeDirection, 1.0f)).xyz;
    fragNormal = (transpose(inverse(M)) * vec4(vertNormal, 0.0)).xyz;
    
    vec3 c1 = cross(vertNormal, vec3(0.0, 0.0, 1.0));
    vec3 c2 = cross(vertNormal, vec3(0.0, 1.0, 0.0));
    
    if (length(c1) > length(c2))
        fragTangent = c1;
    else
        fragTangent = c2;
    
    fragTangent = normalize(fragTangent);
    fragBitangent = normalize(cross(fragNormal, fragTangent));

    gl_Position = MVP * vec4(vertPosition, 1.0);
}
