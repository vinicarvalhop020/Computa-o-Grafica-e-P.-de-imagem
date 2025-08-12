#version 430 core

// saida
out vec4 FragColor;

// entradas vindas do vertex, após passar pela GPU
in vec3 v_Normal;
in vec3 v_FragPos;
in vec3 v_Color; // Cor base do objeto

// uniforms
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform vec3 u_lightColor;

void main(void) {

    vec3 Normal = normalize(v_Normal); //normaliza o vetor normal (a GPU fez o trabalho de quebrar o vetor normal dos vertices e espalhar pleos pixels)
    vec3 FragPos = v_FragPos;

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

    // Junta as 3 e combina com a cor do objeto
    vec3 result = (ambiente + difusa + especular) * v_Color;
    FragColor = vec4(result, 1.0);
}
