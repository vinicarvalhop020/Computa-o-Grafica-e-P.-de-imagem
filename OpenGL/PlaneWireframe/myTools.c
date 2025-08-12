/*
 * mytools.c
 *
 *  Created on: 06 de setembro de 2024
 *      Author: JMario
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// generateShader: reads shader code from ASCII file and compiles code
// ---------------------------------------------------------------------------------------------
unsigned int generateShader(int type, char* filename ) {
	int shader;

	fprintf(stdout, "generateShader::start\n");
	fflush(stdout);

	// input shader file
	FILE *file;
	if ((file = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "ERROR: generateShader::fopen::%s\n", filename);
		exit(1);
	}

	// get size
	fseek(file, 0L, SEEK_END);
	int fileSize = ftell(file);
	rewind(file);

	// read file. One aditional place to close the string (null)
	char *shaderText = malloc((fileSize + 1) * sizeof(char));
	char *shaderSource[] = {(char *) shaderText};

	if(fread(shaderText, sizeof(char), fileSize, file) != fileSize) {
		fprintf(stderr, "ERROR generateShader::fread::%s\n", filename);
		exit(1);
	}
	
	shaderText[fileSize] = '\0';
	
	// close file
	if(fclose(file)) {
		fprintf(stderr, "ERROR generateShader::fclose::%s\n", filename);
		exit(1);
	}

	// Create GPU shader handle
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char * const*) shaderSource, NULL);

	glCompileShader(shader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "ERROR generateShader::shader compilation failed:: %s\n", infoLog);
		exit(1);
	}

	printf("generateShader::end\n");
	fflush(stdout);
	return shader;
}

// Função em Printar a Matriz
void printMatrix4x4(float m[16]) {

    for (int i = 0; i < 4; i++){ 
        for (int j = 0; j < 4; j++) {
            printf("%f\t", m[i+j*4]);
        }
        printf("\n");
    }
    printf("\n");

}




// Função da Matriz de Identidade
void matrixIdentity4x4(float m[16]) {

    m[0] = 1; m[4] = 0; m[8] =  0; m[12] = 0; // | 1  0   0   0 |
    m[1] = 0; m[5] = 1; m[9] =  0; m[13] = 0; // | 0  1   0   0 |
    m[2] = 0; m[6] = 0; m[10] = 1; m[14] = 0; // | 0  0   1   0 |
    m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1; // | 0  0   0   1 | 
    
//    printMatrix4x4(m);

}

// Função de Multiplicação de Matrizes 4x4x
void matrixMultiply4x4(float m2[16], float m1[16], float m[16]) {

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i*4+j] = 0;
            for (int k = 0; k < 4; k++) {
                m[i*4+j] +=  m2[j+k*4]*m1[i*4+k];
 //               printf("m[%i] = m2[%i] + m1[%i] ", i*4+j, j+k*4, i*4+k );
            }
 //           printf("\n");
        }
    } 
//   printMatrix4x4(m);
    
}



// Função de Rotação de Matriz
void matrixRotate4x4(float angle, float x, float y, float z, float m[16]){
    float angleRad = (angle * M_PI) / 180;

    float c = (float) cos(angleRad);
    float s = (float) sin(angleRad);
    float norm = pow((x*x+y*y+z*z),1/2);
    float xn = x/norm; // xn = x/raiz(x^2+y^2+z^2)
    float yn = y/norm; // yn = y/raiz(x^2+y^2+z^2)
    float zn = z/norm; // zn = z/raiz(x^2+y^2+z^2)

    // Calcula matriz de rotação
    // | xn^2*(1-c)+c      xn*yn*(1-c)-zn*s   xn*zn*(1-c)+yn*s    0 |
    // | xn*yn*(1-c)+zn*s  yn^2*(1-c)+c       yn*zn*(1-c)-xn*s    0 |
    // | xn*zn*(1-c)-yn*s  yn*zn*(1-c)+xn*s   zn^2*(1-c)+c        0 |
    // | 0                0                  0                   1 |
    m[0] = (xn*xn)*(1-c)+c;      m[4] = (xn*yn)*(1-c)-(zn*s); m[8] = (xn*zn)*(1-c)+(yn*s);  m[12] = 0; 
    m[1] = (xn*yn)*(1-c)+(zn*s); m[5] = (yn*yn)*(1-c)+c;      m[9] =  (yn*zn)*(1-c)-(xn*s); m[13] = 0; 
    m[2] = (xn*zn)*(1-c)-(yn*s); m[6] = (yn*zn)*(1-c)+(xn*s); m[10] = (zn*zn)*(1-c)+c;      m[14] = 0; 
    m[3] = 0;                    m[7] = 0;                    m[11] = 0;                    m[15] = 1; 

 //   printMatrix4x4(m);
}

// Função de Escalar de Matriz 4x4
void matrixScale4x4(float x, float y, float z, float m[16]){
    
    m[0] = x; m[4] = 0; m[8] =  0; m[12] = 0; // | x  0  0   0 |
    m[1] = 0; m[5] = y; m[9] =  0; m[13] = 0; // | 0  y  0   0 |
    m[2] = 0; m[6] = 0; m[10] = z; m[14] = 0; // | 0  0  z   0 |
    m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1; // | 0  0  0   1 | 

//    printMatrix4x4(m);
}
