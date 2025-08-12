/*
 * testPlaneWireframe.c
 *
 * Draw a wireframe plane 
 *
 *  Created on: January 29 2025
 *      Author: JMario
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "myTools.h"
#include "esfera.h"


// Constant
// ---------------------------------------
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
const char *PROGNAME = "Atividade 8.4 – Esfera Gouraud";
const char *BASEDIR = "/home/vini-carvalho/Documentos/EA979/OpenGL (Cópia 2)/Atividade8/Atividade_8_4";
char *VERTSHADER = "./res/shaderWireframe.vert";
char *FRAGSHADER = "./res/shaderWireframe.frag";

// Function declaration
// ---------------------------------------
void display(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int generateShader(int type, char* filename);
void init(void);
void processInput(GLFWwindow *window);
GLFWwindow* setupEnvironment(void);

// Variable declaration
// ---------------------------------------
GLuint shaderProgram;   
GLuint VAO[1];
GLuint VBO[3]; //vertices, normais, cores
// GLint size;
GLint planeNumVertices;
GLint modelMatrixLoc, projMatrixLoc, modelViewMatrixLoc;
GLint lightPosLoc, viewPosLoc, lightColorLoc, objectColorLoc;

// Main: it starts here
// ---------------------------------------
int main(void) {
	fprintf(stdout, "main::start\n");
	fflush(stdout);

    // change base directory
    chdir(BASEDIR);
    fprintf(stdout, "main::start base directory %s\n", BASEDIR);
	fflush(stdout);

    // create window and load OpenGL functions
	 GLFWwindow* window = setupEnvironment();

    // set up data to display
    // ---------------------------------------
    init();

    // render data
    // ----------------------------------------
    display(window);

	fprintf(stdout, "main::end\n");
	fflush(stdout);

    exit(0);
}

// Render and display data
// ---------------------------------------
void display(GLFWwindow* window) {
	fprintf(stdout, "display::start\n");
	fflush(stdout);

    // Define the background color
    // ---------------------------------------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //matrizes utilizadas no shader
    //vi que é uma boa pratica definir com uniforms
    projMatrixLoc = glGetUniformLocation(shaderProgram, "u_projMatrix");
    modelViewMatrixLoc = glGetUniformLocation(shaderProgram, "u_modelViewMatrix");
    modelMatrixLoc = glGetUniformLocation(shaderProgram, "u_modelMatrix");
    lightPosLoc = glGetUniformLocation(shaderProgram, "u_lightPos");
    viewPosLoc = glGetUniformLocation(shaderProgram, "u_viewPos");
    lightColorLoc = glGetUniformLocation(shaderProgram, "u_lightColor");
    objectColorLoc = glGetUniformLocation(shaderProgram, "u_objectColor");

    // Set Projection Matrix
	// --------------------------------------
	GLfloat projMatrix[16];
    ortho(-1.5f, 1.5f, -1.5f, 1.5f, -1.5f, 5.0f, projMatrix);
    // fprintf(stdout, "projMatrix\n");
    // printMatrix4x4(projMatrix);

	glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, projMatrix);

	// Set View transformation
	// --------------------------------------
	GLfloat viewMatrix[16];
    matrixIdentity4x4(viewMatrix);

    // Set ModelView transformation
	// --------------------------------------
	GLfloat modelViewMatrix[16];
	matrixIdentity4x4(modelViewMatrix);

    // Model transformation variable
	// --------------------------------------
    GLfloat modelMatrix[16];
  
    // Set scale transformation 
	// --------------------------------------
    GLfloat mXScale[16];
    matrixScale4x4(0.5, 0.5, 1, mXScale);

    // Set rotation transformation 
	// --------------------------------------
    GLfloat mXRot[16];
    GLfloat axis[3] = {0, 1, 0}; //rotate in Y axis
    GLfloat angleRot = 0;
    GLfloat stepRot = 1;
 
    // render loop
    // ---------------------------------------
    while (!glfwWindowShouldClose(window)) {

   // 	fprintf(stdout, "render loop\n");
   // 	fflush(stdout);

        // Check input
        // ---------------------------------------
        processInput(window);

        // Clear screen
    	// --------------------------------------
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Posição da Câmera
        glUniform3f(viewPosLoc, 3.0f, 3.0f, 8.0f); 
        // Cor da luz (Branca)
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
        // Posição da Luz
        GLfloat lightPos[] = {3.0f, 3.0f, 8.0f};
        glUniform3fv(lightPosLoc, 1, lightPos);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Set Model transformation
    	// --------------------------------------
        matrixRotate4x4(angleRot, axis[0], axis[1], axis[2], mXRot);
        angleRot += stepRot;
        if (angleRot >= 360 || angleRot <= -360)
            angleRot = 0;

        matrixMultiply4x4(mXScale, mXRot, modelMatrix);

        // --- Envie as matrizes para o shader ---
        // Matriz de Modelo (Model)
        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix);

        // Matriz ModelView
        GLfloat modelViewMatrix[16];
		matrixMultiply4x4(viewMatrix, modelMatrix, modelViewMatrix);
        glUniformMatrix4fv(modelViewMatrixLoc, 1, GL_FALSE, modelViewMatrix);
        glDrawArrays(GL_TRIANGLES, 0, planeNumVertices);

        // Troca de buffers e eventos
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

	fprintf(stdout, "display::end\n");
	fflush(stdout);
    return;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "framebuffer_size_callback::start\n");
	fflush(stdout);

    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);

	fprintf(stdout, "framebuffer_size_callback::end\n");
	fflush(stdout);

	return;
}

// Set up OpenGL data
// ---------------------------------------
void init(void) {
	fprintf(stdout, "init::start\n");
	fflush(stdout);

	fprintf(stderr, "Renderer %s\n", glGetString(GL_RENDERER));
	fprintf(stderr, "Vendor %s\n", glGetString(GL_VENDOR));
	fprintf(stderr, "Version %s\n", glGetString(GL_VERSION));
	fprintf(stderr, "Shading Language Version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fflush(stderr);

    // Create vertex shader
	// ---------------------------------------
	GLuint vertexShader = generateShader(GL_VERTEX_SHADER, VERTSHADER);

    // Create fragment shader
	GLuint fragmentShader = generateShader(GL_FRAGMENT_SHADER, FRAGSHADER);

    // Create Program and link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    // ---------------------------------------
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "init::shader program linking failed::%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Use Program
    //-------------------------------
    glUseProgram(shaderProgram);

    // Vertex position and associated data
    // -----------------------------------
    GLfloat *vertices = NULL; //ponteiro para os vertices
    GLfloat *normals = NULL; //ponteiro para os normais
    GLfloat *texels = NULL; //ponteiro para as texturas
    GLfloat *colors = NULL; //ponteiro para as cores

    //Funcao cria os vertices, normais, texels e cores do plano
    planeNumVertices = createPlane(&vertices, GL_TRUE, &colors, GL_TRUE, &normals, GL_TRUE, &texels);

    //  fprintf(stdout, "v %f", vertices[0]);

	// Create Vertex Array Object ID
	// ---------------------------------------
    glGenVertexArrays(1, VAO); 

    // Create/activate a Vertex Array Object and bind to ID
    // ---------------------------------------
    glBindVertexArray(VAO[0]);

	// Generate Buffer IDs for 2 buffers
    // ---------------------------------------
    glGenBuffers(3, VBO); //gera os 3 buffers: vertices, normals, colors


    //VERTICES

    // Create/activate a Buffer and bind to ID VBO[0]
    // ---------------------------------------
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    // Initialize Buffer with vertex position data
    // ---------------------------------------
    // glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, planeNumVertices * 4 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // Associate vertex data to vertex shader input
    // ---------------------------------------
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    // Enable vertex data to be transfered to shader
    // ---------------------------------------
    glEnableVertexAttribArray(0);

    //NORMAIS

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, planeNumVertices * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW); // Normais são vec3
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(1); // Habilita o location = 1


    //CORES
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, planeNumVertices * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Cores são vec4
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(2); // Habilita o location = 2


    // uniform color
    //GLfloat colorPlane[] = {0.0f, 0.0f, 1.0f , 1.0f};
    //int u_colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	//glUniform4fv(u_colorLoc, 1, colorPlane);

	fprintf(stdout, "init::end\n");
	fflush(stdout);
    return;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
//	fprintf(stdout, "processInput::start\n");
//	fflush(stdout);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

//	fprintf(stdout, "processInput::end\n");
//	fflush(stdout);
}

// Create window and load OpenGL library
// ---------------------------------------
GLFWwindow* setupEnvironment(void) {
	fprintf(stdout, "prepare::start\n");
	fflush(stdout);

    // glfw: initialize and configure
    // ---------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // ---------------------------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, PROGNAME, NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "prepare::failed to create GLFW window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
    	fprintf(stderr, "prepare::failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }

    return window;
}


