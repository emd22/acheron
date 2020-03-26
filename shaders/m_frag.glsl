#version 330 core

// https://learnopengl.com/Lighting/Multiple-lights

#define MAX_DIR_LIGHTS 2
#define MAX_POINT_LIGHTS 4

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
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

in vec3 fragLightDirection;
in vec3 fragEyeDirection;

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
    //255.0f/128.0f
    vec3 norm = normalize(255.0f/128.0f * texture(material.normal, fragUV).rgb-1.0f);
    //vec3 norm = fragNormal;
    //vec3 viewDir = normalize((viewPos-fragPos));
    vec3 result = vec3(0, 0, 0);
    /*for (int i = 0; i < MAX_DIR_LIGHTS; i++) {
        result += CalcDirectionalLight(dirLights[i], norm, fragEyeDirection);
    }*/
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, fragEyeDirection);
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
    viewDir = normalize(viewDir);
    vec3 lightDir = normalize(fragLightDirection);
    float dist = length(light.position - fragPos);
    float cosTheta = clamp(dot(normal, lightDir), 0, 1);

    vec3 matDiffuse = max(texture(material.diffuse, fragUV).rgb, 0.0f);
    vec3 matAmbient = 0.1*vec3(0.1, 0.1, 0.1);
    
    vec3 colour = matAmbient +
                  matDiffuse * vec3(1, 1, 1) * 1 * cosTheta / (dist*dist);
                  //matSpecular * lightPower * pow(cosAlpha, 5) / (dist*dist);
    
    return colour;
}

