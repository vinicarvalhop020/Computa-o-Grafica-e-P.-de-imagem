
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
#include "esfera.h"
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
    float radius = 2.0f; // Raio da esfera
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
     
            // Cada quadrado é formado por 4 vértices, que são os pontos da esfera

            //pega os indices que irao compor o quadrado
            // v1, v2, v3, v4 são os vértices do quadrado
            int v1_idx = (i * resolucao + j) * 4;
            int v2_idx = (i * resolucao + (j + 1)) * 4;
            int v3_idx = ((i + 1) * resolucao + j) * 4;
            int v4_idx = ((i + 1) * resolucao + (j + 1)) * 4;


            //agora constroi os triangulos a partir dos vértices do quadrado
            //seria cortar o quadrado em dois triângulos
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


GLfloat* tonalizacao_flat(GLfloat *triangles, int numVertices, int resolucao) {
    // Recebe os poligonos triangulares e gera os normais de cada polígono
    //Na tonalização flat, cada triângulo tem uma única normal, que é a normal do plano definido pelos três vértices do triângulo.
    //A normal é calculada usando o produto vetorial entre dois vetores formados pelos vértices do triângulo.


    //o numero de triangulos

    int num_quads = (resolucao - 1) * (resolucao - 1);
    int numTriangles = num_quads * 2; // Cada quadrado tem 2 triângulos


    GLfloat *normals = malloc(numVertices * 3 * sizeof(GLfloat)); // cada triangulo tem uma normal, cada vertice desse triangulo recebera ESSA normal
    int normal_index = 0;


    //itera sobre cada triângulo
    for (int i = 0; i < numTriangles; i++) {
        //calcula a normal do triângulo
        // Cada triângulo é formado por 3 vértices, que são os pontos da esfera
        // Os vértices do triângulo são obtidos a partir do array de triângulos

        int v1_idx = i * 12; // Cada triângulo tem 3 vértices, cada vértice tem 4 componentes (x,y,z,w)
        int v2_idx = v1_idx + 4; // Próximo vértice
        int v3_idx = v2_idx + 4; // Próximo vértice

        GLfloat v1[3] = { 
            triangles[v1_idx], //v1_x
            triangles[v1_idx + 1], //v1_y
            triangles[v1_idx + 2]  //v1_z
        };

        GLfloat v2[3] = { 
            triangles[v2_idx], //v2_x
            triangles[v2_idx + 1], //v2_y
            triangles[v2_idx + 2]  //v2_z
        };

        GLfloat v3[3] = { 
            triangles[v3_idx], //v3_x
            triangles[v3_idx + 1], //v3_y
            triangles[v3_idx + 2]  //v3_z
        };

        // Calcula os vetores do triângulo
        // Vetor A: v2 - v1
        GLfloat A[3] = {
            v2[0] - v1[0],
            v2[1] - v1[1],
            v2[2] - v1[2]
        };
        // Vetor B: v3 - v1
        GLfloat B[3] = {
            v3[0] - v1[0],
            v3[1] - v1[1],
            v3[2] - v1[2]
        };

        // Calcula o produto vetorial A x B para obter a normal
        GLfloat normal[3];
        normal[0] = A[1] * B[2] - A[2] * B[1]; // x
        normal[1] = A[2] * B[0] - A[0] * B[2]; // y
        normal[2] = A[0] * B[1] - A[1] * B[0]; // z

        //Faz a normal unitária
        GLfloat length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]); //tamanho do vetor
        if (length > 0.0f) { // se o tamanho for maior que 0, normaliza
            normal[0] /= length;
            normal[1] /= length;
            normal[2] /= length;
        }

        // Armazena a normal no array de normais, todo vertice do triângulo recebe a mesma normal

        //v1
        //faz -normal[0] para que a normal fique apontando para fora da esfera
        normals[normal_index++] = -normal[0]; // x
        normals[normal_index++] = -normal[1]; // y
        normals[normal_index++] = -normal[2]; // z
        //v2
        normals[normal_index++] = -normal[0]; // x
        normals[normal_index++] = -normal[1]; // y
        normals[normal_index++] = -normal[2]; // z
        //v3
        normals[normal_index++] = -normal[0]; // x
        normals[normal_index++] = -normal[1]; // y
        normals[normal_index++] = -normal[2]; // z
    }
    // Retorna o array de normais
    return normals;
}

