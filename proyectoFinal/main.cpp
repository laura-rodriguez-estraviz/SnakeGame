
//LIBRERIAS
//________________________________________________________________________________________

#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <lecturaShader_0_9.h>
#include "esfera.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <time.h>



//VARIABLES GLOBALES
//________________________________________________________________________________________

//Tamaño de la ventana
 float SCR_WIDTH = 800;
 float SCR_HEIGHT = 800;

const float ARADIANES = 3.14 / 180;//constante para pasar de grados a radianes

int shaderProgram; //shader

unsigned int VAO_cuadrado, VAO_esfera, VAO_cubo; //VAOs de los diferentes tipos de objeto

//variables para el mobimiento de la grua
float direccion;

int camara;//variable donde se guardará la camara que esta activada
int inicio = 1;
int fin = 0;
float ind[]= { -4.5f,-4.0f,-3.5f,-3.0f,-2.5f,-2.0f,-1.5f,-1.0f,-0.5f,0.0f,0.5f,1.0f,1.5f,2.0f,2.5f,3.0f,3.5f,4.0f,4.5f };

typedef struct {//estructura para alvergar objetos a dibujar
	float px, py, pz;
	float angulo_trans;
	float angulo_trans_2;
	float sx, sy, sz;
	unsigned int vao;
	int numvertex;
	float angulo_trans_3;
}objeto;

typedef struct piezaSerpiente {
	objeto actual;
	struct piezaSerpiente* next;
	struct piezaSerpiente* previous;
} piezaSerpiente;

typedef struct {
	piezaSerpiente *cabeza;
	piezaSerpiente *cola;
}serpiente;

serpiente Serpiente;


typedef struct {//estructura para alvergar puntos en el espacio xyz
	float px, py, pz;
}punto;

int crece = 0;
objeto suelo = { 0,0,-1.25,0,0,9.5,9.5,2.5,0,0,0 };
objeto extraSuelo = { 0,0,-3.25,0,0,12.5,12.5,2.0,0,0,0 };
objeto fondo = {0,0,0,60,90,20,20,20,0,0,30};
objeto mandarina = { 0,0,0.5f,90,0,0.2,0.2,0.2,0,0,0 };
objeto gameOver = { 0,0,0,0,0,4,4,0.5,0,0,0 };
objeto Inicio = { 0,0,0,0,0,4.5,4.5,0.5,0,0,0 };
GLint texturaFondo;
GLint texturaSuelo;
GLuint texturaCabeza;
GLuint texturaCuerpo;
GLuint texturaMandarina;
GLuint texturaSueloExtra;
GLuint texturaInicio;
GLuint texturaFin;

//puntos para la configuracion de la iluminacion
punto pview = {0.0,0.0,0.0};
punto pluz = { 0.0,0.0,10.0 };

//FUNCIONES DE CONFIGURACION
//________________________________________________________________________________________

//gestion de entrada
void processInput(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


//funcion que configura algunos parámetros
void openGlInit() {
	glClearDepth(30); // Establece el valor del z-buffer al máximo para la profundidad máxima
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Establece el color de limpieza del buffer de color a negro (RGB: 0, 0, 0) y la transparencia a 1.0 (completamente opaco)
	glEnable(GL_DEPTH_TEST); // Habilita el z-buffer para realizar pruebas de profundidad y ocultar objetos más lejanos detrás de objetos más cercanos
	glEnable(GL_CULL_FACE); // Habilita el culling de caras para ocultar las caras posteriores de los objetos, lo que mejora la eficiencia del renderizado
	glCullFace(GL_BACK); // Especifica que se deben ocultar las caras posteriores (las caras orientadas hacia atrás) cuando se realiza el culling de caras
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Actualiza el viewport para que coincida con el nuevo tamaño de la ventana
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}


//Función que gestiona las teclas pulsadas
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	//std::cout << key << std::endl;//imprime por pantalla las teclas pulsadas

	//coche
	if (key == GLFW_KEY_UP) {	//flecha arriba
		if (direccion != 1) {
			direccion = 0;
		}
	}
	if (key == GLFW_KEY_DOWN) {	//flecha abajo
		if (direccion != 0) {
			direccion = 1;
		}

	}
	if (key == GLFW_KEY_LEFT) {		//flecha izquierda
		if (direccion != 3) {
			direccion = 2;
		}
	}
	if (key == GLFW_KEY_RIGHT) {	//flecha derecha
		if (direccion != 2) {
			direccion = 3;
		}
	}


	//camaras
	if (key == 49) {		//1
		camara = 0;
	}
	if (key == 50) {		//2
		camara = 1;
	}
	if (key == 51) {		//3
		camara = 2;
	}
}



