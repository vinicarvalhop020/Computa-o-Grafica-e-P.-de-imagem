#version 430 core

layout(location = 0) in vec4 vertex;
layout(location = 1) uniform vec4 u_color;
layout(location = 2) uniform mat4 u_projMatrix;
layout(location = 3) uniform mat4 u_modelViewMatrix;

out vec4    vColor; // Outgoing varying data sent to the fragment shader

void main(void) {

	vec4 xVertex = u_modelViewMatrix * vertex;
 	gl_Position = u_projMatrix * xVertex;
  	vColor = u_color; 
}
