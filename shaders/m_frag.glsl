#version 400 core

out vec3 outputColour;

in vec2 UV;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D textureSampler;

void main() {
    // ambient
    vec3 lightPos = vec3(2, 2, 2);
    vec3 lightColour = vec3(1, 1, 1);
    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength*lightColour;
    // diffuse
    
    vec3 norm = normalize(normal);
    vec3 lightDir = lightPos-fragPos;
    float brightness = dot(normal, lightDir) / (length(lightDir)*length(normal));
    //brightness = clamp(brightness, 0, 1);
    
    vec3 result = vec3(0.75, 0, 0.25);
    outputColour = vec3(result);
}
