#version 400 core

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragPos;
in vec3 fragEyeDirection;
in vec3 fragLightDirection;
in vec3 fragLightPos_world;
in vec3 fragPos_world;

out vec3 outputColour;

uniform sampler2D textureSampler;

void main() {
    // ambient
    vec3 lightColour = vec3(1, 1, 0.50);
    float lightPower = 50.0f;
    
    vec3 materialDiffuseColour = vec3(1, 1, 1);
    vec3 materialAmbientColour = materialDiffuseColour*0.1f;
    
    float lightDistance = length(fragLightPos_world - fragPos_world);

    vec3 norm = normalize(fragNormal);
    vec3 lightd = normalize(fragLightDirection);
    
    float cosTheta = clamp(dot(norm, lightd), 0, 1);
    
    vec3 eyevec = normalize(fragEyeDirection);
    vec3 refl = reflect(-lightd, norm);
    
    float cosAlpha = clamp(dot(eyevec, refl), 0, 1);
    
    outputColour = 
        materialAmbientColour +
        materialDiffuseColour * lightColour * lightPower * cosTheta / (lightDistance*lightDistance);
}
