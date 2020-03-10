#version 400 core

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragPos;
in vec3 fragLightPos_world;
in vec3 fragPos_world;

// https://learnopengl.com/Lighting/Multiple-lights

#define MAX_DIRECITONAL_LIGHTS 8

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec3 outputColour;

uniform sampler2D textureSampler;
uniform Material material;
uniform DirectionalLight dirLight;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(fragPos_world-fragPos);
    vec3 result = CalcDirectionalLight(dirLight, norm, viewDir);
    
    outputColour = result;
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    // final
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragUV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fragUV));
    return (ambient + diffuse + specular);
}





