
/*
 * plane.c
 *
 * Create a plane
 *
 *  Created on: January 29 2025
 *      Author: JMario
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>

#include <glad/glad.h>
#include "cube.h"


// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const GLfloat vertices[] = {
    // Triângulo 1
    -1.0f, -1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    // Triângulo 2
    -1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    // Triângulo 3
    -1.0f, -1.0f, -1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 1.0f,
    // Triângulo 4
    -1.0f,  1.0f, -1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 1.0f,
    // Triângulo 5
     1.0f, -1.0f, -1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f,
    // Triângulo 6
     1.0f, -1.0f, -1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    // Triângulo 7
    -1.0f, -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,
    // Triângulo 8
    -1.0f, -1.0f, -1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,
    // Triângulo 9
    -1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    // Triângulo 10
    -1.0f,  1.0f, -1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
    // Triângulo 11
    -1.0f, -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f,
    // Triângulo 12
    -1.0f, -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f
   };


static const GLfloat colors[] = {
  // triangle 1 (front)
   1.0f,  0.0f,  0.0f, 1.0f, 
   0.0f,  1.0f,  0.0f, 1.0f,
   0.0f,  0.0f,  1.0f, 1.0f,

   // triangle 2 (front)
   1.0f,  1.0f,  0.0f, 1.0f,  
   1.0f,  0.0f,  1.0f, 1.0f,
   0.0f,  1.0f,  1.0f, 1.0f
   };

static const GLfloat normals[] = {
  // triangle 1 (front)
   0.0f,  0.0f,  1.0f,  
   0.0f,  0.0f,  1.0f,
   0.0f,  0.0f,  1.0f, 

   // triangle 2 (front)
   0.0f,  0.0f,  1.0f,  
   0.0f,  0.0f,  1.0f,
   0.0f,  0.0f,  1.0f,
   };

static const GLfloat texels[] = {
  // triangle 1 (front)
  0.0f, 0.0f,    
  1.0f, 0.0f,  
  1.0f, 1.0f, 

  // triangle 2 (front)
  0.0f, 1.0f, 
  0.0f, 0.0f,     
  1.0f, 1.0f,  
};


GLint createPlane(GLfloat **vertex, GLboolean colorFlag, GLfloat **color,  GLboolean normalFlag, GLfloat **normal, GLboolean texelFlag, GLfloat **texel) {
  int vertexSize = sizeof(vertices);  // 1 face * 2 triangles * 3 points * 4 floats * 4 (sizeof(float))
  int colorSize = 0;
  int normalSize = 0;
  int texelSize = 0;

	fprintf(stdout, "plane::start\n");
	fflush(stdout);

  if(colorFlag == GL_TRUE)
    colorSize = sizeof(colors);   // 1 face * 2 triangles * 3 points * 3 floats * 4 (sizeof(float))

  if(normalFlag == GL_TRUE)
    normalSize = sizeof(normals);   // 1 face * 2 triangles * 3 points * 3 floats * 4 (sizeof(float))

  if(texelFlag == GL_TRUE)
    texelSize = sizeof(texels);   // 1 face * 2 triangles * 3 points * 2 floats * 4 (sizeof(float))

 // GLfloat *cubeInfo = malloc(size);
  *vertex = (GLfloat *) malloc(vertexSize);
  int numVertices = vertexSize / (4 * sizeof(GLfloat));
  int numElements = vertexSize / sizeof(GLfloat);

  for(int i = 0; i < numElements; i++) {
    (*vertex)[i] = vertices[i];
  }
// fprintf(stdout, "planeInfo %f\n", cubeInfo[0]);
//  fprintf(stdout, "plane %f", *plane[0]);

  if(colorFlag == GL_TRUE) {
    *color = (GLfloat *) malloc(colorSize);
    numElements = colorSize / sizeof(GLfloat);
    for(int i = 0; i < numElements; i++) {
    (*color)[i] = colors[i];
    }
  }
  if(normalFlag == GL_TRUE) {
    *normal = (GLfloat *) malloc(normalSize);
     numElements = normalSize / sizeof(GLfloat);
    for(int i = 0; i < numElements; i++) {
    (*normal)[i] = normals[i];
    }
  }


  if(texelFlag == GL_TRUE) { 
    *texel = (GLfloat *) malloc(texelSize);
    numElements = texelSize / sizeof(GLfloat);
    for(int i = 0; i < numElements; i++) {
      (*texel)[i] = texels[i];
    }
  }

	fprintf(stdout, "plane::end\n");
	fflush(stdout);
  return numVertices ;
}