//INICIALIZACION DE LOS VAOs Y TEXTURAS
//----------------------------------------------------------------
//Inicializo el VAO del cubo
void dibujaCubo() {
	unsigned int VBO;

	float vertices[] = {

		// Positions				// Normals					// Texture coordinates
		// Front face
		-0.5f, -0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			1.0f, 1.0f, // Top-right
		 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			1.0f, 1.0f, // Top-right
		-0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			0.0f, 1.0f, // Top-left
		-0.5f, -0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			0.0f, 0.0f, // Bottom-left

		// Right face
		 0.5f, -0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,			0.0f, 0.0f, // Bottom-right
		 0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,			1.0f, 0.0f, // Top-right
		 0.5f,  0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,			1.0f, 1.0f, // Top-left
		 0.5f,  0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,			1.0f, 1.0f, // Top-left
		 0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,			0.0f, 1.0f, // Bottom-left
		 0.5f, -0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,			0.0f, 0.0f, // Bottom-right

		 // Back face
		0.5f, -0.5f, -0.5f,			0.0f,  0.0f, -1.0f,			0.0f, 0.0f, // Bottom-right
		-0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			1.0f, 0.0f, // Bottom-left
		-0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			1.0f, 1.0f, // Top-left
		-0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			1.0f, 1.0f, // Top-left
		0.5f,  0.5f, -0.5f,			0.0f,  0.0f, -1.0f,			0.0f, 1.0f, // Top-right
		0.5f, -0.5f, -0.5f,			0.0f,  0.0f, -1.0f,			0.0f, 0.0f, // Bottom-right

		   // Left face
		-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,			0.0f, 0.0f, // Bottom-left
		-0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,			1.0f, 0.0f, // Bottom-right
		-0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f,			1.0f, 1.0f, // Top-right
		-0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f,			1.0f, 1.0f, // Top-right
		-0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f,			0.0f, 1.0f, // Top-left
		-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,			0.0f, 0.0f, // Bottom-left

		// Bottom face
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,			0.0f, 0.0f, // Bottom-left
		0.5f, -0.5f, -0.5f,			0.0f, 1.0f, 0.0f,			1.0f, 0.0f, // Bottom-right
		0.5f, -0.5f,  0.5f,			0.0f, 1.0f, 0.0f,			1.0f, 1.0f, // Top-right
		0.5f, -0.5f,  0.5f,			0.0f, 1.0f, 0.0f,			1.0f, 1.0f, // Top-right
		-0.5f, -0.5f,  0.5f,		0.0f, 1.0f, 0.0f,			0.0f, 1.0f, // Top-left
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,			0.0f, 0.0f, // Bottom-left

		// Top face
		-0.5f,  0.5f,  0.5f,		0.0f,  -1.0f, 0.0f,			0.0f, 1.0f, // Top-left
		0.5f,  0.5f,  0.5f,			0.0f,  -1.0f, 0.0f,			1.0f, 1.0f, // Top-right
		0.5f,  0.5f, -0.5f,			0.0f,  -1.0f, 0.0f,			1.0f, 0.0f, // Bottom-right
		0.5f,  0.5f, -0.5f,			0.0f,  -1.0f, 0.0f,			1.0f, 0.0f, // Bottom-right
		-0.5f,  0.5f, -0.5f,		0.0f,  -1.0f, 0.0f,			0.0f, 0.0f, // Bottom-left
		-0.5f,  0.5f,  0.5f,		0.0f,  -1.0f, 0.0f,			0.0f, 1.0f  // Top-left
	};

	// Genera el VAO y el VBO para el cubo
	glGenVertexArrays(1, &VAO_cubo);
	glGenBuffers(1, &VBO);

	// Vincula el VAO
	glBindVertexArray(VAO_cubo);

	// Vincula el buffer de vértices y llena el buffer con los datos del cubo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Especifica el atributo de posición de los vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Especifica el atributo de normal de los vértices
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Especifica el atributo de coordenadas de textura de los vértices
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Desvincula el buffer y el VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glDeleteBuffers(1, &VBO);//con la gráfica de mi ordenador no es necesario
}




