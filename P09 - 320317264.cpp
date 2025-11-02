/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza = true;

bool avanzaDragon = true;
float movOffsetDragon = 0.1f;    
float angulovaria = 0.0f;
float anguloAla = 0.0f;
float dragonavance = 0.0f;
float UP1;
float UP2;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float velocidad = 0.0;
float caeDado, giraDado;
float caeDadoOffset, giraDadoOffset;
bool caer, girar;
float tiempoGiro = 0.0f;


// --- Variables globales ---
float desplazamientoLetrero = 0.0f;
float velocidadLetrero = 0.008f; // velocidad del movimiento (ajústala)
float limiteIzquierda = -16.0f; // cuando sale completamente del lado izquierdo
float limiteDerecha = 16.0f;    // posición inicial de reinicio

static int contadorFrames = 0;

static int contadorFrame = 0;         
static bool mostrarNumero1 = true;     

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture PuertaD;
Texture PuertaI;
Texture Tipografia;
Texture dadoTexture2;

Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;
Model AlaDerecha_M;
Model AlaIzquierda_M;
Model Arco_M;
Model Letrero_M;
Model PuertaD_M;
Model PuertaI_M;
Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




GLuint VAO_letrero, VBO_letrero;

void CrearLetrero()
{
	struct Letra {
		float u1, v1, u2, v2;
		bool esEspacio;
	};

	std::vector<Letra> letras = {
		{0.79f, 0.84f, 0.84f, 0.9f, false},   // P
		{0.89f, 0.84f, 0.93f, 0.9f, false},   // R
		{0.74f, 0.84f, 0.77f, 0.9f, false},   // O
		{0.27f, 0.70f, 0.30f, 0.76f, false},  // Y
		{0.22f, 0.84f, 0.25f, 0.9f, false},   // E
		{0.1055f, 0.84f, 0.15f, 0.9f, false}, // C
		{0.01f, 0.70f, 0.04f, 0.76f, false},  // T
		{0.74f, 0.84f, 0.77f, 0.9f, false},   // O
		{0.0f, 0.0f, 0.0f, 0.0f, true},       // espacio
		{0.1055f, 0.84f, 0.15f, 0.9f, false}, // C
		{0.32f, 0.84f, 0.35f, 0.9f, false},   // G
		{0.22f, 0.84f, 0.25f, 0.9f, false},   // E
		{0.42f, 0.84f, 0.46f, 0.9f, false},   // I
		{0.37f, 0.84f, 0.41f, 0.9f, false},   // H
		{0.1055f, 0.84f, 0.15f, 0.9f, false}  // C
	};

	std::vector<GLfloat> vertices;
	float size = 0.5f;

	for (auto& l : letras)
	{
		//Si es espacio, NO crear ningún quad (solo se deja hueco)
		if (l.esEspacio) {
			vertices.insert(vertices.end(), { 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 });
			continue;
		}

		GLfloat cuad[] = {
			-size / 2, -size / 2, 0.0f,  l.u1, l.v1,
			 size / 2, -size / 2, 0.0f,  l.u2, l.v1,
			 size / 2,  size / 2, 0.0f,  l.u2, l.v2,
			-size / 2,  size / 2, 0.0f,  l.u1, l.v2
		};

		vertices.insert(vertices.end(), cuad, cuad + 20);
	}

	if (VAO_letrero == 0) glGenVertexArrays(1, &VAO_letrero);
	if (VBO_letrero == 0) glGenBuffers(1, &VBO_letrero);

	glBindVertexArray(VAO_letrero);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_letrero);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}


