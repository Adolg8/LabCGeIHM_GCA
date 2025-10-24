/*
Práctica 7: Iluminación 1 
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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


Texture AgaveTexture;
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
Texture lampara;
Texture litwick;
Texture pared;

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
Model Blackhawk_M;
Model Lampara_M;
Model Pared_M;
Model Litwick_M;

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



//función de calculo de normales por promedio de vértices 
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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

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

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

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

	pared = Texture("Textures/kirby.jpg");
	pared.LoadTextureA();

	llantas = Texture("Textures/llantasmcqueen.png");
	llantas.LoadTextureA();
	cofre = Texture("Textures/cofre.png");
	cofre.LoadTextureA();

	ojos = Texture("Textures/ojos.png");
	ojos.LoadTextureA();

	Parillada = Texture("Textures/parillada.png");
	Parillada.LoadTextureA();


	Rayo_M = Model();
	Rayo_M.LoadModel("Models/cuerporayo.dae");
	

	Cofre_M = Model();
	Cofre_M.LoadModel("Models/cofre.fbx");

	Cofre2_M = Model();
	Cofre2_M.LoadModel("Models/cofre2.fbx");

	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llantadelanteraderecha.fbx");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
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

	Lampara_M = Model();
	Lampara_M.LoadModel("Models/lampara.obj");

	Pared_M = Model();
	Pared_M.LoadModel("Models/pared.fbx");

	Litwick_M = Model();
	Litwick_M.LoadModel("Models/litwick.fbx");

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

	pointLights[0] = PointLight(
		1.0f, 1.0f, 1.0f,
		1.0f, 1.5f,
		0.0f, 0.0f, 0.0,
		1.0f, 0.01f, 0.03f
	);

	pointLightCount++;

	pointLights[1] = PointLight(
		0.7f, 0.3f, 0.9f,
		1.0f, 1.5f,
		0.0f, 0.0f, 0.0,
		1.0f, 0.01f, 0.03f
	);

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



	// Luz spotlight amarilla (cofre)
	spotLights[1] = SpotLight(
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		5.0f
	);
	spotLightCount++;

	

	// Luz avanza
	spotLights[2] = SpotLight(
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		5.0f
	);
	spotLightCount++;

	// Luz retroceso
	spotLights[3] = SpotLight(
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f
	);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	glm::mat4 modelLampara(1.0);
	glm::mat4 modelLitwick(1.0);
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);





		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), -0.8f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rayo_M.RenderModel();

		


		//Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, 0.6f, 0.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		// Rotar para que mire al frente (igual que el coche rojo)
		model = glm::scale(model, glm::vec3(1.1f, 1.3f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cofre.UseTexture();
		Cofre_M.RenderModel();



		// --- PARÁMETROS DEL COCHE Y COFRE ---
		glm::vec3 posicionCoche = glm::vec3(0.0f + mainWindow.getmuevex(), -0.8f, 1.0f);
		glm::vec3 pivoteCofre = glm::vec3(-0.0f, 0.6f, 0.2f);
		glm::vec3 Faro = glm::vec3(0.0f, 0.0f, 0.4f); 
		glm::vec3 direccionBase = glm::vec3(0.0f, 0.0f, 1.0f); 

		// MatrizCofre
		glm::mat4 matrizCofreMundo = glm::mat4(1.0f);
		matrizCofreMundo = glm::translate(matrizCofreMundo, posicionCoche);                  
		matrizCofreMundo = glm::rotate(matrizCofreMundo, glm::radians(-90.0f), glm::vec3(0, 1, 0)); 
		matrizCofreMundo = glm::translate(matrizCofreMundo, pivoteCofre);                   
		matrizCofreMundo = glm::rotate(matrizCofreMundo, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1, 0, 0));

		// --- POSICIÓN FINAL DEL FARO ---
		glm::vec3 posicionFaro = glm::vec3(matrizCofreMundo * glm::vec4(Faro, 1.0f));

		// --- DIRECCIÓN FINAL DEL FARO ---
		glm::vec3 direccionFaro = glm::normalize(glm::vec3(matrizCofreMundo * glm::vec4(direccionBase, 0.0f)));

		// --- ACTUALIZAR LA SPOTLIGHT EXISTENTE ---
		spotLights[1].SetFlash(posicionFaro, direccionFaro);



		//Luces coche
		// Posición del coche
		glm::vec3 posCoche = glm::vec3(mainWindow.getmuevex(), -0.8f, 1.0f);

		// Actualizamos dirección de luces del coche 
		spotLights[2].SetFlash(posCoche, glm::vec3(-1.0f, 0.0f, 0.0f)); // delantero
		spotLights[3].SetFlash(posCoche, glm::vec3(1.0f, 0.0f, 0.0f));  // trasero

	

		// Preparar arreglo de luces activas
		SpotLight lucesActivas[2]; // coche y cofre
		int spotActivos = 0;

		// Luz del coche según movimiento
		if (mainWindow.getFocoAvanza()) {
			lucesActivas[spotActivos++] = spotLights[2]; // delantero
		}
		else if (mainWindow.getFocoRetrocede()) {
			lucesActivas[spotActivos++] = spotLights[3]; // trasero
		}

		// Luz del cofre 
		lucesActivas[spotActivos++] = spotLights[1];

		// Enviar solo luces activas al shader
		shaderList[0].UseShader();
		shaderList[0].SetSpotLights(lucesActivas, spotActivos);






		//Ojos
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.3f, 1.0f, -0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		// Rotar para que mire al frente (igual que el coche rojo)
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ojos.UseTexture();
		Ojos_M.RenderModel();


		//Parillada

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		// Rotar para que mire al frente (igual que el coche rojo)
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Parillada.UseTexture();
		Parillada_M.RenderModel();

		//LLantaIzquierdaDelantera
		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.2f, -0.6f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantas.UseTexture();
		LlantaD1_M.RenderModel();


		//LLantaDerechaDelantera
		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.2f, 0.6f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantas.UseTexture();
		LlantaD2_M.RenderModel();


		//LlantaDerechaTrasera
		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.2f, -0.6f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantas.UseTexture();
		LlantaT1_M.RenderModel();

		//LlantaIzquierdaTrasera
		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.2f, 0.6f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantas.UseTexture();
		LlantaT2_M.RenderModel();



		//Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getarticulacion6(), 5.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();





		

		//Lampara
		modelLampara = glm::mat4(1.0);
		modelLampara = glm::translate(modelLampara, glm::vec3(-1.0f, -1.0f, 16.0));
		modelLampara = glm::rotate(modelLampara, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLampara));
		Lampara_M.RenderModel();


		glm::vec3 posLamparaLocal = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 posLamparaGlobal = glm::vec3(modelLampara * glm::vec4(posLamparaLocal, 1.0f));

		// 1. Actualiza la posición de la Lámpara (PointLight[0])
		pointLights[0].SetPos(posLamparaGlobal);


		//Litwick
		modelLitwick = glm::mat4(1.0);
		modelLitwick = glm::translate(modelLitwick, glm::vec3(-1.0f, -1.0f, 25.0));
		modelLitwick = glm::scale(modelLitwick, glm::vec3(0.05f, 0.05f, 0.05f));
		modelLitwick = glm::rotate(modelLitwick, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLitwick));
		Litwick_M.RenderModel();

		glm::vec3 posLitwick = glm::vec3(0.0f, 150.0f, -100.0f);
		glm::vec3 posLitwickGlobal = glm::vec3(modelLitwick * glm::vec4(posLitwick, 1.0f));

		// 2. Actualiza la posición de Litwick (PointLight[1])
		pointLights[1].SetPos(posLitwickGlobal);


		//APAGADO Y PRENDIDO DE LUCESD
		PointLight lucesPuntoActivas[2];
		unsigned int pointActivos = 0;

		// Evalúa y agrega la Lámpara (pointLights[0]) si está activa
		if (mainWindow.getLuzlampara()) {
			lucesPuntoActivas[pointActivos++] = pointLights[0];
		}

		// Evalúa y agrega Litwick (pointLights[1]) si está activo
		if (mainWindow.getLuzLitwick()) {
			lucesPuntoActivas[pointActivos++] = pointLights[1];
		}

		// Envía el arreglo temporal (con 0, 1 o 2 luces) al shader.
		// ESTA ES LA ÚNICA LLAMADA NECESARIA.
		shaderList[0].SetPointLights(lucesPuntoActivas, pointActivos);

	
		//Pared
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 6.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.25f, 0.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pared.UseTexture();
		Pared_M.RenderModel();

		

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
