#version 400 core

in vec2 UV;
out vec3 outputColour;

uniform sampler2D textureSampler;

void main(){
    outputColour = texture(textureSampler, UV).rgb;
}
