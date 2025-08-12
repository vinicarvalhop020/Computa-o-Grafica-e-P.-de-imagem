
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
#define M_PI 3.14159265358979323846



GLfloat* pontos_esfera(float r, int resolucao) {
  //gera os pontos da esfera, utilizando coordenadas esféricas
  //r é o raio da esfera
  //resolution é o número de pontos em cada direção (latitude e longitude)

    GLfloat *vertices = malloc(resolucao * resolucao * 4 * sizeof(GLfloat));
    int index = 0;

    //uma esfera de maior resolucao inteira seria i<360, j<180

    for (int i = 0; i < resolucao; i++) {
        for (int j = 0; j < resolucao; j++) {
            // Calcula os ângulos theta e phi

            //Oi vini, vou deixar escrito esses comentarios para você entender melhor o que está acontecendo

            //seria como dividir uma pizza em fatias, 2pi seria a volta total na pizza, multiplicar por j é pegar a fatia j
            //2pi/resolucao-1 é o angulo (discretizado) de cada fatia (angulo theta)
            //pi/resolucao-1 é o angulo (discretizado) de cada angulo phi
            //Para ter resolucao pontos, precisamos de resolucao-1 intervalos (fatias) entre eles


            float theta = j*(2.0 * M_PI / (resolucao - 1));
            float phi   = i*(M_PI / (resolucao - 1));

            float x = r * sinf(phi) * cosf(theta);
            float y = r * cosf(phi);
            float z = r * sinf(phi) * sinf(theta);

            vertices[index++] = x;
            vertices[index++] = y;
            vertices[index++] = z;
            vertices[index++] = 1.0f; // Componente W
        }
    }
    return vertices;
}

GLfloat* esfera_triangulos(int *totalVertices, int resolucao) {
    float radius = 1.0f; // Raio da esfera
    // Gera os pontos da esfera, seria como uma nuvem de pontos
    GLfloat *p_esfera = pontos_esfera(radius, resolucao);


    //numero de quadrados que compoe a esfera 
    int num_quads = (resolucao - 1) * (resolucao - 1);
    *totalVertices = num_quads * 6; //numero de verticies em cada quadrado (2 triângulos * 3 vértices por triângulo) X numero de quadrados

    GLfloat *triangles = malloc(*totalVertices * 4 * sizeof(GLfloat)); //esse 4 é por conta do W
    int triangle_index = 0;

    // mesma iteracao para criar os pontos da esfera
    
    for (int i = 0; i < resolucao - 1; i++) { //latitude
        for (int j = 0; j < resolucao - 1; j++) { //longitude
        

            // A esfera sera composta por quadrados, cada quadrado é formado por 2 triângulos
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
            // Cada quadrado é formado por 4 vértices, que são os pontos da esfera

            //pega os indices que irao compor o quadrado
            // v1, v2, v3, v4 são os vértices do quadrado
            int v1_idx = (i * resolucao + j) * 4;
            int v2_idx = (i * resolucao + (j + 1)) * 4;
            int v3_idx = ((i + 1) * resolucao + j) * 4;
            int v4_idx = ((i + 1) * resolucao + (j + 1)) * 4;


            //agora constroi os triangulos a partir dos vértices do quadrado
            //seria cortar o quadrado em dois triângulos
            // Cria o primeiro triângulo: v1, v3, v2

            // Copia o vértice 1 (x,y,z,w)
            triangles[triangle_index++] = p_esfera[v1_idx]; //x
            triangles[triangle_index++] = p_esfera[v1_idx + 1]; //y
            triangles[triangle_index++] = p_esfera[v1_idx + 2]; //z
            triangles[triangle_index++] = p_esfera[v1_idx + 3]; //w
            // Copia o vértice 3 (x,y,z,w)
            triangles[triangle_index++] = p_esfera[v3_idx];
            triangles[triangle_index++] = p_esfera[v3_idx + 1];
            triangles[triangle_index++] = p_esfera[v3_idx + 2];
            triangles[triangle_index++] = p_esfera[v3_idx + 3];
            // Copia o vértice 2 (x,y,z,w)
            triangles[triangle_index++] = p_esfera[v2_idx];
            triangles[triangle_index++] = p_esfera[v2_idx + 1];
            triangles[triangle_index++] = p_esfera[v2_idx + 2];
            triangles[triangle_index++] = p_esfera[v2_idx + 3];

            // Cria o segundo triângulo: v2, v3, v4
             // Copia o vértice 2 (x,y,z,w)
            triangles[triangle_index++] = p_esfera[v2_idx];
            triangles[triangle_index++] = p_esfera[v2_idx + 1];
            triangles[triangle_index++] = p_esfera[v2_idx + 2];
            triangles[triangle_index++] = p_esfera[v2_idx + 3];
            // Copia o vértice 3 (x,y,z,w)
            triangles[triangle_index++] = p_esfera[v3_idx];
            triangles[triangle_index++] = p_esfera[v3_idx + 1];
            triangles[triangle_index++] = p_esfera[v3_idx + 2];
            triangles[triangle_index++] = p_esfera[v3_idx + 3];
            // Copia o vértice 4 (x,y,z,w)
            triangles[triangle_index++] = p_esfera[v4_idx];
            triangles[triangle_index++] = p_esfera[v4_idx + 1];
            triangles[triangle_index++] = p_esfera[v4_idx + 2];
            triangles[triangle_index++] = p_esfera[v4_idx + 3];
        }
    }
    
    free(p_esfera);
    
    return triangles;
}

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
  int colorSize = 0;
  int normalSize = 0;
  int texelSize = 0;
  int numVertices = 0;
  *vertex = esfera_triangulos(&numVertices, 20); //esfera com 20 de latitude e longitude
  int vertexSize = numVertices * 4 * sizeof(GLfloat); // 4 floats per vertex (x,y,z,w)

	fprintf(stdout, "plane::start\n");
	fflush(stdout);

  if(colorFlag == GL_TRUE)
    colorSize = sizeof(colors);   // 1 face * 2 triangles * 3 points * 3 floats * 4 (sizeof(float))

  if(normalFlag == GL_TRUE)
    normalSize = sizeof(normals);   // 1 face * 2 triangles * 3 points * 3 floats * 4 (sizeof(float))

  if(texelFlag == GL_TRUE)
    texelSize = sizeof(texels);   // 1 face * 2 triangles * 3 points * 2 floats * 4 (sizeof(float))

  int numElements = vertexSize / sizeof(GLfloat);

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