void CrearDado8Caras()
{
	// Índices de las 8 caras del octaedro (triángulos)
	unsigned int indices[] = {
		0, 1, 2,     // Cara 1 
		3, 4, 5,     // Cara 2 
		6, 7, 8,     // Cara 3 
		9, 10, 11,   // Cara 4 
		12, 13, 14,  // Cara 5 
		15, 16, 17,  // Cara 6
		18, 19, 20,  // Cara 7 
		21, 22, 23   // Cara 8 
	};


	GLfloat vertices[] = {
		// Cara 1 centro (frontal superior)
	   -0.5f, 0.0f,  0.5f,   0.25f, 0.5f,    0.0f,  -1.0f, -1.0f,
		0.5f, 0.0f,  0.5f,   0.75f, 0.5f,    0.0f,  -1.0f, -1.0f,
		0.0f, 0.65f, 0.0f,   0.5f,  0.75f,   0.0f,  -1.0f, -1.0f,

		// Cara 2 superior derecha
		0.5f, 0.0f,  0.5f,   0.75f, 0.5f,    -1.0f,  -1.0f,  0.0f,
		0.5f, 0.0f, -0.5f,   1.0f,  0.75f,   -1.0f,  -1.0f,  0.0f,
		0.0f, 0.65f, 0.0f,   0.5f,  0.75f,   -1.0f,  -1.0f,  0.0f,

		// Cara 3 trasera
		0.5f, 0.0f, -0.5f,   0.25f, 1.0f,    0.0f,  -1.0f,  1.0f,
	   -0.5f, 0.0f, -0.5f,   0.0f,  0.75f,   0.0f,  -1.0f,  1.0f,
		0.0f, 0.65f, 0.0f,   0.5f,  0.75f,   0.0f,  -1.0f,  1.0f,

		// Cara 4 superior izquierda
	   -0.5f, 0.0f, -0.5f,   0.0f,  0.75f,  1.0f,  -1.0f,  0.0f,
	   -0.5f, 0.0f,  0.5f,   0.25f, 0.5f,   1.0f,  -1.0f,  0.0f,
		0.0f, 0.65f, 0.0f,   0.5f,  0.75f,  1.0f,  -1.0f,  0.0f,

		// Cara 5 inferior frontal
	   -0.5f, 0.0f,  0.5f,   0.25f, 0.5f,    0.0f, 1.0f, -1.0f,
		0.5f, 0.0f,  0.5f,   0.75f, 0.5f,    0.0f, 1.0f, -1.0f,
		0.0f, -0.65f, 0.0f,  0.5f,  0.25f,    0.0f, 1.0f, -1.0f,

		// Cara 6 inferior derecha
		0.5f, 0.0f,  0.5f,   0.75f, 0.5f,    -1.0f, 1.0f,  0.0f,
		0.5f, 0.0f, -0.5f,   1.0f,  0.25f,   -1.0f, 1.0f,  0.0f,
		0.0f, -0.65f, 0.0f,  0.5f,  0.25f,   -1.0f, 1.0f,  0.0f,

		// Cara 7 inferior trasera
		0.5f, 0.0f, -0.5f,   0.25f, 0.0f,    0.0f, 1.0f,  1.0f,
		-0.5f, 0.0f, -0.5f,  0.0f,  0.25f,   0.0f, 1.0f,  1.0f,
		0.0f, -0.65f, 0.0f,  0.5f,  0.25f,   0.0f, 1.0f,  1.0f,

		// Cara 8 inferior izquierda
	   -0.5f, 0.0f, -0.5f,   0.0f,  0.25f,  1.0f, 1.0f,  0.0f,
	   -0.5f, 0.0f,  0.5f,   0.25f, 0.5f,   1.0f, 1.0f,  0.0f,
		0.0f, -0.65f, 0.0f,  0.5f,  0.25f,  1.0f, 1.0f,  0.0f
	};

	Mesh* dado8 = new Mesh();
	// 24 vértices, 8 triángulos × 3 vértices = 24 índices
	dado8->CreateMesh(vertices, indices, 192, 24);
	meshList.push_back(dado8);
}
// --- Centros de las caras usando solo UV (columnas 4 y 5 de tus vértices) ---
glm::vec2 centrosCaras[8] = {
	glm::vec2(0.5f, 0.75f),  // cara 1
	glm::vec2(0.75f, 0.75f), // cara 2
	glm::vec2(0.5f, 1.0f),   // cara 3
	glm::vec2(0.25f, 0.75f), // cara 4
	glm::vec2(0.5f, 0.25f),  // cara 5
	glm::vec2(0.75f, 0.25f), // cara 6
	glm::vec2(0.5f, 0.0f),   // cara 7
	glm::vec2(0.25f, 0.25f)  // cara 8
};

