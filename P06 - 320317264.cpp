/*
Práctica 6: Texturizado
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
#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture dadoTexture2;
Texture logofiTexture;
Texture llantas;
Texture cofre;
Texture ojos;
Texture Parillada;


Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Dado2_M;
Model Rayo_M;
Model Cofre_M;
Model Cofre2_M;
Model LlantaD1_M;
Model LlantaD2_M;
Model LlantaT1_M;
Model LlantaT2_M;
Model Ojos_M;
Model Parillada_M;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




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
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
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

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// parte frontal de cubo

		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3

		// parte trasera del cubo

		-0.5f, -0.5f, -0.5f,	0.99f,  0.34f,		0.0f,	0.0f,	1.0f,	//4
		0.5f, -0.5f, -0.5f,		0.76f,	0.34f,		0.0f,	0.0f,	1.0f,	//5
		0.5f,  0.5f, -0.5f,		0.76f,	0.66f,		0.0f,	0.0f,	1.0f,	//6
		-0.5f,  0.5f, -0.5f,	0.99f,	0.66f,		0.0f,	0.0f,	1.0f,	//7
			
		//cara derecha del cubo
		0.5f, -0.5f,  0.5f,	    0.01f,  0.34f,		-1.0f,	0.0f,	0.0f,	//8
		0.5f, -0.5f,  -0.5f,	0.24f,	0.34f,		-1.0f,	0.0f,	0.0f,	//9
		0.5f,  0.5f,  -0.5f,	0.24f,	0.65f,		-1.0f,	0.0f,	0.0f,	//10
		0.5f,  0.5f,  0.5f,	    0.01f,	0.65f,		-1.0f,	0.0f,	0.0f,	//11

		

	
		// cara izquierda
		-0.5f, -0.5f,  -0.5f,	0.51f,  0.34f,		1.0f,	0.0f,	0.0f,	//12
		-0.5f, -0.5f,  0.5f,	0.74f,	0.34f,		1.0f,	0.0f,	0.0f,	//13
		-0.5f,  0.5f,  0.5f,	0.74f,	0.65f,		1.0f,	0.0f,	0.0f,	//14
		-0.5f,  0.5f,  -0.5f,	0.51f,	0.65f,		1.0f,	0.0f,	0.0f,	//15


		// parte de abajo del cubo
		
		-0.5f, -0.5f,  0.5f,	0.74f,  0.99f,		0.0f,	1.0f,	0.0f,	//16  abajo a la izquierda
		0.5f,  -0.5f,  0.5f,	0.74f,	0.67f,		0.0f,	1.0f,	0.0f,	//17 abajo a la derecha 
		 0.5f,  -0.5f,  -0.5f,	0.51f,  0.67f,		0.0f,	1.0f,	0.0f,	//18 arriba a la derecha
		-0.5f, -0.5f,  -0.5f,	0.51f,	0.99f,		0.0f,	1.0f,	0.0f,	//19 arriba a la izquierda

		// parte superior del cubo
		 -0.5f, 0.5f,  0.5f,	0.74f,  0.01f,		0.0f,	-1.0f,	0.0f,	//20
		 0.5f,  0.5f,  0.5f,	0.74f,	0.33f,		0.0f,	-1.0f,	0.0f,	//21
		  0.5f, 0.5f,  -0.5f,	0.51f,	0.33f,		0.0f,	-1.0f,	0.0f,	//22
		 -0.5f, 0.5f,  -0.5f,	0.51f,	0.01f,		0.0f,	-1.0f,	0.0f,	//23

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

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

	// Vértices: x, y, z, S, T, NX, NY, NZ
	GLfloat vertices[] = {
		// Cara 1 centro
	   -0.5f, 0.0f,  0.5f,   0.25f, 0.5f,    0.0f, 0.707f, 0.707f,
		0.5f, 0.0f,  0.5f,   0.75f, 0.5f,    0.0f, 0.707f, 0.707f,
		0.0f, 0.65f, 0.0f,   0.5f, 0.75f,    0.0f, 0.707f, 0.707f,

		// Cara 2 superior derecha
		0.5f, 0.0f,  0.5f,   0.75f, 0.5f,    0.707f, 0.707f, 0.0f,
		0.5f, 0.0f, -0.5f,   1.0f, 0.75f,    0.707f, 0.707f, 0.0f,
		0.0f, 0.65f, 0.0f,   0.5f, 0.75f,    0.707f, 0.707f, 0.0f,

		// Cara 3 trasera
		0.5f, 0.0f, -0.5f,   0.25f, 1.0f,    0.0f, 0.707f, -0.707f,
	   -0.5f, 0.0f, -0.5f,   0.0f, 0.75f,    0.0f, 0.707f, -0.707f,
		0.0f, 0.65f, 0.0f,   0.5f, 0.75f,    0.0f, 0.707f, -0.707f,

		// Cara 4 superior izquierda
	   -0.5f, 0.0f, -0.5f,   0.0f, 0.75f,   -0.707f, 0.707f, 0.0f,
	   -0.5f, 0.0f,  0.5f,   0.25f, 0.5f,   -0.707f, 0.707f, 0.0f,
		0.0f, 0.65f, 0.0f,   0.5f, 0.75f,   -0.707f, 0.707f, 0.0f,

		// Cara 5 inferior frontal
	   -0.5f, 0.0f,  0.5f,   0.25f, 0.5f,    0.0f, -0.707f, 0.707f,
		0.5f, 0.0f,  0.5f,   0.75f, 0.5f,    0.0f, -0.707f, 0.707f,
		0.0f, -0.65f, 0.0f,  0.5f, 0.25f,    0.0f, -0.707f, 0.707f,

		// Cara 6 inferior derecha
		0.5f, 0.0f,  0.5f,   0.75f, 0.5f,    0.707f, -0.707f, 0.0f,
		0.5f, 0.0f, -0.5f,   1.0f, 0.25f,    0.707f, -0.707f, 0.0f,
		0.0f, -0.65f, 0.0f,  0.5f, 0.25f,    0.707f, -0.707f, 0.0f,

		// Cara 7 inferior trasera
		0.5f, 0.0f, -0.5f,   0.0f, 0.25f,    0.0f, -0.707f, -0.707f,
	   -0.5f, 0.0f, -0.5f,   0.25f, 0.0f,    0.0f, -0.707f, -0.707f,
		0.0f, -0.65f, 0.0f,  0.5f, 0.25f,    0.0f, -0.707f, -0.707f,

		// Cara 8 inferior izquierda
	   -0.5f, 0.0f, -0.5f,   0.25f, 0.5f,   -0.707f, -0.707f, 0.0f,
	   -0.5f, 0.0f,  0.5f,   0.0f, 0.25f,   -0.707f, -0.707f, 0.0f,
		0.0f, -0.65f, 0.0f,  0.5f, 0.25f,   -0.707f, -0.707f, 0.0f
	};

	Mesh* dado8 = new Mesh();
	// 24 vértices, 8 triángulos × 3 vértices = 24 índices
	dado8->CreateMesh(vertices, indices, 192, 24);
	meshList.push_back(dado8);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearDado8Caras();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_animales2.png");
	dadoTexture.LoadTextureA();

	dadoTexture2 = Texture("Textures/dado_8_caras2.png");
	dadoTexture2.LoadTextureA();

	logofiTexture = Texture("Textures/escudo_fi_color.png");
	logofiTexture.LoadTextureA();
	llantas = Texture("Textures/llantasmcqueen.png"); 
	llantas.LoadTextureA();
	cofre =Texture("Textures/cofre.png");
	cofre.LoadTextureA();

	ojos = Texture("Textures/ojos.png");
	ojos.LoadTextureA();

	Parillada = Texture("Textures/parillada.png");
	Parillada.LoadTextureA();


	Dado2_M = Model();
	Dado2_M.LoadModel("Models/Cuboanimalbueno.dae");
	
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");

	Rayo_M = Model();
	Rayo_M.LoadModel("Models/cuerporayo.dae");

	Cofre_M = Model();
	Cofre_M.LoadModel("Models/cofre.fbx");

	Cofre2_M= Model();
	Cofre2_M.LoadModel("Models/cofre2.fbx");

	LlantaD1_M = Model();
	LlantaD1_M.LoadModel("Models/llantadelanteraizquierda.fbx");

	LlantaD2_M = Model();
	LlantaD2_M.LoadModel("Models/llantadelanteraderecha.fbx");

	LlantaT1_M = Model();
	LlantaT1_M.LoadModel("Models/llantatraseraderecha.fbx");

	LlantaT2_M = Model();
	LlantaT2_M.LoadModel("Models/llantatraseraizquierda.fbx");

	Ojos_M = Model();
	Ojos_M.LoadModel("Models/ojos.fbx");

	Parillada_M = Model();
	Parillada_M.LoadModel("Models/parillada.fbx");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();





		//Reporte de práctica :
		// Ejercicio 1: Crear un dado de 8 caras y texturizarlo por medio de código
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.5f, 9.0f, -2.0f));
		model = glm::scale(model, glm::vec3(2.05f, 2.05f, 2.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture2.UseTexture();
		meshList[5]->RenderMesh();

		/*Ejercicio 2: Importar el modelo de su coche con sus 4 llantas acomodadas y tener texturizadas las 4 llantas (diferenciar caucho y rin)  y
		texturizar el logo de la Facultad de ingeniería en el cofre de su propio modelo de coche¨*/

		glm::mat4 modelaux(1.0);
		glm::mat4 modelllantaDD(1.0);
		glm::mat4 modelllantaDI(1.0);
		glm::mat4 modelllantaTD(1.0);
		glm::mat4 modelllantaTI(1.0);
		glm::mat4 modelojos(1.0);


		//Cuerpo rayo

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion6(), -1.77f, -1.5f));
		modelaux = model; //Cola y mandibula
		modelojos = model;
		modelllantaDD = model; //LLanta Derecha Delantera
		modelllantaDI = model; //Pata Izquierda Delantera
		modelllantaTD = model; //Pata Derecha Trasera
		modelllantaTI = model; //Pata Izquierda Trasera
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rayo_M.RenderModel();//modificar por el modelo sin las 4 patas y sin cola



		//Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.1f, 1.3f, -0.3f));
		// Rotar para que mire al frente (igual que el coche rojo)
		model = glm::scale(model, glm::vec3(1.1f, 1.3f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cofre.UseTexture();
		Cofre_M.RenderModel();

		//Ojos
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.68f, 1.9f, -0.0f));
		// Rotar para que mire al frente (igual que el coche rojo)
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ojos.UseTexture();
		Ojos_M.RenderModel();

		//Parillada

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.0f, 0.1f, -0.0f));
		// Rotar para que mire al frente (igual que el coche rojo)
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Parillada.UseTexture();
		Parillada_M.RenderModel();

		//LLantaIzquierdaDelantera
		model = modelllantaDI;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, -1.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantas.UseTexture();
		LlantaD1_M.RenderModel();

		//LLantaDerechaDelantera
		model = modelllantaDD;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantas.UseTexture();
		LlantaD2_M.RenderModel();

		//LlantaDerechaTrasera
		model = modelllantaTD;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 0.5f, -1.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantas.UseTexture();
		LlantaT1_M.RenderModel();

		//LlantaIzquierdaTrasera
		model = modelllantaTI;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 0.5f, 1.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantas.UseTexture();
		LlantaT2_M.RenderModel();
		


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/