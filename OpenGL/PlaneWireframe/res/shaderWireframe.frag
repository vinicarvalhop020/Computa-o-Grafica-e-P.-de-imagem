#version 430 core


in   vec4    vColor; // incomming varying data to the
                     // fragment shader
                     // sent from the vertex shader

layout (location = 0) out vec4 myOutputColor;

void main (void) { 

    myOutputColor = vColor;
  
} 