// --- Combinaciones válidas de caras (cara arriba / cara abajo) ---
int combinaciones[4][2] = {
	{1, 5}, // frontal
	{2, 6}, // derecha
	{3, 7}, // trasera
	{4, 8}  // izquierda
};

// --- Función para rotar el octaedro ---
glm::mat4 rotarOctaedro(int caraArriba, int caraAbajo) {
	// --- Obtener centros UV de las caras seleccionadas ---
	glm::vec2 uvArriba = centrosCaras[caraArriba - 1];
	glm::vec2 uvAbajo = centrosCaras[caraAbajo - 1];

	// --- Convertir UV a vectores 3D aproximados para rotación ---
	// Usamos solo la componente Y para "arriba" y X/Z para "forward"
	glm::vec3 dirArriba = glm::normalize(glm::vec3(uvArriba.x - 0.5f, 1.0f, uvArriba.y - 0.5f));
	glm::vec3 dirAbajo = glm::normalize(glm::vec3(uvAbajo.x - 0.5f, -1.0f, uvAbajo.y - 0.5f));

	// --- Rotación para alinear la cara superior con +Y ---
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 axis = glm::cross(dirArriba, up);
	float angle = acos(glm::clamp(glm::dot(dirArriba, up), -1.0f, 1.0f));
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::normalize(axis));

	// --- Rotación alrededor de Y para que la cara opuesta quede hacia abajo ---
	glm::vec3 forward = glm::vec3(0, 0, 1);
	forward = glm::vec3(rot * glm::vec4(forward, 0.0f));
	float yaw = atan2(forward.x, forward.z);
	rot = glm::rotate(rot, -yaw, up);

	return rot;
}

