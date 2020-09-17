#version 420 core

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec2 vert_uv;
layout(location = 2) in vec3 vert_normal;
layout(location = 3) in vec3 vert_tangent;
layout(location = 4) in vec3 vert_bitangent;

uniform mat4 m;
uniform mat4 mvp;

uniform mat4 shadow_bias;

out vec3 frag_eye_direction;

out vec4 frag_shadow_coords;

out vec2 frag_uv;
out vec3 frag_normal;
out vec3 frag_vertex;
out vec3 frag_tangent;
out vec3 frag_bitangent;

void main() {
    // vertex position
    frag_vertex = (m*vec4(vert_position, 1.0)).xyz;
    vec3 eye_direction = vec3(0.0f, 0.0f, 0.0f)-frag_vertex;

    frag_uv = vert_uv;
    
    frag_eye_direction = (vec4(eye_direction, 1.0f)).xyz;
    frag_normal = (transpose(inverse(m)) * vec4(vert_normal, 0.0)).xyz;
    
    vec3 c1 = cross(vert_normal, vec3(0.0, 0.0, 1.0));
    vec3 c2 = cross(vert_normal, vec3(0.0, 1.0, 0.0));
    
    if (length(c1) > length(c2))
        frag_tangent = c1;
    else
        frag_tangent = c2;
    
    frag_tangent = normalize(frag_tangent);
    frag_bitangent = normalize(cross(frag_normal, frag_tangent));
    
    frag_shadow_coords = (shadow_bias*m)*vec4(vert_position, 1.0);

    gl_Position = mvp * vec4(vert_position, 1.0);
}
