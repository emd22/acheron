#version 330 core

// https://learnopengl.com/Lighting/Multiple-lights

#define MAX_DIR_LIGHTS 1
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

in vec3 frag_vertex;

in vec3 frag_eye_direction;

in vec2 frag_uv;
in vec3 frag_normal;
in vec3 frag_tangent;
in vec3 frag_bitangent;

out vec4 output_colour;

uniform Material material;
uniform vec3 view_pos;

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_dir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir);

void main() {
    vec3 norm = frag_normal;
    
    vec3 normals_texture;
    normals_texture.xy = 255.0f/128.0f * (vec2(1.0) - texture2D(material.normal, frag_uv).rg) - 1.0;
    normals_texture.z = sqrt(1.0 - dot(normals_texture.xy, normals_texture.xy));
    vec3 n = normalize(norm);
    n = normalize((frag_tangent * normals_texture.x) + (frag_bitangent * normals_texture.y) + (n * normals_texture.z));
    
    vec3 result = vec3(0);
    /*for (int i = 0; i < MAX_DIR_LIGHTS; i++) {
        result += CalcDirectionalLight(dirLights[i], norm, fragEyeDirection);
    }*/
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], n, frag_eye_direction);
    }
    output_colour = vec4(result, 1.0f);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(normal, light_dir), 0.0f);
    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
    // final
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, frag_uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, frag_uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, frag_uv));
    
    specular *= diff;
    
    return (ambient + diffuse + specular);
}

float blinnPhong(vec3 normal, vec3 frag_vertex, vec3 view_pos, vec3 light_dir, float shininess) {
    vec3 eye_dir = normalize(-frag_vertex);
    vec3 half_vector = normalize(light_dir + eye_dir);
    vec3 n = normalize(normal);
    
    return pow(max(dot(n, half_vector), 0.0), shininess);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir) {
    view_dir = normalize(view_dir);
    float radius = 5.0;
    vec3 light_pos = light.position - frag_vertex;
    //float dist = distance(light.position, fragVertex);
    float dist = length(light_pos);
    //dist = dist*dist;
    light_pos = normalize(light_pos);
    vec3 light_dir = normalize(light.position - frag_vertex);
    
    float n_dot_l = max(dot(normalize(normal), light_dir), 0.0);
    vec3 mat_ambient = 0.1*vec3(0.1, 0.1, 0.1);
    vec3 mat_diffuse = (vec3(n_dot_l) * texture(material.diffuse, frag_uv).rgb) * (1.0 - (dist / radius));
    float specularity = blinnPhong(normal, frag_vertex, view_pos, light_dir, 1.0);
    specularity *= (1.0 - (dist / radius));
    
    vec3 colour = mat_ambient + mat_diffuse + vec3(specularity);
    
    //vec3 colour = vec3(specularity);
                  //matSpecular * lightPower * pow(cosAlpha, 5) / (dist*dist);
    return colour;
}

