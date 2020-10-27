#version 420 core

in vec3 frag_vertex;
in vec2 frag_uv;
in vec3 frag_normal;

struct Material {
    bool use_normalmap;
    bool use_specularmap;
    bool use_diffuse;
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
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float radius;
    
    samplerCube shadow_map;
    float shadow_far_plane;
    bool shadows_enabled;
};

uniform Material material;
uniform vec3 view_pos;

uniform DirectionalLight dirLights[1];
uniform PointLight pointLights[4];

uniform sampler2D image;

layout(location = 0) out vec4 output_colour;

void main() {
    vec3 result = vec3(1, 0, 1);
    if (material.use_diffuse) {
        result = vec3(texture(material.diffuse, frag_uv));
    }
    output_colour = vec4(result, 1.0f);
}
