
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

static const GLfloat vertices[] = {

  //posicao de cada vertice do cubo, cada face segue o padrao:
  //cada face do cubo segue esse padrão:

                           /*
              v1 --------- v2
              |             /|
              |           /  |
              |         /    |
              |        /     |
              |      /       |
              |    /         |
              |  /           |
              v3 --------- v4
              */

  // Frente
  //triângulo 1
  -1.0f, 1.0f, 1.0f, 1.0f,   // v1
   1.0f, 1.0f, 1.0f,  1.0f,  // v2
  -1.0f, -1.0f, 1.0f,  1.0f,  // v3

  //triangulo 2
  1.0f, 1.0f, 1.0f,  1.0f,    // v2
 -1.0f, -1.0f, 1.0f,  1.0f, // v3
  1.0f, -1.0f, 1.0f,   1.0f, // v4

  //direita
  //triângulo 1
  1.0f, 1.0f, 1.0f,  1.0f,   // v2
  1.0f, 1.0f, -1.0f,  1.0f,  // v5
  1.0f, -1.0f, 1.0f,  1.0f,  // v4

  //triângulo 2
  1.0f, 1.0f, -1.0f, 1.0f,   // v5
  1.0f, -1.0f, 1.0f, 1.0f,   // v4
  1.0f, -1.0f, -1.0f, 1.0f,  // v6

  //trás
  //triângulo 1
  1.0f, 1.0f, -1.0f, 1.0f,   // v5
  -1.0, 1.0f, -1.0f, 1.0f,  // v7
  1.0f, -1.0f, -1.0f, 1.0f,  // v6

  //triângulo 2
  -1.0f, 1.0f, -1.0f, 1.0f,  // v7
  1.0f, -1.0f, -1.0f, 1.0f,  // v6
  -1.0f, -1.0f, -1.0f, 1.0f, // v8

  //esquerda
  -1.0f, 1.0f, -1.0f, 1.0f,  // v7
  -1.0, 1.0f, 1.0f, 1.0f, //v1
  -1.0f, -1.0f, -1.0f,  1.0f, // v8

  -1.0, 1.0f, 1.0f, 1.0f, //v1
  -1.0f, -1.0f, -1.0f,  1.0f, // v8
  -1.0f, -1.0f, 1.0f,  1.0f, // v3

  //cima
  -1.0f, 1.0f, -1.0f,  1.0f, // v7
  1.0f, 1.0f, -1.0f,  1.0f,  // v5
  -1.0, 1.0f, 1.0f,  1.0f,//v1

   1.0f, 1.0f, -1.0f,  1.0f,  // v5
  -1.0, 1.0f, 1.0f,  1.0f, //v1
  1.0f, 1.0f, 1.0f,  1.0f,   // v2

  //baixo
  1.0f, -1.0f, -1.0f,  1.0f,  // v6
  -1.0f, -1.0f, -1.0f,  1.0f,// v8
  1.0f, -1.0f, 1.0f,    1.0f,// v4

  -1.0f, -1.0f, -1.0f,  1.0f,// v8
  1.0f, -1.0f, 1.0f, 1.0f,   // v4
  -1.0f, -1.0f, 1.0f,  1.0f, // v3

};

static const GLfloat colors[] = {
    //DEFINE VERMELHO COMO COR SÓLIDA
    //frente 
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    //trás 
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    //direita
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    //esquerda
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    //topo
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    //base
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
};


static const GLfloat normals[] = {
    // Frente (z = +1)
    0.0f, 0.0f, 1.0f,  
    0.0f, 0.0f, 1.0f,  
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  
    0.0f, 0.0f, 1.0f,  
    0.0f, 0.0f, 1.0f,

    // Direita (x = +1)
    1.0f, 0.0f, 0.0f,  
    1.0f, 0.0f, 0.0f,  
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  
    1.0f, 0.0f, 0.0f,  
    1.0f, 0.0f, 0.0f,

    // Trás (z = -1)
    0.0f, 0.0f, -1.0f,  
    0.0f, 0.0f, -1.0f,  
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,  
    0.0f, 0.0f, -1.0f,  
    0.0f, 0.0f, -1.0f,
    
    // Esquerda (x = -1)
    -1.0f, 0.0f, 0.0f,  
    -1.0f, 0.0f, 0.0f,  
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,  
    -1.0f, 0.0f, 0.0f,  
    -1.0f, 0.0f, 0.0f,

    // Topo (y = +1)
    0.0f, 1.0f, 0.0f,  
    0.0f, 1.0f, 0.0f,  
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 
    0.0f, 1.0f, 0.0f,  
    0.0f, 1.0f, 0.0f,

    // Base (y = -1)
    0.0f, -1.0f, 0.0f,  
    0.0f, -1.0f, 0.0f,  
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,  
    0.0f, -1.0f, 0.0f,  
    0.0f, -1.0f, 0.0f
};

GLint createPlane(GLfloat **vertex, GLboolean colorFlag, GLfloat **color,  GLboolean normalFlag, GLfloat **normal, GLboolean texelFlag, GLfloat **texel) {
  int vertexSize = sizeof(vertices);  // 1 face * 2 triangles * 3 points * 4 floats * 4 (sizeof(float))
  int colorSize = 0;
  int normalSize = 0;

	fprintf(stdout, "plane::start\n");
	fflush(stdout);

  if(colorFlag == GL_TRUE)
    colorSize = sizeof(colors);   // 1 face * 2 triangles * 3 points * 3 floats * 4 (sizeof(float))

  if(normalFlag == GL_TRUE)
    normalSize = sizeof(normals);   // 1 face * 2 triangles * 3 points * 3 floats * 4 (sizeof(float))

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


	fprintf(stdout, "plane::end\n");
	fflush(stdout);
  return numVertices ;
}

