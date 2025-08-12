/*
 * gizmo.h
 *
 *  Created on: 07 de setembro de 2024
 *      Author: JMario
 */

#ifndef INCLUDE_MYTOOLS_H_
#define INCLUDE_MYTOOLS_H_

unsigned int generateShader(int type, char* filename );

void printMatrix4x4(float m[16]);

void matrixIdentity4x4(float m[16]);

void matrixMultiply4x4(float m2[16], float m1[16], float m[16]);

void matrixRotate4x4(float angle, float x, float y, float z, float m[16]);

void matrixScale4x4(float x, float y, float z, float m[16]);



#endif /* INCLUDE_MYTOOLS_H_ */