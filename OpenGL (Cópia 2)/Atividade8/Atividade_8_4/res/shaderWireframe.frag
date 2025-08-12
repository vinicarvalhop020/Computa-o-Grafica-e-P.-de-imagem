#version 430 core

in vec4 vColor;

layout (location = 0) out vec4 myOutputColor;

void main (void) {
    myOutputColor = vColor;
}