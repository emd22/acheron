#version 420 core

in vec2 frag_uv;
in vec4 frag_shadow_coord;

layout(location = 0) out vec3 output_colour;

uniform sampler2D texture_sampler;
uniform sampler2DShadow shadow_map;

void main() {
    vec3 light_colour = vec3(1, 1, 1);
    vec3 diffuse_colour = texture(texture_sampler, frag_uv).rgb;
    float visibility = texture(shadow_map, vec3(frag_shadow_coord.xy, (frag_shadow_coord.z)/frag_shadow_coord.w));
    
    output_colour = visibility * diffuse_colour * light_colour;
}