glm::mat4 obtenerRotacionFinal(int idxCombinacion)
{
	// Vector “forward” inicial del modelo (cara mirando +Z)
	glm::vec3 forwardInicial = glm::vec3(0, 0, 1);
	glm::vec3 upDeseado = glm::vec3(0, 1, 0);

	// Rotación principal para apuntar +Z → +Y
	glm::vec3 axis = glm::cross(forwardInicial, upDeseado);
	float angle = acos(glm::clamp(glm::dot(glm::normalize(forwardInicial), glm::normalize(upDeseado)), -1.0f, 1.0f));
	glm::mat4 rotacionPrincipal = glm::rotate(glm::mat4(1.0f), angle, glm::normalize(axis));

	// Rotación alrededor de +Y para elegir la cara específica arriba
	float anguloCara = 0.0f;
	switch (idxCombinacion)
	{
	case 0: anguloCara = 180.0f; break;    // cara 1 arriba
	case 1: anguloCara = 90.0f; break;   // cara 2 arriba
	case 2: anguloCara = 180.0f; break;  // cara 3 arriba
	case 3: anguloCara = -90.0f; break;  // cara 4 arriba
	}

	glm::mat4 rotacionCara = glm::rotate(glm::mat4(1.0f), glm::radians(anguloCara), glm::vec3(0, 1, 0));

	// Combinar ambas rotaciones
	return rotacionCara * rotacionPrincipal;
}



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

	
	CrearLetrero();
	CrearDado8Caras();

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	Tipografia = Texture("Textures/tipografia.tga");
	Tipografia.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

	dadoTexture2 = Texture("Textures/dado_8_caras2.png");
	dadoTexture2.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");

	Dragon_M = Model();
	Dragon_M.LoadModel("Models/dragoncuerpo.fbx");

	AlaDerecha_M = Model();
	AlaDerecha_M.LoadModel("Models/aladerecha.fbx");


	AlaIzquierda_M = Model();
	AlaIzquierda_M.LoadModel("Models/alaizquierda.fbx");

	Arco_M = Model();
	Arco_M.LoadModel("Models/arco.fbx");

	PuertaD_M = Model();	
	PuertaD_M.LoadModel("Models/puertaderecha.fbx");

	PuertaD_M = Model();
	PuertaD_M.LoadModel("Models/puertaD.fbx");

	PuertaI_M = Model();
	PuertaI_M.LoadModel("Models/puertaizquierda.fbx");

	Letrero_M = Model();
	Letrero_M.LoadModel("Models/letrero.fbx");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.3f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelarco(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	caeDado = 0.0f;
	caeDadoOffset = 0.05f;
	giraDado = 0.0f;
	giraDadoOffset = 0.5f;
	caer = true;
	girar = true;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 8.0f * deltaTime;
		//dragonavance
		anguloAla += 8.0f * deltaTime;
		float aleteo = sin(glm::radians(anguloAla)) * 30; // 30° es la amplitud del movimiento


		// BANDERA DE AVANCE Y REGRESO
		if (avanzaDragon) {
			if (dragonavance > -20.0f) {
				dragonavance -= movOffsetDragon * deltaTime;
			}
			else {
				avanzaDragon = !avanzaDragon; // cambia dirección
			}
		}
		else {
			if (dragonavance < 0.0f) {
				dragonavance += movOffsetDragon * deltaTime;
			}
			else {
				avanzaDragon = !avanzaDragon; // regresa a avanzar
			}
		}

		// ANIMACION EN LOOP
		// Se ejecuta de forma continua mientres la aplicaión está activa
		// Si la animación no es en loop y se puede iniciar vasrias veces
		// El estado final y el estado inicial deben de ser el mismo, o
		// agregar elementos para que no se vea que los modelos desaparecen 
		// o aparecen de la nada
		//bandera
		if (avanza) {
			if (movCoche > -250.0f)
			{
				movCoche -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
				rotllanta -= rotllantaOffset * deltaTime;
			}
			else {
				avanza = !avanza;
			}
		}

		else
		{
			if (movCoche < 325.0f) {
				movCoche += movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
				rotllanta += rotllantaOffset * deltaTime;
			}
			else {
				avanza = !avanza;
			}
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);







		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		modelarco = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);


		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();



		//Arco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, 2.6f, -6.0f));
		modelarco = model;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arco_M.RenderModel();


		// --- Puerta derecha ---
		if (mainWindow.getPuertaDerechaMoviendose()) {
			velocidad = 0.15f; // unidades por segundo

			float z = mainWindow.getPuertaD_Z();
			float x = mainWindow.getPuertaD_X();

			if (mainWindow.getPuertaDerechaAbierta()) {
				// Abrir: mover en +Z y +X
				if (z < 0.5f) {
					z += velocidad * deltaTime;
					if (z > 0.5f) z = 0.5f;
				}
				else if (x < 2.5f) {
					x += velocidad * deltaTime;
					if (x > 2.5f) x = 2.5f;
				}
				else {
					mainWindow.setPuertaDerechaMoviendose(false); // terminó abrir
				}
			}
			else {
				// Cerrar: regresar a posición inicial
				if (x > 1.85f) {
					x -= velocidad * deltaTime;
					if (x < 1.85f) x = 1.85f;
				}
				else if (z > -0.8f) {
					z -= velocidad * deltaTime;
					if (z < -0.8f) z = -0.8f;
				}
				else {
					mainWindow.setPuertaDerechaMoviendose(false); // terminó cerrar
				}
			}

			mainWindow.setPuertaD_X(x);
			mainWindow.setPuertaD_Z(z);
		}

		//Puerta derecha
		model = modelarco;
		model = glm::translate(model, glm::vec3(
			mainWindow.getPuertaD_X(),
			mainWindow.getPuertaD_Y(),
			mainWindow.getPuertaD_Z()
		));

		model = glm::scale(model, glm::vec3(0.9f, 1.5f, 2.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaD_M.RenderModel();

		
		//Pueta izquierda 
		velocidad = 10.0f; // grados por segundo
		if (mainWindow.getPuertaMoviendose()) {
			float angulo = mainWindow.getAnguloPuertaI();

			if (mainWindow.getPuertaAbierta()) { // abrir
				angulo -= velocidad * deltaTime;
				if (angulo <= -90.0f) {
					angulo = -90.0f;
					mainWindow.setPuertaMoviendose(false); // termina animación
				}
			}
			else { // cerrar
				angulo += velocidad * deltaTime;
				if (angulo >= 0.0f) {
					angulo = 0.0f;
					mainWindow.setPuertaMoviendose(false); // termina animación
				}
			}

			mainWindow.setAnguloPuertaI(angulo);
		}

		model = modelarco;
		model = glm::translate(model, glm::vec3(-1.8f, -3.1f, -0.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getAnguloPuertaI()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.47f, 2.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaI_M.RenderModel();

;

		// === MODELO DEL LETRERO (base física) ===
		model = modelarco;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, -1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 2.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero_M.RenderModel();

		/// === ANIMACIÓN DEL TEXTO DEL LETRERO ===

		// Texto base
		std::string texto = "PROYECTO CGEIHC ";
		int totalLetras = texto.size();
		float tiempoCambio = 0.05f; // tiempo 

		
		static float acumuladorTiempo = 0.0f;
		static int desplazamientoLetrero = 0;

		acumuladorTiempo += deltaTime / 1000.0f;

		if (acumuladorTiempo >= tiempoCambio) {
			desplazamientoLetrero = (desplazamientoLetrero + 1) % totalLetras;
			acumuladorTiempo = 0.0f;
		}

		// Letrero
		glm::mat4 modelTexto = modelarco;
		modelTexto = glm::translate(modelTexto, glm::vec3(0.0f, -1.5f, -1.0f));
		modelTexto = glm::scale(modelTexto, glm::vec3(1.5f, 2.5f, 2.8f));
		modelTexto = glm::translate(modelTexto, glm::vec3(-0.5f, 0.25f, 0.3f)); // un poco al frente

		Tipografia.UseTexture();
		glBindVertexArray(VAO_letrero);

	
		int letrasVisibles = 4;   
		float anchoLetra = 0.5f; 
		float inicio = -0.18f;    

		
		for (int i = 0; i < letrasVisibles; i++) {
			int indiceLetra = (desplazamientoLetrero + i) % totalLetras;

			glm::mat4 modelLetra = modelTexto;

	
			float posLetra = inicio + i * anchoLetra;
			modelLetra = glm::translate(modelLetra, glm::vec3(posLetra, 0.0f, 0.0f));
			modelLetra = glm::scale(modelLetra, glm::vec3(0.7f));

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLetra));

			glDrawArrays(GL_TRIANGLE_FAN, indiceLetra * 4, 4);
		}

		glBindVertexArray(0);

		
		// DADO 
		


		// -- VARIABLES DE CONTROL --
		float velocidadCaida = 0.02f;
		float velocidadGiro = 10.0f;
		float alturaFinal = 0.0f;

		static glm::mat4 rotacionFinal = glm::mat4(1.0f); // rotación final al aterrizar
		float giroZ = 0.0f;                               // Z fija al aterrizar
		float giroY = 0.0f;                                // Y aleatoria al aterrizar

		// --- CAÍDA DEL DADO ---
		if (mainWindow.getDadoCayendo()) {
			float nuevaY = mainWindow.getCaeDado() - velocidadCaida * deltaTime;

			if (nuevaY <= alturaFinal) {
				nuevaY = alturaFinal;
				mainWindow.setDadoCayendo(false);
				mainWindow.setDadoAterrizado(true);

				// Elegir combinación de caras válida al azar
				int idx = rand() % 4;
				int caraArriba = combinaciones[idx][0];
				mainWindow.setDadoNumero(caraArriba);

				// Aplicar rotación Y aleatoria al aterrizar
				int angulosY[4] = { 90, 180, 270, 360 };
				giroY = static_cast<float>(angulosY[rand() % 4]);

				// Alternar giro Z entre 120° 
				int angulosZ[2] = { 120, 60 };
				giroZ = static_cast<float>(angulosZ[rand() % 2]);

				// Crear la rotación final: Z aleatoria (120 o 45) + Y aleatoria
				rotacionFinal = glm::rotate(glm::mat4(1.0f), glm::radians(giroZ), glm::vec3(0, 0, 1));
				rotacionFinal = glm::rotate(rotacionFinal, glm::radians(giroY), glm::vec3(0, 1, 0));

				mainWindow.setGiraDado(0.0f); // reinicia rotación libre
			}

			mainWindow.setCaeDado(nuevaY);
			mainWindow.setGiraDado(mainWindow.getGiraDado() + velocidadGiro * deltaTime);
		}

		// --- CREAR MODEL ---
		glm::mat4 traslacion = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, alturaFinal + mainWindow.getCaeDado(), 6.0f));
		glm::mat4 model = glm::mat4(1.0f);

		if (mainWindow.getDadoAterrizado()) {
			// Aplicar traslación + rotación final (Z=90 + Y aleatoria)
			model = traslacion * rotacionFinal;
		}
		else {
			// Rotación libre mientras cae
			model = glm::rotate(traslacion, glm::radians(mainWindow.getGiraDado()), glm::vec3(1.0f, 1.0f, 1.0f));
		}

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture2.UseTexture();
		meshList[7]->RenderMesh();

		



		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		// Matriz base del dragón
		glm::mat4 modelDragon = glm::mat4(1.0f);
		modelDragon = glm::translate(modelDragon, glm::vec3(0.05f + dragonavance, 5.0f + sin(glm::radians(angulovaria)), 6.0f));
		modelDragon = glm::scale(modelDragon, glm::vec3(0.3f, 0.3f, 0.3f));
		modelDragon = glm::rotate(modelDragon, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelDragon));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Dragon_M.RenderModel();

		//AlaDerecha
		glm::mat4 modelAlaDerecha = modelDragon;
		modelAlaDerecha = glm::translate(modelAlaDerecha, glm::vec3(-1.0f, 0.0f, 3.5f)); 
		modelAlaDerecha = glm::rotate(modelAlaDerecha, glm::radians(-aleteo), glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAlaDerecha));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		AlaDerecha_M.RenderModel();

		//Alaizquierdda
		glm::mat4 modelAlaIzquierda = modelDragon; // también hereda del cuerpo
		modelAlaIzquierda = glm::translate(modelAlaIzquierda, glm::vec3(-1.0f, 0.0f, 3.5f)); 
		modelAlaIzquierda = glm::rotate(modelAlaIzquierda, glm::radians(aleteo), glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAlaIzquierda));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		AlaIzquierda_M.RenderModel();




	


		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		
		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		for (int i = 1; i<4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		 }

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}
 

		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
	

		// Cada frame:
		contadorFrames++;

		// Contador para los frames
		if (contadorFrames >= 5000)
		{
			toffsetnumerocambiau += 0.25f; // Avanza al siguiente número
			if (toffsetnumerocambiau > 1.0f)
				toffsetnumerocambiau = 0.0f;

			contadorFrames = 0; // Reiniciar contador
		}

		toffsetnumerov = 0.0f;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);



		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		// cambiar automáticamente entre textura número 1 y número 2
		

		contadorFrame++;

		// Cambiar cada 60 frames (aprox. 1 segundo si va a 60 FPS)
		if (contadorFrame >= 5000)
		{
			mostrarNumero1 = !mostrarNumero1;  // Alternar entre número 1 y 2
			contadorFrame = 0;                // Reiniciar el contador
		}

		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Alternar automáticamente entre las dos texturas
		if (mostrarNumero1)
			Numero1Texture.UseTexture();
		else
			Numero2Texture.UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();





		glDisable(GL_BLEND);
		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
