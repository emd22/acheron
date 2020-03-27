#version 330 core

// https://learnopengl.com/Lighting/Multiple-lights

#define MAX_DIR_LIGHTS 2
#define MAX_POINT_LIGHTS 2

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

in vec3 fragVertex;

in vec3 fragEyeDirection;

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragTangent;
in vec3 fragBitangent;

out vec4 outputColour;

uniform Material material;
uniform vec3 viewPos;

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);

void main() {
    //vec3 norm = normalize(255.0f/128.0f * texture(material.normal, fragUV).rgb-1.0f);
    vec3 norm = fragNormal;
    
    vec3 normalsTexture;
    normalsTexture.xy = 2.0 * (vec2(1.0) - texture2D(material.normal, fragUV).rg) - 1.0;
    normalsTexture.z = sqrt(1.0 - dot(normalsTexture.xy, normalsTexture.xy));
    vec3 n = normalize(norm);
    n = normalize((fragTangent * normalsTexture.x) + (fragBitangent * normalsTexture.y) + (n * normalsTexture.z));
    
    vec3 result = vec3(0);
    /*for (int i = 0; i < MAX_DIR_LIGHTS; i++) {
        result += CalcDirectionalLight(dirLights[i], norm, fragEyeDirection);
    }*/
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], n, fragEyeDirection);
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

float blinnPhong(vec3 normal, vec3 fragVertex, vec3 viewPos, vec3 lightDir, float shininess) {
    vec3 eyeDir = normalize( - fragVertex);
    vec3 halfVector = normalize(lightDir + eyeDir);
    vec3 n = normalize(normal);
    
    return pow(max(dot(n, halfVector), 0.0), shininess);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    viewDir = normalize(viewDir);
    float radius = 5.0;
    vec3 lightPos = light.position - fragVertex;
    //float dist = distance(light.position, fragVertex);
    float dist = length(lightPos);
    //dist = dist*dist;
    lightPos = normalize(lightPos);
    vec3 lightDir = normalize(light.position - fragVertex);
    
    float nDotL = max(dot(normalize(normal), lightDir), 0.0);
    vec3 matDiffuse = (vec3(nDotL) * texture(material.diffuse, fragUV).rgb) * (1.0 - (dist / radius));
    float specularity = blinnPhong(normal, fragVertex, viewPos, lightDir, 1.0);
    specularity *= (1.0 - (dist / radius));
    
    vec3 matAmbient = 0.1*vec3(0.1, 0.1, 0.1);
    vec3 colour = matAmbient + matDiffuse + vec3(specularity);
    
    //vec3 colour = vec3(specularity);
                  //matSpecular * lightPower * pow(cosAlpha, 5) / (dist*dist);
    return colour;
}

