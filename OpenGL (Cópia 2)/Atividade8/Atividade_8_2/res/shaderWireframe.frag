#version 430 core

in vec4 vColor;
in vec2 vTexCoord; //vetor da textura, coordenada
uniform sampler2D tex;
out vec4 Corfinal;

void main(void) {
    vec3 lightColor = vColor.rgb;
    vec3 textureColor = texture(tex, vTexCoord).rgb;
    vec3 resultingColor = lightColor * textureColor; 
    Corfinal = vec4(resultingColor, 1.0);
}