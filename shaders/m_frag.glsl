#version 400 core

out vec3 outputColour;

in vec2 UV;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D textureSampler;

void main() {
    // ambient
    vec3 lightPos = vec3(5, 5, 5);
    vec3 lightColour = vec3(1, 1, 1);
    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength*lightColour;
    // diffuse
    
    vec3 norm = normalize(normal);
    vec3 lightDir = -lightPos;
    float lightIntensity = (dot(normal, lightDir));
    //brightness = clamp(brightness, 0, 1);
    
    vec3 result = vec3(0.75, 0, 0.25);
    outputColour = clamp((vec3(1, 1, 1) ), 0.0, 1.0f)*result;
}