GLfloat* tonalizacao_gouraud(GLfloat *vertices, int numVertices){
    // Na tonalização Gouraud, cada vertice recebe uma normal
    // Para a esfera, a normal de cada vertice é o vetor que sai do centro até o vértice normalizado
    // seria semelhante ao sol, com seus raios saindo do centro ... 

    GLfloat *normals = malloc(numVertices * 3 * sizeof(GLfloat)); // Cada vértice tem uma normal (x,y,z)
    int normal_index = 0;

    GLfloat centro[3] = {0.0f, 0.0f, 0.0f}; // foi definido como o centro da esfera a origem

    // itera sobre cada vértice
    for (int i = 0; i < numVertices; i++) {
        //cria o vetor que vai do centro até o vértice
        GLfloat v[3] = { //segue o padrao x, y , z e w (w nao serve pra nada aqui)
            vertices[i * 4], // x
            vertices[i * 4 + 1], // y
            vertices[i * 4 + 2]  // z
        };

        // Calcula o vetor do centro até o vértice
        GLfloat vetor[3] = {
            v[0] - centro[0],
            v[1] - centro[1],
            v[2] - centro[2]
        };

        // normaliza o vetor
        GLfloat length = sqrt(vetor[0] * vetor[0] + vetor[1] * vetor[1] + vetor[2] * vetor[2]); //tamanho do vetor
        if (length > 0.0f) { // se o tamanho for maior que
            vetor[0] /= length;
            vetor[1] /= length;
            vetor[2] /= length;
        }

        // Armazena o vetor no array
        normals[normal_index++] = vetor[0]; // x
        normals[normal_index++] = vetor[1]; // y
        normals[normal_index++] = vetor[2]; // z
    }
    return normals;    
}

GLfloat* pontos_textura(int numVertices, int resolucao) {

    GLfloat *texels = malloc(numVertices * 2 * sizeof(GLfloat));
    int texel_index = 0;

    // Os loops é o mesmo IDÊNTICOS aos da esfera_triangulos para percorrer cada "quadrado" da esfera.
    for (int i = 0; i < resolucao - 1; i++) { // latitude
        for (int j = 0; j < resolucao - 1; j++) { // longitude

            // Calcula as coordenadas UV para os 4 cantos do quadrado atual.

            
            // Calcula as coordenadas UV para os 4 cantos do quadrado atual.
            
            //eu fiz varios testes com texturas e percebi que as vezes a imagem ficava invertida, por isso o 1.0f - (float)j / (resolucao - 1)
            //notei que quando o loop j começa (em j=0), theta é 0. O cosf(0) é 1, o que coloca o vértice no ponto mais à direita da esfera.
            //a textura vai de 1 a 0 enquanto j vai de 0 a resolucao-1

            float u1 = 1.0f-((float)j / (resolucao - 1));
            float v1 = (float)i/(resolucao - 1);
            float u2 = 1.0f-((float)(j + 1) / (resolucao - 1));
            float v2 = (float)i/(resolucao - 1);
            float u3 = 1.0f-((float)j / (resolucao - 1));
            float v3 = (float)(i + 1) / (resolucao - 1);
            float u4 = 1.0f-((float)(j + 1) / (resolucao - 1));
            float v4 = (float)(i + 1) / (resolucao - 1);


            /*
              (u1,v1) ----- (u2,v2)
                 |           / |
                 |         /   |
                 |       /     |
              (u3,v3) ----- (u4,v4)
            */

            // primeiro triangulo: v1, v3, v2
            // A ordem de adição DEVE ser a mesma que esfera_triangulos para os vértices: v1, v3, v2.
            
            // vértice 1
            texels[texel_index++] = u1;
            texels[texel_index++] = v1;
            
            // vértice 3
            texels[texel_index++] = u3;
            texels[texel_index++] = v3;
            
            // vértice 2
            texels[texel_index++] = u2;
            texels[texel_index++] = v2;

            // segundo triangulo: v2, v3, v4

            // vértice 2
            texels[texel_index++] = u2;
            texels[texel_index++] = v2;
            
            // vértice 3
            texels[texel_index++] = u3;
            texels[texel_index++] = v3;

            // vértice 4
            texels[texel_index++] = u4;
            texels[texel_index++] = v4;
        }
    }
    return texels;
}

GLint createPlane(GLfloat **vertex, GLboolean colorFlag, GLfloat **color,  GLboolean normalFlag, GLfloat **normal, GLboolean texelFlag, GLfloat **texel) {
  int numVertices = 0;
  int resolucao = 12; 

  *vertex = esfera_triangulos(&numVertices, resolucao); //esfera com 20 de latitude e longitude

  //gouraud
  *normal = tonalizacao_gouraud(*vertex, numVertices); //normal gouraud
  //flat
  //*normal = tonalizacao_flat(*vertex, numVertices, resolucao); //normal flat  
  *texel = pontos_textura(numVertices, resolucao); //texel

    if(colorFlag == GL_TRUE) {
            *color = (GLfloat *) malloc(numVertices * 3 * sizeof(GLfloat));
            if (*color) {
                for(int i = 0; i < numVertices; i++) {
                    (*color)[i*3 + 0] = 0.8f; // R
                    (*color)[i*3 + 1] = 0.0f; // G 
                    (*color)[i*3 + 2] = 0.0f; // B
                }
            }
        }
    
	fprintf(stdout, "plane::end\n");
	fflush(stdout);
  return numVertices ;
}