//Inicializo el VAO de la esfera
void dibujaEsfera() {
	unsigned int VBO;

	// Genera el VAO y el VBO para la esfera
	glGenVertexArrays(1, &VAO_esfera);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO_esfera);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Rellena el buffer de vértices con los datos de la esfera
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_esfera), vertices_esfera, GL_STATIC_DRAW);

	// Especifica el atributo de posición de los vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(0);

	// Especifica el atributo de coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Especifica el atributo de normales
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glDeleteBuffers(1, &VBO);//con la gráfica de mi ordenador no es necesario
}

//funcion que carga las texturas
unsigned int CargarTextura(const char* archivo) {
	unsigned int texture; // Identificador de textura

	// Invierte verticalmente los datos de la imagen
	stbi_set_flip_vertically_on_load(true);
	// Genera un identificador de textura
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	// Vincula la textura recién generada
	glBindTexture(GL_TEXTURE_2D, texture);
	// Establece el modo de repetición de la textura en la dirección S (horizontal) y T (vertical)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Establece el método de interpolación para la reducción de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Establece el método de interpolación para el agrandamiento de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	// Carga la imagen de la textura
	unsigned char* data = stbi_load(archivo, &width, &height, &nrChannels, 0);

	if (data && (nrChannels == 3 || nrChannels == 4)) {
		if (nrChannels == 3) {//si la imagen no tiene transparencias
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {//si la imagen tiene transparencias
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		// Genera los mipmaps de la textura para la interpolación de niveles de detalle
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		// Si falla la carga de la textura imprime un mensaje de error
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Obtiene la ubicación del sampler2D en el shader y asigna la unidad de textura 0 a este sampler
	GLuint tex0Uni = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1i(tex0Uni, 0);
	return texture; // Devuelve el identificador de la textura generada
}



//FUNCIONES DE CONFIGURACION DE LA CAMARA
//-------------------------------------------------------------------------------------------------

//funcion que configura la camara a la por defento desde arriba
void myCamara() {
	// Establece el viewport para toda la ventana
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Configura la matriz de vista para la cámara fija
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(glm::vec3(0.0, 0.0, 7.5), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	// Obtiene la ubicación de la matriz de vista en el shader y la actualiza
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// Configura la matriz de proyección para la perspectiva
	glm::mat4 projection;
	projection = glm::mat4();
	if (SCR_WIDTH> SCR_HEIGHT) {
		projection = glm::ortho(SCR_WIDTH / SCR_HEIGHT*-5.0f, SCR_WIDTH / SCR_HEIGHT*5.0f, -5.0f, 5.0f, 0.1f, 50.0f);
	}
	else
		projection = glm::ortho(-5.0f, 5.0f,  SCR_HEIGHT/ SCR_WIDTH*-5.0f, SCR_HEIGHT / SCR_WIDTH*5.0f, 0.1f, 50.0f);


	// Obtiene la ubicación de la matriz de proyección en el shader y la actualiza
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}


//funcion que configura la camara a tercera persona
void myTerceraPers() {
	// Establece el viewport para toda la ventana
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Configura la matriz de vista para la cámara fija
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(glm::vec3(-7, -7, 7), glm::vec3(0.0, 0.0, 0.0), glm::vec3(.0, .0, 1.0));

	// Obtiene la ubicación de la matriz de vista en el shader y la actualiza
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// Configura la matriz de proyección para la perspectiva
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(86.6f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f); // FOV, aspect ratio, near, far

	// Obtiene la ubicación de la matriz de proyección en el shader y la actualiza
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}





//FUNCIONES DE DIBUJO
//-------------------------------------------------------------------------------------------------


void crearSuelo( unsigned int transformLoc) {
	glm::mat4 transform = glm::mat4();
	// Bucle para generar el suelo
	float i, j;
	for (i = -50; i <= 50; i += 5) {
		for (j = -50; j <= 50; j += 5) {
			// Vincula la textura del suelo
			glBindTexture(GL_TEXTURE_2D, texturaSueloExtra);

			// Calcula la matriz de transformación para el suelo
			transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, j, -3.25f));
			transform = glm::scale(transform, glm::vec3(5, 5, 5));

			// Actualiza la matriz de transformación en el shader
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

			// Vincula el VAO del cuadrado y realiza el dibujado
			glBindVertexArray(VAO_cuadrado);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}


//funcion que dibuja un objeto
void dibujarObjeto( unsigned int transformLoc, objeto b) {
	glm::mat4 transform =glm::mat4();

	// Translada el objeto a su posición en el espacio
	transform = glm::translate(transform, glm::vec3(b.px, b.py, b.pz));

	// Aplica rotaciones al objeto
	transform = glm::rotate(transform, (float)(b.angulo_trans * ARADIANES), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, (float)(b.angulo_trans_2 * ARADIANES), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, (float)(b.angulo_trans_3 * ARADIANES), glm::vec3(0.0f, 0.0f, 1.0f));

	// Escala el objeto
	transform = glm::scale(transform, glm::vec3(b.sx, b.sy, b.sz));

	// Actualiza la matriz de transformación en el shader
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Vincula el VAO del objeto y realiza el dibujado
	glBindVertexArray(b.vao);
	glDrawArrays(GL_TRIANGLES, 0, b.numvertex);
}


//Funcion que configura la iluminacion
void iluminacion() {
	// Configura la posición de la luz en el shader
	unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 0.0, 7.5, 0);

	// Configura el color del objeto en el shader (no lo uso ya que todos los objetos tienen textura)
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(colorLoc, 1.0, 1.0, 1.0);
	
}

//función que dibuja la serpiente
void dibujarSerpiente(unsigned int transformLoc) {
	glBindTexture(GL_TEXTURE_2D, texturaCuerpo);
	piezaSerpiente *aux=Serpiente.cabeza;
	dibujarObjeto( transformLoc, aux->actual);
	while (aux->next!=NULL) {
		aux = aux->next;
		dibujarObjeto( transformLoc,aux->actual);
	}
}


//funcion que dibuja los objetos y luces
void display() {
	
	glm::mat4 transform; // Matriz de transformación
	glm::mat4 transformtemp; // Matriz temporal de transformación

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform"); // Obtiene la ubicación de la matriz de transformación en el shader

	// dibuja el fondo
	transformtemp = glm::mat4(); // Matriz identidad
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D,texturaFondo);
	dibujarObjeto(transformLoc, fondo);
	glEnable(GL_CULL_FACE);
	
	//dibuja el suelo
	glBindTexture(GL_TEXTURE_2D, texturaSuelo);
	transformtemp = glm::mat4(); // Matriz identidad
	dibujarObjeto(transformLoc, suelo);

	if(inicio==0 && fin==0){
		// dibuja la serpiente
		dibujarSerpiente(transformLoc);

		//dibuja la mandarina
		glBindTexture(GL_TEXTURE_2D, texturaMandarina);
		dibujarObjeto(transformLoc, mandarina);
	}

	// Realiza la iluminación
	iluminacion();

	glBindVertexArray(0); // Desvincula el array de vértices
	
}

//función que sitúa la mandarina de manera aleatoria en el suelo
void situarMandarina() {
	mandarina.px=ind[rand() % 18];//Calculamos de manera aleatoria la posición en el eje x de la mandarina
	mandarina.py=ind[rand() % 18];//Calculamos de manera aleatoria la posición en el eje y de la mandarina
	piezaSerpiente* aux = Serpiente.cabeza;//Situamos en una variable auxiliar la nueva cabeza de la serpiente
	//Comprobamos que la mandarina no ha sido generada en un lugar ocupado por la serpiente
	while (aux->next != NULL) {
		aux = aux->next;
		if (aux->actual.px == mandarina.px && aux->actual.py == mandarina.py) {
			situarMandarina();
			return;
		}
	}
}

//función que comprueba si la serpiente tiene que crecer o no, si la mandarina se tiene que situar en una nueva posición o no y si el juego tiene que terminar
void comprobaciones() {
	piezaSerpiente* aux = Serpiente.cabeza;
	while (aux->next != NULL) {//se recorren las piezas de la serpiente
		aux = aux->next;
		if (aux->actual.px == Serpiente.cabeza->actual.px && aux->actual.py == Serpiente.cabeza->actual.py) {//si la posicion de la cabeza coincide con alguna de la de las otras piezas se marca que el juego debe acabar
			fin = 1;
		}
		if (aux->actual.px == mandarina.px && aux->actual.py == mandarina.py) {//si la posicion de la cabeza coincide con la de la mandarina se marca que esta debe crcer y se vuelve a situar la mandarina en un lugar aleatorio
			situarMandarina();
			crece = 1;
		}
	}
}

//función que va creando las diferentes partes de la serpiente a medida que esta se va mobiendo
void crearParteSerpiente(float x, float y) {
	piezaSerpiente* aux = (piezaSerpiente*)malloc(sizeof(piezaSerpiente));//reservo memoria para una pieza de la serpiente
	//valoro si la serpiente está tocando alguno de los bordes para que esta pase al lado contrario
	if (abs(x) < 5.0 && abs(y) < 5.0) {//si esta dentro de los bordes 
		aux->actual = { x, y, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, VAO_cubo, 36, 0.0f };//inicializo los valores de la pieza utilizando la posicion pasada a la función
	}
	else if (x==5.0) {//si pisa el lado superior
		aux->actual = { -4.5, y, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, VAO_cubo, 36, 0.0f };//la siguiente pieza se colocara en el lado inferior
	}
	else if(y==5.0) {//si pisa el lado derecho
		aux->actual = { x, -4.5, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, VAO_cubo, 36, 0.0f };//la siguiente pieza se colocara en el lado izquierdo
	}
	else if (x == -5.0) {//si pisa el lado inferior
		aux->actual = { 4.5,y, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, VAO_cubo, 36, 0.0f };//la siguiente pieza se colocará en el lado superior
	}
	else if (y == -5.0) {//si pisa el lado izquierdo
		aux->actual = { x, 4.5, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, VAO_cubo, 36, 0.0f };//la siguiente pieza se colocara en el lado derecho
	}

	//Se coloca la nueva pieza como la nueva cabeza
	aux->next = Serpiente.cabeza;
	aux->previous = NULL;
	Serpiente.cabeza->previous = aux;
	Serpiente.cabeza = aux;
	//Se elimina la cola, y la nueva cola es la pieza anterior
	aux = Serpiente.cola;
	Serpiente.cola = Serpiente.cola->previous;
	Serpiente.cola->next = NULL;

	comprobaciones();//se comprueba si la serpiente come una mandarina o no, o si se come a si misma y debe acabar el juego o no

	if (crece == 0) {//si la serpiente no ha comido una mandarina se livera la cola vieja
		free(aux);
	}
	else{//si la serpiente  ha comido una mandarina la cola se vuelve a añadir a la serpiente
		Serpiente.cola->next = aux;
		Serpiente.cola = aux;
		crece = 0;
	}	
}

//MAIN
//-----------------------------------------------------------------------------------------------------------------------
//main del proyecto
int main()
{
	//Genero ventana
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creo la ventana
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Grua", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Leo el gald
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Fallo inicializacion Glad" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, keyCallback);// Establece la función de devolución de llamada para manejar eventos de teclado

	openGlInit();//Configuracion de z-buffer, color de limpieza de buffer etc.

	shaderProgram = setShaders("shader.vert", "shader.frag"); // Carga y compila los shaders de vértices y fragmentos, y los vincula en un programa de shader

	glUseProgram(shaderProgram);// Usa el programa de shader especificado

	//inicializo los VAOs
	dibujaEsfera();
	dibujaCubo();

	//inicializo los vaos de los objetos y los vertices
	fondo.vao = VAO_esfera;
	fondo.numvertex = 1080;
	mandarina.vao = VAO_esfera;
	mandarina.numvertex = 1080;
	Inicio.vao = extraSuelo.vao = gameOver.vao = suelo.vao = VAO_cubo;
	gameOver.numvertex = Inicio.numvertex = extraSuelo.numvertex = suelo.numvertex = 36;

	//inicializo las texturas
	texturaMandarina = CargarTextura("mandarina.png");
	texturaFondo = CargarTextura("fondoCastillo.jpeg");
	texturaSuelo = CargarTextura("suelo.jpeg");
	texturaCuerpo = CargarTextura("cuerpoSerpiente.png");
	texturaSueloExtra = CargarTextura("suelo2.jpg");
	texturaInicio = CargarTextura("logo.jpg");
	texturaFin = CargarTextura("gameOver.jpg");
	

	direccion = 0;//inicializo la direccion para que la serpiente empiece mobiendose hacia arriba

	// Inicialización de la cabeza de la serpiente
	Serpiente.cabeza = (piezaSerpiente*)malloc(sizeof(piezaSerpiente)); // Asegúrate de haber incluido <stdlib.h>
	Serpiente.cabeza->actual = { 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, VAO_cubo, 36, 0.0f };
	Serpiente.cabeza->next = NULL;
	Serpiente.cabeza->previous = NULL;

	// Creación y asignación del siguiente nodo
	Serpiente.cabeza->next = (piezaSerpiente*)malloc(sizeof(piezaSerpiente));

	Serpiente.cabeza->next->actual = { 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, VAO_cubo, 36, 0.0f };

	Serpiente.cabeza->next->next = NULL;
	Serpiente.cabeza->next->previous = Serpiente.cabeza;

	// Asignación de la cola
	Serpiente.cola = Serpiente.cabeza->next;
	
	camara = 0;//inicializamos la camara a 0

	//variables para llevar cuenta del tiempo
	double lastTime = glfwGetTime();
	double lapsoTime;
	double currentTime;

	srand(time(NULL));//instancio la semilla que utilizo para colocar la mandarina random
	situarMandarina();//coloco la mandarina en un lugar random

	// Lazo principal
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		//limpieza de los buffers
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		currentTime=glfwGetTime();//obtengo el tiempo actual
		srand(time(NULL));//instancio la semilla que utilizo para colocar la mandarina random
		lapsoTime = currentTime - lastTime;//calculo el lapso de tiempo entre el tiempo actual y la última vez que modifiqué la textura del agua
		
		if (lapsoTime > 0.3&& inicio==0&&fin==0) {//compruebo si ha pasado tiempo suficiente 
			if (direccion == 0)//up
				crearParteSerpiente(Serpiente.cabeza->actual.px, Serpiente.cabeza->actual.py + 0.5f);//muevo la serpiente hacia arriba
			else if (direccion ==1)//down
				crearParteSerpiente(Serpiente.cabeza->actual.px, Serpiente.cabeza->actual.py-0.5f);//muevo la serpiente hacia abajo
			else if (direccion == 2)//left
				crearParteSerpiente(Serpiente.cabeza->actual.px-0.5f, Serpiente.cabeza->actual.py);//muevo la serpiente hacia la izquierda
			else if (direccion == 3)//right
				crearParteSerpiente(Serpiente.cabeza->actual.px+0.5f, Serpiente.cabeza->actual.py);//muevo la serpiente hacia derecha
			lastTime = glfwGetTime();
		}
		
		
		if (inicio == 1) {
			glm::mat4 transformtemp; // Matriz temporal de transformación
			unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform"); // Obtiene la ubicación de la matriz de transformación en el shader
			glBindTexture(GL_TEXTURE_2D, texturaInicio);//aplico la textura
			transformtemp = glm::mat4(); // Matriz identidad
			dibujarObjeto(transformLoc, Inicio);//dibujo el objeto que contiene el mensaje
			if ((glfwGetTime() - lastTime) > 4) {//espero 4 s
				inicio = 0;//indico que ya no se muestre mas el mensaje de inicio
			}
		}

		if (fin == 1) {
			glm::mat4 transformtemp; // Matriz temporal de transformación
			unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform"); // Obtiene la ubicación de la matriz de transformación en el shader
			glBindTexture(GL_TEXTURE_2D, texturaFin);//aplico la textura
			transformtemp = glm::mat4(); // Matriz identidad
			dibujarObjeto(transformLoc, gameOver);//dibujo el objeto que contiene el mensaje
			if ((glfwGetTime() - lastTime) > 4) {//espero 4 s
				//termino todo y salgo de la ventana
			
				//Vacio los bufferse
				glDeleteVertexArrays(1, &VAO_cuadrado);
				glDeleteVertexArrays(1, &VAO_esfera);
				glDeleteVertexArrays(1, &VAO_cubo);
				// Termino todo.
				glfwTerminate();
				return 0;
			}
		}

		switch (camara){//posiciono la camara dependiendo de la seleccionada
			case 0:
				myCamara();
				break;
			case 1:
				myTerceraPers();
				break;
		}

		display();//dibujo los objetos

		// limpio los buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Como se acabo vacio los bufferse
	glDeleteVertexArrays(1, &VAO_cuadrado);
	glDeleteVertexArrays(1, &VAO_esfera);
	glDeleteVertexArrays(1, &VAO_cubo);

	// Termino todo.
	glfwTerminate();
	return 0;
}