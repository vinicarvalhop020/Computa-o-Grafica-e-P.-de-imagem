#version 430 core

// entrada cor fragmentada vinda da gpu
in vec4 vColor;

// saida
layout (location = 0) out vec4 myOutputColor;

void main (void) {
    myOutputColor = vColor;
}