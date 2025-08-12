#version 430 core

// entrada
layout(location = 0) in vec4 vertex;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor; 
layout(location = 3) in vec2 aTexCoord; // ENTRADA para a coordenada de textura


// uniform das matrizes
layout(location = 2) uniform mat4 u_projMatrix; 
layout(location = 3) uniform mat4 u_modelViewMatrix;
layout(location = 4) uniform mat4 u_modelMatrix;

layout(location = 5) uniform vec3 u_lightPos;
layout(location = 6) uniform vec3 u_viewPos;
layout(location = 7) uniform vec3 u_lightColor;

// saida 
out vec4 vColor; // Cor calculada que será enviada para o Fragment Shader
out vec2 vTexCoord; // textura


void main(void) {

	vTexCoord = aTexCoord; //textura
    vec3 FragPos = vec3(u_modelMatrix * vertex); 
    vec3 Normal = normalize(mat3(transpose(inverse(u_modelMatrix))) * aNormal); 

	//Phong
	// vetor L
	vec3 lightDir = normalize(u_lightPos - FragPos);

	// vetor V
	// Componente Ambiente
	vec3 refletividade_ambiente = vec3(0.1 ,0.1 , 0.1); //ka para r,g,b
	vec3 ambiente = refletividade_ambiente * u_lightColor;  //ka * Ia 

	// Componente Difusa
	vec3 refletividade_difusa = vec3(0.7 ,0.7 , 0.7); //kd para r,g,b
	float fatorDifuso = max(dot(Normal, lightDir), 0.0); //vetor_Normal dot Vetor_reflexao_luz
	vec3 difusa = refletividade_difusa  * u_lightColor * fatorDifuso; //kd * Ii * (N dot L)
	
	// Componente Especular
	vec3 refletividade_especular = vec3(0.8 , 0.8, 0.8);//ks para r,g,b
	vec3 Vetor_observador = normalize(u_viewPos - FragPos); // Vetor V
	vec3 Vetor_reflexao_luz = reflect(-lightDir, Normal);   // Vetor R 

	int n = 32;
	float fatorEspecular = pow(max(dot(Vetor_reflexao_luz, Vetor_observador), 0.0), n); //(R dot V)^n
	vec3 especular = refletividade_especular * fatorEspecular * u_lightColor;  //ks * Is * (R dot V)^n

	vec3 result = (ambiente + difusa + especular);
	vColor = vec4(result, 1.0);

	gl_Position = u_projMatrix * u_modelViewMatrix * vertex;
}