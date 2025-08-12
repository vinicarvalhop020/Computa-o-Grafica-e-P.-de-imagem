#version 430 core

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

// Uniforms de matrizes
uniform mat4 u_projMatrix;
uniform mat4 u_modelViewMatrix;
uniform mat4 u_modelMatrix;

// saidas
// Passa esses "valores" para o Fragment Shader
out vec3 v_Normal;
out vec3 v_FragPos;
out vec3 v_Color;

void main(void) {
    v_FragPos = vec3(u_modelMatrix * vertex);
    v_Normal = normalize(mat3(transpose(inverse(u_modelMatrix))) * aNormal);
    v_Color = aColor;
    gl_Position = u_projMatrix * u_modelViewMatrix * vertex;
}