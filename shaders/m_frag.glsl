#version 330 core

// https://learnopengl.com/Lighting/Multiple-lights

#define MAX_DIR_LIGHTS 2
#define MAX_POINT_LIGHTS 4

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

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragPos;
in vec2 fragUV;
in vec3 fragNormal;

out vec4 outputColour;

uniform Material material;
uniform vec3 viewPos;

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos-fragPos);
    vec3 result = vec3(0, 0, 0);
    for (int i = 0; i < MAX_DIR_LIGHTS; i++) {
        result += CalcDirectionalLight(dirLights[i], norm, viewDir);
    }
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, viewDir);
    }
    outputColour = vec4(result, 1.0f);
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
    
    specular *= diff;
    
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    // attenuation
    float dist = length(light.position - fragPos);
    float attenuation = light.constant / ((1+light.linear * dist)*(1+light.quadratic * dist * dist));
    // final
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragUV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fragUV));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

