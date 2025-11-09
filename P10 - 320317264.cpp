/* 
Animación por keyframes
La textura del skybox fue conseguida desde la página https ://opengameart.org/content/elyvisions-skyboxes?page=1
y edité en Gimp rotando 90 grados en sentido antihorario la imagen  sp2_up.png para poder ver continuidad.
Fuentes :
	https ://www.khronos.org/opengl/wiki/Keyframe_Animation
	http ://what-when-how.com/wp-content/uploads/2012/07/tmpcd0074_thumb.png
	*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <thread>
#include <chrono>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <fstream>   
#include <glew.h>
#include <glfw3.h>
#include <sstream>
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

//variables archivo
int guardoArchivo = 0;
int cicloCarga = 0;
int banderaCarga = 0;

//variables para animación
float braviaryavance = 0.0f;
bool avanzabraviary = true;
float movOffsetbraviary = 0.5f;
float anguloAlaBraviary = 0.0f;
float rotacionBraviary = 0.0f;


//Meta knight
bool playMetaKnight = false;   
int playIndexMK = 0;          
int i_curr_stepsMK = 0;        

//Carro Dedede
float desplazamientoCarro = 0.0f;       
float incrementoCarro = 0.3f;           
bool carroAvanzando = false;            

float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float angulovaria = 0.0f;


//Animaciones basicas

float anguloTapa = 0.0f;       
float incrementoTapa = 0.01f;
bool pokebolaAbierta = false;    

//Magolor

float desplazamientoMagolor = 0.0f;     
float incrementoMagolor = 0.01f;         
bool magolorSubiendo = false;           
bool magolorEnMovimiento = false;

//variables para keyframes
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, ciclo3, ciclo4, ciclo5, ciclo6, ciclo7, ciclo8 , ciclo9 = 0;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

//Camara Aerea
bool camaraAereaActiva = false;

//Camra Waddle
bool camaraTerceraPersonaActiva = false;
glm::vec3 posicionWaddle = glm::vec3(-250.0f, 3.5f, -2.0f);
float rotacionWaddle = 0.0f;
float velocidadWaddle = 0.5f;
float anguloPiernas = 0.0f;
bool caminando = false;
float velocidadGiro = 10.0f;    
float escalaWaddle = 3.0f;




// ----- Animaciones -----
float anguloBrazos = 0.0f;
bool sentidoBrazo = true;

float inclinacionCuerpoZ = 0.0f;
bool sentidoCuerpo = true;
bool sentidoPierna = true;

Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;


//ModelsSnorlax
Model CuerpoSnorlax_M;
Model PiernaDerecha_M;
Model PiernaIzquierda_M;
Model BrazoIzquierdo_M;
Model BrazoDerecho_M;

//Modelos Braviary
Model Braviary_M;
Model AlaDerechaB_M;
Model AlaIzquierdaB_M;

//Modelos Archen
Model Archen_M;
Model AlaDerechaA_M;
Model AlaIzquierdaA_M;


Model Blackhawk_M;


//Model Jiggglypuff
Model Jigglypuff_M;


//Model Meta Knight
Model MetaKnight_M;
Model BrazoDerechoMK_M;
Model BrazoIzquierdoMK_M;
Model piernaDerechaMK_M;	
Model piernaIzquierdaMK_M;
Model EspadaMK_M;
Model AlaDerechaMK_M;
Model AlaIzquierdaMK_M;
Model OjosMK_M;

//Model Pokebola
Model TapaPokebola_M;
Model Eevee_M;

//Model Carro Dedede
Model CarroDedede_M;


//Model Magolor
Model Magolor_M;

//Avatar Waddle
Model CuerpoWaddle_M;
Model BrazoWaddleD_M;
Model BrazoWaddleI_M;
Model PiernaWaddleD_M;
Model PiernaWaddleI_M;


//Escenario
Model Ring_M;
Model CentroPokemon_M;
Model Arbol_M;
Model EntradaPrehispanica_M;
Model MuroPrehispanico_M;
Model MuroPrehispanico2_M;
Model Pikachu_M;
Model Kirby_M;

Model Lampara_M;
Model PokeLampara_M;
Model Anotorcha_M;
Model Litwick_M;

Model Tribuna_M;
Model VigasLuz_M;
Model BolaLuz_M;
Model Anuncio_M;
Model Camino_M;

Model Policia_M;

Model BoteBasuraVacio_M;
Model ContenedoresEcologicos_M;
Model MesaSombrilla_M;
Model Banca_M;
Model Estatuas_M;

Model PuestoTacos_M;
Model PuestoPapas_M;
Model Trailer_M;


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

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

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
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;



//NEW// Keyframes
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;

//Snorlax
// Key Frames Snorlax
float movCuerpoSnorlax_x = 0.0f;
float movCuerpoSnorlax_z = 0.0f;
float giroSnorlax = 0.0f;
// Brazos Snorlax
float giroBrazoIzquierdo_x = 0.0f;
float giroBrazoIzquierdo_y = 0.0f;
float giroBrazoIzquierdo_z = 0.0f;
float movBrazoIzquierdo_x = 0.0f;   // Movimiento lineal en X
float movBrazoIzquierdo_z = 0.0f;   // Movimiento lineal en Z

float giroBrazoDerecho_x = 0.0f;
float giroBrazoDerecho_z = 0.0f;
float giroBrazoDe_y = 0.0f;
float movBrazoDerecho_x = 0.0f;     // Movimiento lineal en X
float movBrazoDerecho_z = 0.0f;     // Movimiento lineal en Z

// Piernas Snorlax
float giroPiernaIzquierda_x = 0.0f;
float giroPiernaIzquierda_z = 0.0f;
float movPiernaIzquierda_x = 0.0f;  // Movimiento lineal en X
float movPiernaIzquierda_z = 0.0f;  // Movimiento lineal en Z

float giroPiernaDerecha_x = 0.0f;
float giroPiernaDerecha_z = 0.0f;
float movPiernaDerecha_x = 0.0f;    // Movimiento lineal en X
float movPiernaDerecha_z = 0.0f;    // Movimiento lineal en Z


//META KNIGHT
float movMetaKnight_x = 0.0f;   // Posición en X
float movMetaKnight_y = 0.0f;   // Posición en Y
float movMetaKnight_z = 0.0f;   // Posición en Z

// Meta Knight - Giro
float giroMetaKnight_x = 0.0f;  // Rotación alrededor de X
float giroMetaKnight_y = 0.0f;  // Rotación alrededor de Y
float giroMetaKnight_z = 0.0f;  // Rotación alrededor de Z




#define MAX_FRAMES 100 //Número de cuadros máximos
int i_max_steps = 1000; //Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
int i_max_stepsMK = 3000;
int i_curr_steps = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;		//Variable para GiroAvion
	float giroAvionInc;		//Variable para IncrementoGiroAvion


	// Incrementos del cuerpo Snorlax
	float movCuerpoSnorlax_xInc;
	float movCuerpoSnorlax_zInc;
	float giroSnorlaxInc;

	
	// Brazos Snorlax
	float giroBrazoIzquierdo_xInc;
	float giroBrazoIzquierdo_zInc;
	float movBrazoIzquierdo_xInc;   // Movimiento lineal en X
	float movBrazoIzquierdo_zInc;   // Movimiento lineal en Z

	float giroBrazoDerecho_xInc;
	float giroBrazoDerecho_zInc;
	float movBrazoDerecho_xInc;     // Movimiento lineal en X
	float movBrazoDerecho_zInc;     // Movimiento lineal en Z

	// Piernas Snorlax
	float giroPiernaIzquierda_xInc;
	float giroPiernaIzquierda_zInc;
	float movPiernaIzquierda_xInc;  // Movimiento lineal en X
	float movPiernaIzquierda_zInc;  // Movimiento lineal en Z

	float giroPiernaDerecha_xInc;
	float giroPiernaDerecha_zInc;
	float movPiernaDerecha_xInc;    // Movimiento lineal en X
	float movPiernaDerecha_zInc;    // Movimiento lineal en Z


	// Incrementos Meta Knight
	// META KNIGHT - Movimiento lineal
	float movMetaKnight_xInc;   // Incremento en X
	float movMetaKnight_yInc;   // Incremento en Y
	float movMetaKnight_zInc;   // Incremento en Z

	// META KNIGHT - Giro
	float giroMetaKnight_xInc;  // Incremento de rotación alrededor de X
	float giroMetaKnight_yInc;  // Incremento de rotación alrededor de Y
	float giroMetaKnight_zInc;  // Incremento de rotación alrededor de Z

	//------------------------------------------------------------------------
	//Key Frames Snorlax
	float movCuerpoSnorlax_x;
	float movCuerpoSnorlax_z;
	float giroSnorlax;


	// Brazos Snorlax
	float giroBrazoIzquierdo_x;
	float giroBrazoIzquierdo_z;
	float movBrazoIzquierdo_x;   // Movimiento lineal en X
	float movBrazoIzquierdo_z;   // Movimiento lineal en Z

	float giroBrazoDerecho_x;
	float giroBrazoDerecho_z;
	float movBrazoDerecho_x;     // Movimiento lineal en X
	float movBrazoDerecho_z;     // Movimiento lineal en Z

	// Piernas Snorlax
	float giroPiernaIzquierda_x;
	float giroPiernaIzquierda_z;
	float movPiernaIzquierda_x;  // Movimiento lineal en X
	float movPiernaIzquierda_z;  // Movimiento lineal en Z

	float giroPiernaDerecha_x;
	float giroPiernaDerecha_z;
	float movPiernaDerecha_x;    // Movimiento lineal en X
	float movPiernaDerecha_z;    // Movimiento lineal en Z



	//META KNIGHT
	// META KNIGHT - Movimiento lineal
	float movMetaKnight_x;   // Posición en X
	float movMetaKnight_y;   // Posición en Y
	float movMetaKnight_z;   // Posición en Z

	// META KNIGHT - Giro
	float giroMetaKnight_x;  // Rotación alrededor de X
	float giroMetaKnight_y;  // Rotación alrededor de Y
	float giroMetaKnight_z;  // Rotación alrededor de Z

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 100;			//El número de cuadros guardados actualmente desde 0 para no sobreescribir
bool play = false;
int playIndex = 0;

void saveFrame(void) // tecla L
{
	printf("FrameIndex %d\n", FrameIndex);

	// --- Snorlax ---

	// Cuerpo
	KeyFrame[FrameIndex].movCuerpoSnorlax_x = movCuerpoSnorlax_x;
	KeyFrame[FrameIndex].movCuerpoSnorlax_z = movCuerpoSnorlax_z;
	KeyFrame[FrameIndex].giroSnorlax = giroSnorlax;

	// Brazo Izquierdo
	KeyFrame[FrameIndex].giroBrazoIzquierdo_x = giroBrazoIzquierdo_x;
	KeyFrame[FrameIndex].giroBrazoIzquierdo_z = giroBrazoIzquierdo_z;
	KeyFrame[FrameIndex].movBrazoIzquierdo_x = movBrazoIzquierdo_x;
	KeyFrame[FrameIndex].movBrazoIzquierdo_z = movBrazoIzquierdo_z;

	// Brazo Derecho
	KeyFrame[FrameIndex].giroBrazoDerecho_x = giroBrazoDerecho_x;
	KeyFrame[FrameIndex].giroBrazoDerecho_z = giroBrazoDerecho_z;
	KeyFrame[FrameIndex].movBrazoDerecho_x = movBrazoDerecho_x;
	KeyFrame[FrameIndex].movBrazoDerecho_z = movBrazoDerecho_z;

	// Pierna Izquierda
	KeyFrame[FrameIndex].giroPiernaIzquierda_x = giroPiernaIzquierda_x;
	KeyFrame[FrameIndex].giroPiernaIzquierda_z = giroPiernaIzquierda_z;
	KeyFrame[FrameIndex].movPiernaIzquierda_x = movPiernaIzquierda_x;
	KeyFrame[FrameIndex].movPiernaIzquierda_z = movPiernaIzquierda_z;

	// Pierna Derecha
	KeyFrame[FrameIndex].giroPiernaDerecha_x = giroPiernaDerecha_x;
	KeyFrame[FrameIndex].giroPiernaDerecha_z = giroPiernaDerecha_z;
	KeyFrame[FrameIndex].movPiernaDerecha_x = movPiernaDerecha_x;
	KeyFrame[FrameIndex].movPiernaDerecha_z = movPiernaDerecha_z;


	// --- Meta Knight ---
	KeyFrame[FrameIndex].movMetaKnight_x = movMetaKnight_x;
	KeyFrame[FrameIndex].movMetaKnight_y = movMetaKnight_y;
	KeyFrame[FrameIndex].movMetaKnight_z = movMetaKnight_z;

	KeyFrame[FrameIndex].giroMetaKnight_x = giroMetaKnight_x;
	KeyFrame[FrameIndex].giroMetaKnight_y = giroMetaKnight_y;
	KeyFrame[FrameIndex].giroMetaKnight_z = giroMetaKnight_z;

	// Avanza al siguiente frame
	FrameIndex++;
}



void resetElements(void) // Tecla 0
{
	// --- Avión (si aún lo usas) ---
	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;

	// --- Snorlax ---

	// Cuerpo
	movCuerpoSnorlax_x = KeyFrame[0].movCuerpoSnorlax_x;
	movCuerpoSnorlax_z = KeyFrame[0].movCuerpoSnorlax_z;
	giroSnorlax = KeyFrame[0].giroSnorlax;

	// Brazo Izquierdo
	giroBrazoIzquierdo_x = KeyFrame[0].giroBrazoIzquierdo_x;
	giroBrazoIzquierdo_z = KeyFrame[0].giroBrazoIzquierdo_z;
	movBrazoIzquierdo_x = KeyFrame[0].movBrazoIzquierdo_x;
	movBrazoIzquierdo_z = KeyFrame[0].movBrazoIzquierdo_z;

	// Brazo Derecho
	giroBrazoDerecho_x = KeyFrame[0].giroBrazoDerecho_x;
	giroBrazoDerecho_z = KeyFrame[0].giroBrazoDerecho_z;
	movBrazoDerecho_x = KeyFrame[0].movBrazoDerecho_x;
	movBrazoDerecho_z = KeyFrame[0].movBrazoDerecho_z;

	// Pierna Izquierda
	giroPiernaIzquierda_x = KeyFrame[0].giroPiernaIzquierda_x;
	giroPiernaIzquierda_z = KeyFrame[0].giroPiernaIzquierda_z;
	movPiernaIzquierda_x = KeyFrame[0].movPiernaIzquierda_x;
	movPiernaIzquierda_z = KeyFrame[0].movPiernaIzquierda_z;

	// Pierna Derecha
	giroPiernaDerecha_x = KeyFrame[0].giroPiernaDerecha_x;
	giroPiernaDerecha_z = KeyFrame[0].giroPiernaDerecha_z;
	movPiernaDerecha_x = KeyFrame[0].movPiernaDerecha_x;
	movPiernaDerecha_z = KeyFrame[0].movPiernaDerecha_z;


	// --- Meta Knight ---
	movMetaKnight_x = KeyFrame[0].movMetaKnight_x;
	movMetaKnight_y = KeyFrame[0].movMetaKnight_y;
	movMetaKnight_z = KeyFrame[0].movMetaKnight_z;

	giroMetaKnight_x = KeyFrame[0].giroMetaKnight_x;
	giroMetaKnight_y = KeyFrame[0].giroMetaKnight_y;
	giroMetaKnight_z = KeyFrame[0].giroMetaKnight_z;

}

void interpolation(void)
{
	// --- Avión ---
	KeyFrame[playIndex].movAvion_xInc =
		(KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc =
		(KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc =
		(KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

	// --- Cuerpo Snorlax ---
	KeyFrame[playIndex].movCuerpoSnorlax_xInc =
		(KeyFrame[playIndex + 1].movCuerpoSnorlax_x - KeyFrame[playIndex].movCuerpoSnorlax_x) / i_max_steps;
	KeyFrame[playIndex].movCuerpoSnorlax_zInc =
		(KeyFrame[playIndex + 1].movCuerpoSnorlax_z - KeyFrame[playIndex].movCuerpoSnorlax_z) / i_max_steps;
	KeyFrame[playIndex].giroSnorlaxInc =
		(KeyFrame[playIndex + 1].giroSnorlax - KeyFrame[playIndex].giroSnorlax) / i_max_steps;

	// --- Brazo Izquierdo ---
	KeyFrame[playIndex].giroBrazoIzquierdo_xInc =
		(KeyFrame[playIndex + 1].giroBrazoIzquierdo_x - KeyFrame[playIndex].giroBrazoIzquierdo_x) / i_max_steps;
	KeyFrame[playIndex].giroBrazoIzquierdo_zInc =
		(KeyFrame[playIndex + 1].giroBrazoIzquierdo_z - KeyFrame[playIndex].giroBrazoIzquierdo_z) / i_max_steps;
	KeyFrame[playIndex].movBrazoIzquierdo_xInc =
		(KeyFrame[playIndex + 1].movBrazoIzquierdo_x - KeyFrame[playIndex].movBrazoIzquierdo_x) / i_max_steps;
	KeyFrame[playIndex].movBrazoIzquierdo_zInc =
		(KeyFrame[playIndex + 1].movBrazoIzquierdo_z - KeyFrame[playIndex].movBrazoIzquierdo_z) / i_max_steps;

	// --- Brazo Derecho ---
	KeyFrame[playIndex].giroBrazoDerecho_xInc =
		(KeyFrame[playIndex + 1].giroBrazoDerecho_x - KeyFrame[playIndex].giroBrazoDerecho_x) / i_max_steps;
	KeyFrame[playIndex].giroBrazoDerecho_zInc =
		(KeyFrame[playIndex + 1].giroBrazoDerecho_z - KeyFrame[playIndex].giroBrazoDerecho_z) / i_max_steps;
	KeyFrame[playIndex].movBrazoDerecho_xInc =
		(KeyFrame[playIndex + 1].movBrazoDerecho_x - KeyFrame[playIndex].movBrazoDerecho_x) / i_max_steps;
	KeyFrame[playIndex].movBrazoDerecho_zInc =
		(KeyFrame[playIndex + 1].movBrazoDerecho_z - KeyFrame[playIndex].movBrazoDerecho_z) / i_max_steps;

	// --- Pierna Izquierda ---
	KeyFrame[playIndex].giroPiernaIzquierda_xInc =
		(KeyFrame[playIndex + 1].giroPiernaIzquierda_x - KeyFrame[playIndex].giroPiernaIzquierda_x) / i_max_steps;
	KeyFrame[playIndex].giroPiernaIzquierda_zInc =
		(KeyFrame[playIndex + 1].giroPiernaIzquierda_z - KeyFrame[playIndex].giroPiernaIzquierda_z) / i_max_steps;
	KeyFrame[playIndex].movPiernaIzquierda_xInc =
		(KeyFrame[playIndex + 1].movPiernaIzquierda_x - KeyFrame[playIndex].movPiernaIzquierda_x) / i_max_steps;
	KeyFrame[playIndex].movPiernaIzquierda_zInc =
		(KeyFrame[playIndex + 1].movPiernaIzquierda_z - KeyFrame[playIndex].movPiernaIzquierda_z) / i_max_steps;

	// --- Pierna Derecha ---
	KeyFrame[playIndex].giroPiernaDerecha_xInc =
		(KeyFrame[playIndex + 1].giroPiernaDerecha_x - KeyFrame[playIndex].giroPiernaDerecha_x) / i_max_steps;
	KeyFrame[playIndex].giroPiernaDerecha_zInc =
		(KeyFrame[playIndex + 1].giroPiernaDerecha_z - KeyFrame[playIndex].giroPiernaDerecha_z) / i_max_steps;
	KeyFrame[playIndex].movPiernaDerecha_xInc =
		(KeyFrame[playIndex + 1].movPiernaDerecha_x - KeyFrame[playIndex].movPiernaDerecha_x) / i_max_steps;
	KeyFrame[playIndex].movPiernaDerecha_zInc =
		(KeyFrame[playIndex + 1].movPiernaDerecha_z - KeyFrame[playIndex].movPiernaDerecha_z) / i_max_steps;
}


void interpolationMetaKnight(int index)
{
	KeyFrame[index].movMetaKnight_xInc =
		(KeyFrame[index + 1].movMetaKnight_x - KeyFrame[index].movMetaKnight_x) / i_max_stepsMK;
	KeyFrame[index].movMetaKnight_yInc =
		(KeyFrame[index + 1].movMetaKnight_y - KeyFrame[index].movMetaKnight_y) / i_max_stepsMK;
	KeyFrame[index].movMetaKnight_zInc =
		(KeyFrame[index + 1].movMetaKnight_z - KeyFrame[index].movMetaKnight_z) / i_max_stepsMK;

	KeyFrame[index].giroMetaKnight_xInc =
		(KeyFrame[index + 1].giroMetaKnight_x - KeyFrame[index].giroMetaKnight_x) / i_max_stepsMK;
	KeyFrame[index].giroMetaKnight_yInc =
		(KeyFrame[index + 1].giroMetaKnight_y - KeyFrame[index].giroMetaKnight_y) / i_max_stepsMK;
	KeyFrame[index].giroMetaKnight_zInc =
		(KeyFrame[index + 1].giroMetaKnight_z - KeyFrame[index].giroMetaKnight_z) / i_max_stepsMK;
}



// ----------------------------------------------------
// Función para guardar todos los keyframes de Meta Knight a un archivo
// ----------------------------------------------------
void GuardarKeyframesMetaKnight(FRAME KeyFrame[], int FrameIndex, const std::string& nombreArchivo) {
	std::ofstream archivo(nombreArchivo, std::ios::app); // modo append

	if (!archivo.is_open()) {
		std::cerr << "Error al abrir el archivo para guardar los keyframes de Meta Knight.\n";
		return;
	}

	archivo << "---- NUEVA SESIÓN DE GUARDADO META KNIGHT ----\n";
	for (int i = 0; i < FrameIndex; ++i) {
		archivo << "Keyframe " << i << ":\n";

		// ------------------ Meta Knight ------------------
		archivo << "movMetaKnight_x: " << KeyFrame[i].movMetaKnight_x << "\n";
		archivo << "movMetaKnight_y: " << KeyFrame[i].movMetaKnight_y << "\n";
		archivo << "movMetaKnight_z: " << KeyFrame[i].movMetaKnight_z << "\n";

		archivo << "giroMetaKnight_x: " << KeyFrame[i].giroMetaKnight_x << "\n";
		archivo << "giroMetaKnight_y: " << KeyFrame[i].giroMetaKnight_y << "\n";
		archivo << "giroMetaKnight_z: " << KeyFrame[i].giroMetaKnight_z << "\n";

		archivo << "----------------------------\n";
	}

	archivo.close();
	std::cout << "Se guardaron " << FrameIndex << " keyframes de Meta Knight en " << nombreArchivo << "\n";
}


// ----------------------------------------------------
// Función para cargar keyframes de Meta Knight desde un archivo
// ----------------------------------------------------
void CargarKeyframesMetaKnight(FRAME KeyFrame[], int& FrameIndex, const std::string& nombreArchivo) {
	std::ifstream archivo(nombreArchivo);

	if (!archivo.is_open()) {
		std::cerr << "Error al abrir el archivo para cargar los keyframes de Meta Knight.\n";
		return;
	}

	std::string linea;
	int index = 0;

	while (std::getline(archivo, linea)) {
		std::stringstream ss(linea.substr(linea.find(":") + 1));

		// ------------------ Meta Knight ------------------
		if (linea.find("movMetaKnight_x:") != std::string::npos) ss >> KeyFrame[index].movMetaKnight_x;
		else if (linea.find("movMetaKnight_y:") != std::string::npos) ss >> KeyFrame[index].movMetaKnight_y;
		else if (linea.find("movMetaKnight_z:") != std::string::npos) ss >> KeyFrame[index].movMetaKnight_z;

		else if (linea.find("giroMetaKnight_x:") != std::string::npos) ss >> KeyFrame[index].giroMetaKnight_x;
		else if (linea.find("giroMetaKnight_y:") != std::string::npos) ss >> KeyFrame[index].giroMetaKnight_y;
		else if (linea.find("giroMetaKnight_z:") != std::string::npos) ss >> KeyFrame[index].giroMetaKnight_z;

		// ------------------ Fin de keyframe ------------------
		else if (linea.find("----------------------------") != std::string::npos) {
			index++;
			if (index >= MAX_FRAMES) break; // Evita desbordamiento
		}
	}

	FrameIndex = index;
	archivo.close();

	std::cout << "Se cargaron " << FrameIndex << " keyframes de Meta Knight desde " << nombreArchivo << "\n";
}



void animate(void)
{
	// Movimiento del Snorlax con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) // ¿Fin de la animación entre frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);

			if (playIndex > FrameIndex - 2) // ¿Fin de toda la animación con el último frame?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("Terminó la animación\n");
				playIndex = 0;
				play = false;
			}
			else // Interpolación del próximo cuadro
			{
				i_curr_steps = 0; // Resetea contador
				interpolation();  // Interpolar valores del siguiente frame
			}
		}
		else
		{
			// -----------------------------
			// Dibujar Animación de Snorlax
			// -----------------------------

			// --- Cuerpo ---
			movCuerpoSnorlax_x += KeyFrame[playIndex].movCuerpoSnorlax_xInc;
			movCuerpoSnorlax_z += KeyFrame[playIndex].movCuerpoSnorlax_zInc;
			giroSnorlax += KeyFrame[playIndex].giroSnorlaxInc;

			// --- Brazo Izquierdo ---
			giroBrazoIzquierdo_x += KeyFrame[playIndex].giroBrazoIzquierdo_xInc;
			giroBrazoIzquierdo_z += KeyFrame[playIndex].giroBrazoIzquierdo_zInc;
			movBrazoIzquierdo_x += KeyFrame[playIndex].movBrazoIzquierdo_xInc;
			movBrazoIzquierdo_z += KeyFrame[playIndex].movBrazoIzquierdo_zInc;

			// --- Brazo Derecho ---
			giroBrazoDerecho_x += KeyFrame[playIndex].giroBrazoDerecho_xInc;
			giroBrazoDerecho_z += KeyFrame[playIndex].giroBrazoDerecho_zInc;
			movBrazoDerecho_x += KeyFrame[playIndex].movBrazoDerecho_xInc;
			movBrazoDerecho_z += KeyFrame[playIndex].movBrazoDerecho_zInc;

			// --- Pierna Izquierda ---
			giroPiernaIzquierda_x += KeyFrame[playIndex].giroPiernaIzquierda_xInc;
			giroPiernaIzquierda_z += KeyFrame[playIndex].giroPiernaIzquierda_zInc;
			movPiernaIzquierda_x += KeyFrame[playIndex].movPiernaIzquierda_xInc;
			movPiernaIzquierda_z += KeyFrame[playIndex].movPiernaIzquierda_zInc;

			// --- Pierna Derecha ---
			giroPiernaDerecha_x += KeyFrame[playIndex].giroPiernaDerecha_xInc;
			giroPiernaDerecha_z += KeyFrame[playIndex].giroPiernaDerecha_zInc;
			movPiernaDerecha_x += KeyFrame[playIndex].movPiernaDerecha_xInc;
			movPiernaDerecha_z += KeyFrame[playIndex].movPiernaDerecha_zInc;


			// Avanza paso a paso dentro de la interpolación actual
			i_curr_steps++;
		}
	}
}


void animateMetaKnight(void)
{
	if (playMetaKnight)
	{
		
		if (i_curr_stepsMK >= i_max_stepsMK)
		{
			playIndexMK++;
			if (playIndexMK > FrameIndex - 2)
			{
				playIndexMK = 0;
				playMetaKnight = false;
			}
			else
			{
				i_curr_stepsMK = 0;
				interpolationMetaKnight(playIndexMK); // <- usar la nueva función
			}
		}

		else
		{
			// --- Movimiento lineal ---
			movMetaKnight_x += KeyFrame[playIndexMK].movMetaKnight_xInc;
			movMetaKnight_y += KeyFrame[playIndexMK].movMetaKnight_yInc;
			movMetaKnight_z += KeyFrame[playIndexMK].movMetaKnight_zInc;

			// --- Giro ---
			giroMetaKnight_x += KeyFrame[playIndexMK].giroMetaKnight_xInc;
			giroMetaKnight_y += KeyFrame[playIndexMK].giroMetaKnight_yInc;
			giroMetaKnight_z += KeyFrame[playIndexMK].giroMetaKnight_zInc;

			// Avanza paso a paso dentro de la interpolación actual
			i_curr_stepsMK++;
		}
	}
}


///////////////* FIN KEYFRAMES*////////////////////////////



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3 ( - 270.0f, 3.5f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), -6.0f, 0.0f, 0.5f, 0.5f);


	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pasto.png");
	pisoTexture.LoadTextureA();

	//Modelos snorlax
	CuerpoSnorlax_M = Model();
	CuerpoSnorlax_M.LoadModel("Models/cabezasnorlax.fbx");
	PiernaDerecha_M = Model();
	PiernaDerecha_M.LoadModel("Models/piederechosnorlax.fbx");
	PiernaIzquierda_M = Model();
	PiernaIzquierda_M.LoadModel("Models/pieizquierdosnorlax.fbx");
	BrazoIzquierdo_M = Model();
	BrazoIzquierdo_M.LoadModel("Models/brazoizquierdosnorlax.fbx");
	BrazoDerecho_M = Model();
	BrazoDerecho_M.LoadModel("Models/brazoderechosnorlax.fbx");


	//Modelos Braviary
	Braviary_M = Model();
	Braviary_M.LoadModel("Models/braviary.fbx");

	AlaDerechaB_M = Model();
	AlaDerechaB_M.LoadModel("Models/aladerechaB.fbx");

	AlaIzquierdaB_M = Model();
	AlaIzquierdaB_M.LoadModel("Models/alaizquierdaB.fbx");


	//Modelos Archen
	Archen_M = Model();
	Archen_M.LoadModel("Models/archen.fbx");

	AlaDerechaA_M = Model();
	AlaDerechaA_M.LoadModel("Models/aladerechaarchen.fbx");


	AlaIzquierdaA_M = Model();
	AlaIzquierdaA_M.LoadModel("Models/alaizquierdaarchen.fbx");

	//Jigglypuff}
	Jigglypuff_M = Model();
	Jigglypuff_M.LoadModel("Models/Jigglypuff.fbx");


	//Meta Knight
	MetaKnight_M = Model();
	MetaKnight_M.LoadModel("Models/Metacuerpo.fbx");

	BrazoDerechoMK_M = Model();
	BrazoDerechoMK_M.LoadModel("Models/MetabrazoDerecho.fbx");


	BrazoIzquierdoMK_M = Model();
	BrazoIzquierdoMK_M.LoadModel("Models/MetabrazoIzquierdo.fbx");

	piernaDerechaMK_M = Model();
	piernaDerechaMK_M.LoadModel("Models/Metapiederecho.fbx");

	piernaIzquierdaMK_M = Model();
	piernaIzquierdaMK_M.LoadModel("Models/Metapieizquierdo.fbx");

	AlaDerechaMK_M = Model();
	AlaDerechaMK_M.LoadModel("Models/Metaaladerecha.fbx");

	AlaIzquierdaMK_M = Model();
	AlaIzquierdaMK_M.LoadModel("Models/Metaalaizquierda.fbx");

	EspadaMK_M = Model();
	EspadaMK_M.LoadModel("Models/Metaespada.fbx");

	OjosMK_M = Model();
	OjosMK_M.LoadModel("Models/Metaojos.fbx");

	//Pokebola
	TapaPokebola_M = Model();
	TapaPokebola_M.LoadModel("Models/TapaPokebola.fbx");

	Eevee_M = Model();
	Eevee_M.LoadModel("Models/eevee.fbx");

	//CarroDede
	CarroDedede_M = Model();
	CarroDedede_M.LoadModel("Models/CarroD.fbx");


	//Magolor
	Magolor_M = Model();
	Magolor_M.LoadModel("Models/Magolor.fbx");

	//AVATAR WADDLE
	CuerpoWaddle_M = Model();
	CuerpoWaddle_M.LoadModel("Models/waddlecuerpo.fbx");

	BrazoWaddleD_M = Model();
	BrazoWaddleD_M.LoadModel("Models/waddlebrazod.fbx");

	BrazoWaddleI_M = Model();
	BrazoWaddleI_M.LoadModel("Models/waddlebrazoifbx.fbx");

	PiernaWaddleD_M = Model();
	PiernaWaddleD_M.LoadModel("Models/waddlepiernader.fbx");

	PiernaWaddleI_M = Model();
	PiernaWaddleI_M.LoadModel("Models/waddlepiernaizq.fbx");


	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/nave.fbx");


	//==========================SEGUNDA PARTE==========================================
	//Escenario
	Ring_M = Model();
	Ring_M.LoadModel("Models/RingCompleto.fbx");
	CentroPokemon_M = Model();
	CentroPokemon_M.LoadModel("Models/centropokemon.fbx");
	Arbol_M = Model();
	Arbol_M.LoadModel("Models/Whispy.fbx");
	EntradaPrehispanica_M = Model();
	EntradaPrehispanica_M.LoadModel("Models/EntradaPrehispanica.fbx");
	MuroPrehispanico_M = Model();
	MuroPrehispanico_M.LoadModel("Models/MuroPrehispanico.fbx");
	MuroPrehispanico2_M = Model();
	MuroPrehispanico2_M.LoadModel("Models/MuroPrehispanico2.fbx");
	Pikachu_M = Model();
	Pikachu_M.LoadModel("Models/Pikachu.fbx");
	Kirby_M = Model();
	Kirby_M.LoadModel("Models/kirby.fbx");
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/lampara.fbx");
	PokeLampara_M = Model();
	PokeLampara_M.LoadModel("Models/LamparaPokemon.fbx");
	Tribuna_M = Model();
	Tribuna_M.LoadModel("Models/Tribuna.fbx");
	Anotorcha_M = Model();
	Anotorcha_M.LoadModel("Models/Antorcha.fbx");
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/lampara.fbx");
	Litwick_M = Model();
	Litwick_M.LoadModel("Models/litwick.fbx");
	VigasLuz_M = Model();
	VigasLuz_M.LoadModel("Models/VigasLuz.fbx");
	BolaLuz_M = Model();
	BolaLuz_M.LoadModel("Models/BolaLuz.fbx");
	Anuncio_M = Model();
	Anuncio_M.LoadModel("Models/anuncio.fbx");
	Camino_M = Model();
	Camino_M.LoadModel("Models/Camino.fbx");
	Policia_M = Model();
	Policia_M.LoadModel("Models/Policia.fbx");
	BoteBasuraVacio_M = Model();
	BoteBasuraVacio_M.LoadModel("Models/BoteBasuraVacio.fbx");
	ContenedoresEcologicos_M = Model();
	ContenedoresEcologicos_M.LoadModel("Models/ContenedoresEcologicos.fbx");
	MesaSombrilla_M = Model();
	MesaSombrilla_M.LoadModel("Models/MesaSombrilla.fbx");
	Banca_M = Model();
	Banca_M.LoadModel("Models/Banca.fbx");
	PuestoTacos_M = Model();
	PuestoTacos_M.LoadModel("Models/PuestoTacos.fbx");
	PuestoPapas_M = Model();
	PuestoPapas_M.LoadModel("Models/PuestoPapas.fbx");
	Estatuas_M = Model();
	Estatuas_M.LoadModel("Models/estatua.fbx");
	Trailer_M = Model();
	Trailer_M.LoadModel("Models/Trailer.fbx");


	// Skybox 1
	std::vector<std::string> skyboxFaces1;
	skyboxFaces1.push_back("Textures/Skybox/central2.png");
	skyboxFaces1.push_back("Textures/Skybox/izquierda.png");
	skyboxFaces1.push_back("Textures/Skybox/abajo.png");
	skyboxFaces1.push_back("Textures/Skybox/arriba.png");
	skyboxFaces1.push_back("Textures/Skybox/central1.png");
	skyboxFaces1.push_back("Textures/Skybox/derecha.png");
	Skybox skybox1(skyboxFaces1);

	// Skybox 2
	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/central2oscuro.png");
	skyboxFaces2.push_back("Textures/Skybox/izquierdaoscuro.png");
	skyboxFaces2.push_back("Textures/Skybox/abajo.png");
	skyboxFaces2.push_back("Textures/Skybox/arribaoscuro.png");
	skyboxFaces2.push_back("Textures/Skybox/central1osucuro.png");
	skyboxFaces2.push_back("Textures/Skybox/derechaoscuro.png");
	Skybox skybox2(skyboxFaces2);



	float lastSwitchTime = 0.0f; // Tiempo del último cambio
	float switchInterval = 95.0f; // Intervalo en segundos
	bool useFirstSkybox = true;  // Flag para saber qué skybox usar

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	// ================== POINT LIGHTS ==================
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	// 0) Poke lámpara — blanca
	pointLights[0] = PointLight(
		1.0f, 1.0f, 1.0f,     // color
		0.1f, 1.0f,           // ambient, diffuse
		0.0f, 4.0f, 0.0f,    // pos (ajusta si quieres)
		0.15f, 0.004f, 0.0003f   // atenuación (const, lin, cuad)
	);
	pointLightCount++;


	// 1) Centro Ring — blanca
	pointLights[1] = PointLight(
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.1f, 0.004f, 0.003f //0.1f, 0.0004f, 0.003f
	);
	pointLightCount++;

	// 2) Lidwick — morada
	pointLights[2] = PointLight(
		0.70f, 0.40f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.15f, 0.004f, 0.0003f
	);
	pointLightCount++;

	// 3) Antorcha — naranja
	pointLights[3] = PointLight(
		1.0f, 0.5f, 0.1f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.15f, 0.004f, 0.0003f
	);
	pointLightCount++;

	// ================== SPOT LIGHTS ==================
	unsigned int spotLightCount = 0;

	// 0) Linterna — Spotlight blanca
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	// 1) Letrero — Spotlight blanca
	spotLights[1] = SpotLight(
		1.0f, 1.0f, 1.0f,     // color
		1.0f, 2.0f,           // ambient, diffuse
		0.0f, 0.0f, 0.0f,    // pos
		0.0f, 0.0f, -1.0f,    // dir
		1.0f, 0.0f, 0.0f,     // atenuación (const, lin, cuad)
		95.0f                 // edge
	);
	spotLightCount++;

	// 2) Lámpara — Spotlight blanca
	spotLights[2] = SpotLight(
		1.0f, 1.0f, 1.0f,     // color
		1.0f, 2.0f,           // ambient, diffuse
		0.0f, 0.0f, 0.0f,    // pos
		0.0f, 0.0f, -1.0f,    // dir
		1.0f, 0.0f, 0.0f,     // atenuación (const, lin, cuad)
		45.0f                 // edge
	);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	




	// ---------------- KEYFRAMES SNORLAX CAMINANDO Y GIRANDO ----------------
// ============================================================
// === FRAME 2: final del paso +X y empieza giro hacia -Z ===
// ============================================================

// --- CUERPO ---
	KeyFrame[2].movCuerpoSnorlax_x = 4.0f;
	KeyFrame[2].movCuerpoSnorlax_z = 0.0f;
	KeyFrame[2].giroSnorlax = 45.0f; // empieza el giro

	// --- BRAZOS ---
	KeyFrame[2].giroBrazoIzquierdo_z = 45.0f;
	KeyFrame[2].giroBrazoIzquierdo_x = 45.0f;
	KeyFrame[2].movBrazoIzquierdo_x = 0.0f;
	KeyFrame[2].movBrazoIzquierdo_z = 0.0f;
	KeyFrame[2].giroBrazoDerecho_x = 45.0f;
	KeyFrame[2].giroBrazoDerecho_z = -45.0f;
	KeyFrame[2].movBrazoDerecho_x = 0.0f;
	KeyFrame[2].movBrazoDerecho_z = 0.0f;

	// --- PIERNAS ---
	KeyFrame[2].giroPiernaIzquierda_x = 45.0f;
	KeyFrame[2].giroPiernaIzquierda_z = -20.0f;
	KeyFrame[2].movPiernaIzquierda_x = 0.0f;
	KeyFrame[2].movPiernaIzquierda_z = 0.0f;
	KeyFrame[2].giroPiernaDerecha_x = 45.0f;
	KeyFrame[2].giroPiernaDerecha_z = 20.0f;
	KeyFrame[2].movPiernaDerecha_x = 0.0f;
	KeyFrame[2].movPiernaDerecha_z = 0.0f;

	// ====================================================
	// === FRAME 3: termina el giro, mira hacia -Z ===
	// ====================================================

	// --- CUERPO ---
	KeyFrame[3].movCuerpoSnorlax_x = 4.0f;
	KeyFrame[3].movCuerpoSnorlax_z = 0.5f;
	KeyFrame[3].giroSnorlax = 90.0f;

	// --- BRAZOS ---
	KeyFrame[3].giroBrazoIzquierdo_z = -45.0f;
	KeyFrame[3].giroBrazoDerecho_z = 45.0f;

	// --- PIERNAS ---
	KeyFrame[3].giroPiernaIzquierda_z = 45.0f;
	KeyFrame[3].giroPiernaDerecha_z = -45.0f;

	// =======================================
	// === FRAME 4: camina hacia -Z ===
	// =======================================

	// --- CUERPO ---
	KeyFrame[4].movCuerpoSnorlax_x = 4.0f;
	KeyFrame[4].movCuerpoSnorlax_z = 10.0f;
	KeyFrame[4].giroSnorlax = 90.0f;

	// --- BRAZOS ---
	KeyFrame[4].giroBrazoIzquierdo_z = 45.0f;
	KeyFrame[4].giroBrazoDerecho_z = -45.0f;

	// --- PIERNAS ---
	KeyFrame[4].giroPiernaIzquierda_z = -20.0f;
	KeyFrame[4].giroPiernaDerecha_z = 20.0f;

	// =======================================
	// === FRAME 5: sigue avanzando hacia -Z ===
	// =======================================

	// --- CUERPO ---
	KeyFrame[5].movCuerpoSnorlax_x = 4.0f;
	KeyFrame[5].movCuerpoSnorlax_z = 20.0f;
	KeyFrame[5].giroSnorlax = 90.0f;

	// --- BRAZOS ---
	KeyFrame[5].giroBrazoIzquierdo_z = -45.0f;
	KeyFrame[5].giroBrazoDerecho_z = 45.0f;

	// --- PIERNAS ---
	KeyFrame[5].giroPiernaIzquierda_z = 20.0f;
	KeyFrame[5].giroPiernaDerecha_z = -20.0f;

	// =======================================
	// === FRAME 6: último paso hacia -Z ===
	// =======================================

	// --- CUERPO ---
	KeyFrame[6].movCuerpoSnorlax_x = 4.0f;
	KeyFrame[6].movCuerpoSnorlax_z = 30.0f;
	KeyFrame[6].giroSnorlax = 90.0f;

	// --- BRAZOS ---
	KeyFrame[6].giroBrazoIzquierdo_z = 45.0f;
	KeyFrame[6].giroBrazoDerecho_z = -45.0f;

	// --- PIERNAS
	KeyFrame[6].giroPiernaIzquierda_z = -20.0f;
	KeyFrame[6].giroPiernaDerecha_z = 20.0f;

	// =======================================
	// === FRAME 7 al 26: avance hacia -Z de 10 en 10 ===
	// =======================================

	for (int i = 7; i <= 26; i++) {
		KeyFrame[i] = KeyFrame[6]; // Copia base del frame 6
		KeyFrame[i].movCuerpoSnorlax_z = KeyFrame[6].movCuerpoSnorlax_z + 10.0f * (i - 6);
		if (i % 2 == 0) {
			KeyFrame[i].giroBrazoIzquierdo_z = 45.0f;
			KeyFrame[i].giroBrazoDerecho_z = -45.0f;
			KeyFrame[i].giroPiernaIzquierda_z = -20.0f;
			KeyFrame[i].giroPiernaDerecha_z = 20.0f;
		}
		else {
			KeyFrame[i].giroBrazoIzquierdo_z = -45.0f;
			KeyFrame[i].giroBrazoDerecho_z = 45.0f;
			KeyFrame[i].giroPiernaIzquierda_z = 20.0f;
			KeyFrame[i].giroPiernaDerecha_z = -20.0f;
		}
	}

	


	//Se agregan nuevos frames 


	//------------------FRAMES NAVE META KNIGHTS-----------------------

	


		printf("\nTeclas para uso de Keyframes:\n1.-Presionar barra espaciadora para reproducir animacion snorlax.\n2.-Presionar 0 para volver a habilitar reproduccion de la animacion nave y snorlax\n");
		printf("3.-Presiona L para guardar frame\n4.-Presiona P para habilitar guardar nuevo frame\n5.-Presiona 1 para cambiar a camara weedle dee\n6.-Presiona 2 para cambiar a camara aerea\n");
		printf("7.-Presiona C para cargar animacion keyframe MetaKnight \n8.-Presiona V para habilitar volver a cargar \n9.-Presiona F para abrir y cerrar pokebola\n10.-Presiona h para mover coche y regresa\n");
		printf("11.-Presiona J para hacer volar a magollor \n12.-Presiona 7  para mover en -y\n13.-Presiona 8 para volver a habilitar mover en -y\n");
		printf("14.-Presiona M ver una nave girar \n15.-Presiona N para habilitar la rotación \n16-. Presionar C para Cargar txt pasado \n17-. Presionar V para habilitar Cargar txt pasado  ");



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		glm::vec3 lowerLight = glm::vec3(0.0f,0.0f,0.0f);

	////Loop mientras no se cierra la ventana
		while (!mainWindow.getShouldClose())
		{
			GLfloat now = glfwGetTime();
			deltaTime = now - lastTime;
			deltaTime += (now - lastTime) / limitFPS;
			lastTime = now;

			angulovaria += 8.0f * deltaTime;


			//dragonavance

			anguloAlaBraviary += 8.0f * deltaTime;
			float aleteoBraviary = sin(glm::radians(anguloAlaBraviary)) * 30; // 30° es la amplitud del movimiento

			float aleteoArchen = sin(glm::radians(anguloAlaBraviary)) * 60;
			// BANDERA DE AVANCE Y REGRESO

			if (avanzabraviary) {
				if (braviaryavance > -250.0f) {
					braviaryavance -= movOffsetbraviary * deltaTime;
				}
				else {
					avanzabraviary = false;
					rotacionBraviary = 180.0f; // Girar hacia atrás
				}
			}
			else {
				if (braviaryavance < 325.0f) {
					braviaryavance += movOffsetbraviary * deltaTime;
				}
				else {
					avanzabraviary = true;
					rotacionBraviary = 0.0f; // Volver al frente
				}
			}






			if (movCoche < 30.0f)
			{
				movCoche -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			rotllanta += rotllantaOffset * deltaTime;



			//Recibir eventos del usuario
			glfwPollEvents();
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());



			glm::mat4 view;

			if (camaraTerceraPersonaActiva)
			{
				camera.followTargetThirdPerson(posicionWaddle, rotacionWaddle, 20.0f, 4.0f, 10.0f, deltaTime);
				view = camera.calculateViewMatrix();
			}
			else if (camaraAereaActiva)
			{
				view = camera.calculateViewMatrix(); // solo aquí se usa camaraAerea
			}
			else
			{
				camera.keyControl(mainWindow.getsKeys(), deltaTime);
				camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
				view = camera.calculateViewMatrix();
			}

			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));



			//-------Para Keyframes
			inputKeyframes(mainWindow.getsKeys());
			animate();
			animateMetaKnight();




			// Clear the window
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/*
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
			shaderList[0].UseShader();
			*/

			float currentTime = glfwGetTime(); // Tiempo en segundos desde que comenzó el programa

			if (currentTime - lastSwitchTime >= switchInterval) {
				useFirstSkybox = !useFirstSkybox; // Cambiar skybox
				lastSwitchTime = currentTime;     // Resetear tiempo
			}

			// Dibujar skybox activo
			if (useFirstSkybox)
				skybox1.DrawSkybox(camera.calculateViewMatrix(), projection);
			else
				skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);

			shaderList[0].UseShader(); // Volver a activar el shader de objetos


			uniformModel = shaderList[0].GetModelLocation();
			uniformProjection = shaderList[0].GetProjectionLocation();
			uniformView = shaderList[0].GetViewLocation();
			uniformEyePosition = shaderList[0].GetEyePositionLocation();
			uniformColor = shaderList[0].getColorLocation();
			uniformTextureOffset = shaderList[0].getOffsetLocation();

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
			//shaderList[0].SetDirectionalLight(&mainLight);
			//shaderList[0].SetPointLights(pointLights, pointLightCount);
			//shaderList[0].SetSpotLights(spotLights, spotLightCount);

			model = glm::mat4(1.0);
			modelaux = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			toffset = glm::vec2(0.0f, 0.0f);

			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			pisoTexture.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[2]->RenderMesh();


			// ----- SNORLAX --------- 

			// --- Cuerpo Snorlax ---
			glm::mat4 modelSnorlax = glm::mat4(1.0f);
			modelSnorlax = glm::translate(modelSnorlax, glm::vec3(-230.0f + movCuerpoSnorlax_x, 9.0f, 0.0f + movCuerpoSnorlax_z));
			modelSnorlax = glm::scale(modelSnorlax, glm::vec3(3.5f, 3.5f, 3.5f)); // escala uniforme
			modelSnorlax = glm::rotate(modelSnorlax, giroSnorlax * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			modelSnorlax = glm::rotate(modelSnorlax, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			modelSnorlax = glm::rotate(modelSnorlax, -90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelSnorlax));
			CuerpoSnorlax_M.RenderModel();

			// --- Pierna derecha ---
			glm::mat4 modelPiernaDerecha = modelSnorlax;   //x lo mueve bien, snorlax mira hacia z
			modelPiernaDerecha = glm::translate(modelPiernaDerecha, glm::vec3(1.2f + movPiernaDerecha_x, -0.0f + movPiernaDerecha_z, -2.0f ));
			modelPiernaDerecha = glm::rotate(modelPiernaDerecha, giroPiernaDerecha_x * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			modelPiernaDerecha = glm::rotate(modelPiernaDerecha, giroPiernaDerecha_z * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			modelPiernaDerecha = glm::rotate(modelPiernaDerecha, -90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaDerecha));
			PiernaDerecha_M.RenderModel();

			//Z es y 
			// --- Pierna izquierda ---
			glm::mat4 modelPiernaIzquierda = modelSnorlax;
			modelPiernaIzquierda = glm::translate(modelPiernaIzquierda, glm::vec3(-1.2f + movPiernaIzquierda_x, -0.0f + movPiernaIzquierda_z, -2.0f ));
			modelPiernaIzquierda = glm::rotate(modelPiernaIzquierda, giroPiernaIzquierda_x * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			modelPiernaIzquierda = glm::rotate(modelPiernaIzquierda, giroPiernaIzquierda_z * toRadians, glm::vec3(1.0f,0.0f, 0.0f));
			modelPiernaIzquierda = glm::rotate(modelPiernaIzquierda, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			//modelPiernaIzquierda = glm::rotate(modelPiernaIzquierda, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaIzquierda));
			PiernaIzquierda_M.RenderModel();

			// --- Brazo izquierdo ---
			glm::mat4 modelBrazoIzquierdo = modelSnorlax;
			modelBrazoIzquierdo = glm::translate(modelBrazoIzquierdo, glm::vec3(1.7f + movBrazoIzquierdo_x, 0.0f + movBrazoIzquierdo_z, 1.2f ));
			modelBrazoIzquierdo = glm::rotate(modelBrazoIzquierdo, giroBrazoIzquierdo_x * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			modelBrazoIzquierdo = glm::rotate(modelBrazoIzquierdo, giroBrazoIzquierdo_z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			//modelBrazoIzquierdo = glm::rotate(modelBrazoIzquierdo, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			//modelBrazoIzquierdo = glm::rotate(modelBrazoIzquierdo, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoIzquierdo));
			BrazoIzquierdo_M.RenderModel();

			// --- Brazo derecho ---
			glm::mat4 modelBrazoDerecho = modelSnorlax;
			modelBrazoDerecho = glm::translate(modelBrazoDerecho, glm::vec3(-1.7f + movBrazoDerecho_x, 0.0f + movBrazoDerecho_z , 1.0f));
			modelBrazoDerecho = glm::rotate(modelBrazoDerecho, giroBrazoDerecho_x * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			modelBrazoDerecho = glm::rotate(modelBrazoDerecho, giroBrazoDerecho_z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			//modelBrazoDerecho = glm::rotate(modelBrazoDerecho, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			//modelBrazoDerecho = glm::rotate(modelBrazoDerecho, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoDerecho));
			BrazoDerecho_M.RenderModel();



			//-----------------------------------------




			//-----BRAVIARY -----

			// Matriz base del Braviary
			glm::mat4 modelBraviary = glm::mat4(1.0f);
			modelBraviary = glm::translate(modelBraviary, glm::vec3(1.05f - braviaryavance, 90.0f + sin(glm::radians(angulovaria)), 6.0f));
			modelBraviary = glm::scale(modelBraviary, glm::vec3(1.0f, 1.0f, 1.0f));
			modelBraviary = glm::rotate(modelBraviary, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			modelBraviary = glm::rotate(modelBraviary, glm::radians(rotacionBraviary), glm::vec3(0.0f, 0.0f, 1.0f)); // <<--- AQUÍ
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBraviary));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Braviary_M.RenderModel();

			//AlaDerecha
			glm::mat4 modelAlaDerechaB = modelBraviary;
			modelAlaDerechaB = glm::translate(modelAlaDerechaB, glm::vec3(6.5f, -3.0f, 9.5f));
			modelAlaDerechaB = glm::rotate(modelAlaDerechaB, glm::radians(-aleteoBraviary), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAlaDerechaB));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			AlaDerechaB_M.RenderModel();

			//Alaizquierdda
			glm::mat4 modelAlaIzquierdaB = modelBraviary; // también hereda del cuerpo
			modelAlaIzquierdaB = glm::translate(modelAlaIzquierdaB, glm::vec3(6.5f, 3.0f, 9.5f));
			modelAlaIzquierdaB = glm::rotate(modelAlaIzquierdaB, glm::radians(aleteoBraviary), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAlaIzquierdaB));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			AlaIzquierdaB_M.RenderModel();




			//-----Archen ----

			glm::mat4 modelArchen = glm::mat4(1.0f);
			modelArchen = glm::translate(modelArchen, glm::vec3(1.05f - braviaryavance, 90.0f + sin(glm::radians(angulovaria)), 70.0f));
			modelArchen = glm::scale(modelArchen, glm::vec3(0.1f, 0.1f, 0.1f));
			modelArchen = glm::rotate(modelArchen, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			modelArchen = glm::rotate(modelArchen, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			modelArchen = glm::rotate(modelArchen, glm::radians(rotacionBraviary), glm::vec3(0.0f, 0.0f, 1.0f)); // <<--- AQUÍ
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelArchen));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Archen_M.RenderModel();

			//AlaDerecha
			glm::mat4 modelAlaDerechaA = modelArchen;
			modelAlaDerechaA = glm::translate(modelAlaDerechaA, glm::vec3(-5.5f, 5.0f, 25.0f));
			modelAlaDerechaA = glm::rotate(modelAlaDerechaA, glm::radians(-aleteoArchen), glm::vec3(1.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAlaDerechaA));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			AlaDerechaA_M.RenderModel();

			//Alaizquierdda
			glm::mat4 modelAlaIzquierdaA = modelArchen; // también hereda del cuerpo
			modelAlaIzquierdaA = glm::translate(modelAlaIzquierdaA, glm::vec3(5.5f, 5.0f, 25.0f));
			modelAlaIzquierdaA = glm::rotate(modelAlaIzquierdaA, glm::radians(aleteoArchen), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAlaIzquierdaA));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			AlaIzquierdaA_M.RenderModel();



			//Nave Metaknight
			model = glm::mat4(1.0);
			posblackhawk = glm::vec3(45.0f + movMetaKnight_x, 500.0f + movMetaKnight_y, 16.0f + movMetaKnight_z);
			model = glm::translate(model, posblackhawk);
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			model = glm::rotate(model, giroMetaKnight_x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, giroMetaKnight_y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, giroMetaKnight_z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Blackhawk_M.RenderModel();


			//Animación Jigglypuff
			float time = glfwGetTime();

			// Escala base y cuánto se infla
			float scaleBase = 0.1f;
			float scaleAmplitude = 0.2f; // más inflado

			// Escala oscilante, siempre >= scaleBase
			float scale = scaleBase + scaleAmplitude * (sin(time * 3.0f) * 0.5f + 0.5f);

			// Flotación vertical opcional
			float y = 0.2f + 0.02f * sin(time * 3.0f);

			// Jigglypuff
			glm::mat4 modelJigglypuff = glm::mat4(1.0f);
			modelJigglypuff = glm::translate(modelJigglypuff, glm::vec3(15.0f, 5.5f + y, 160.0f));
			modelJigglypuff = glm::rotate(modelJigglypuff, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			modelJigglypuff = glm::scale(modelJigglypuff, glm::vec3(scale, scale, scale));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelJigglypuff));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Jigglypuff_M.RenderModel();




			//MetaKnight

		   // ----- META KNIGHT -----

		   // Animación de Meta Knight
			float timeMK = glfwGetTime();

			// Movimiento vertical suave (planeo)
			float alturaMK = 10.0f + 5.5f * sin(timeMK * 2.0f);

			// Aleteo con función seno
			float aleteoMK = 30.0f * sin(timeMK * 5.0f); // velocidad de batido

			// Movimiento de brazos (ligero vaivén)
			float movimientoBrazos = 30.0f * sin(timeMK * 2.5f);

			// Movimiento de piernas (coordinado con brazos)
			float movimientoPiernas = -30.0f * sin(timeMK * 2.5f);

			// Giro de espada
			float rotacionEspada = 15.0f * sin(timeMK * 4.0f);


			// ----- META KNIGHT -----
			glm::mat4 modelMetaKnight = glm::mat4(1.0f);
			modelMetaKnight = glm::translate(modelMetaKnight, glm::vec3(150.0f, alturaMK, 200.0f)); // movimiento vertical animado
			modelMetaKnight = glm::scale(modelMetaKnight, glm::vec3(0.4f, 0.4f, 0.4f));
			modelMetaKnight = glm::rotate(modelMetaKnight, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMetaKnight));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			MetaKnight_M.RenderModel();

			// ----- Brazo Derecho -----
			glm::mat4 modelBrazoDerechoMK = modelMetaKnight;
			modelBrazoDerechoMK = glm::translate(modelBrazoDerechoMK, glm::vec3(-0.5f, 0.5f, 0.0f));
			modelBrazoDerechoMK = glm::rotate(modelBrazoDerechoMK, glm::radians(movimientoBrazos), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoDerechoMK));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			BrazoDerechoMK_M.RenderModel();

			// ----- Brazo Izquierdo -----
			glm::mat4 modelBrazoIzquierdoMK = modelMetaKnight;
			modelBrazoIzquierdoMK = glm::translate(modelBrazoIzquierdoMK, glm::vec3(0.5f, 0.5f, 0.0f));
			modelBrazoIzquierdoMK = glm::rotate(modelBrazoIzquierdoMK, glm::radians(-movimientoBrazos), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoIzquierdoMK));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			BrazoIzquierdoMK_M.RenderModel();

			// ----- Pierna Derecha -----
			glm::mat4 modelPiernaDerechaMK = modelMetaKnight;
			modelPiernaDerechaMK = glm::translate(modelPiernaDerechaMK, glm::vec3(-0.7f, -0.5f, 0.0f));
			modelPiernaDerechaMK = glm::rotate(modelPiernaDerechaMK, glm::radians(movimientoPiernas), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaDerechaMK));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			piernaDerechaMK_M.RenderModel();

			// ----- Pierna Izquierda -----
			glm::mat4 modelPiernaIzquierdaMK = modelMetaKnight;
			modelPiernaIzquierdaMK = glm::translate(modelPiernaIzquierdaMK, glm::vec3(0.7f, -0.5f, 0.0f));
			modelPiernaIzquierdaMK = glm::rotate(modelPiernaIzquierdaMK, glm::radians(-movimientoPiernas), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaIzquierdaMK));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			piernaIzquierdaMK_M.RenderModel();

			// ----- Ala Derecha -----
			glm::mat4 modelAlaDerechaMK = modelMetaKnight;
			modelAlaDerechaMK = glm::translate(modelAlaDerechaMK, glm::vec3(-2.0f, 8.0f, -2.0f));
			modelAlaDerechaMK = glm::rotate(modelAlaDerechaMK, glm::radians(aleteoMK), glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAlaDerechaMK));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			AlaDerechaMK_M.RenderModel();

			// ----- Ala Izquierda -----
			glm::mat4 modelAlaIzquierdaMK = modelMetaKnight;
			modelAlaIzquierdaMK = glm::translate(modelAlaIzquierdaMK, glm::vec3(2.0f, 8.0f, -2.0f));
			modelAlaIzquierdaMK = glm::rotate(modelAlaIzquierdaMK, glm::radians(-aleteoMK), glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAlaIzquierdaMK));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			AlaIzquierdaMK_M.RenderModel();

			// ----- Espada -----
			glm::mat4 modelEspadaMK = modelBrazoDerechoMK;
			modelEspadaMK = glm::translate(modelEspadaMK, glm::vec3(-9.0f, 5.0f, 0.0f));
			modelEspadaMK = glm::rotate(modelEspadaMK, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			modelEspadaMK = glm::rotate(modelEspadaMK, glm::radians(rotacionEspada), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEspadaMK));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			EspadaMK_M.RenderModel();

			// ----- Ojos -----
			glm::mat4 modelOjosMK = modelMetaKnight;
			modelOjosMK = glm::translate(modelOjosMK, glm::vec3(0.0f, 5.0f, 0.0f)); // posición relativa al cuerpo (ajústala según el modelo)
			modelOjosMK = glm::rotate(modelOjosMK, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			modelOjosMK = glm::rotate(modelOjosMK, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelOjosMK));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			OjosMK_M.RenderModel();


			if (pokebolaAbierta)
			{
				if (anguloTapa > -30.0f)
					anguloTapa -= incrementoTapa;
			}
			else
			{
				if (anguloTapa < 0.0f)
					anguloTapa += incrementoTapa;
			}


			//Tapa Pokebola

			

			glm::mat4 modelEevee = glm::mat4(1.0f);
			modelEevee = glm::translate(modelEevee, glm::vec3(170.0f, -5.0f, 130.0f));  // posición global
			modelEevee = glm::scale(modelEevee, glm::vec3(0.05f, 0.05f, 0.05f));                      // escala general
			modelEevee = glm::rotate(modelEevee, -90 * toRadians , glm::vec3(0.0f, 1.0f, 0.0f)); // rotación en eje Y
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEevee));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Eevee_M.RenderModel();

			glm::mat4 modelTapaPokebola = modelEevee; // hereda transformaciones del Eevee
			modelTapaPokebola = glm::translate(modelTapaPokebola, glm::vec3(10.0f, 260.0f, -215.0f));  // posición relativa debajo de Eevee
			modelTapaPokebola = glm::rotate(modelTapaPokebola, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // apertura animada
			modelTapaPokebola = glm::rotate(modelTapaPokebola, glm::radians(anguloTapa), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTapaPokebola));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			TapaPokebola_M.RenderModel();


			//Carro Dedede
			// --- Movimiento del carro ---
			if (carroAvanzando)
			{
				// Se mueve hacia adelante mientras no sobrepase el límite
				if (desplazamientoCarro < 80.0f)
					desplazamientoCarro += incrementoCarro;
			}
			else
			{
				// Se mueve hacia atrás mientras no vuelva al punto inicial
				if (desplazamientoCarro > -80.0f)
					desplazamientoCarro -= incrementoCarro;
			}

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(50.0 + desplazamientoCarro, 0.5, -200.0f ));  // posición global
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));                      // escala general
			model= glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f,0.0f)); // apertura animada
			model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // apertura animada
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			CarroDedede_M.RenderModel();

			
			if (magolorEnMovimiento)
			{
				if (magolorSubiendo)
				{
					desplazamientoMagolor += incrementoMagolor;
					if (desplazamientoMagolor >= 30.0f)  // límite superior
					{
						desplazamientoMagolor = 30.0f;
						magolorSubiendo = false; // cambia dirección
					}
				}
				else
				{
					desplazamientoMagolor -= incrementoMagolor;
					if (desplazamientoMagolor <= 0.0f)  // límite inferior
					{
						desplazamientoMagolor = 0.0f;
						magolorEnMovimiento = false; // detiene el ciclo
					}
				}
			}

			//Magolor
			glm::mat4 modelMagolor = glm::mat4(1.0f);
			modelMagolor = glm::translate(modelMagolor, glm::vec3(-30.0, 3.5f + desplazamientoMagolor, -50.0));  // posición global
			modelMagolor = glm::scale(modelMagolor, glm::vec3(4.5f, 4.5f, 4.5f));                      // escala general
			//modelMagolor = glm::rotate(modelMagolor, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // apertura animada
			//modelMagolor = glm::rotate(modelMagolor, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // apertura animada
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMagolor));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Magolor_M.RenderModel();


			//Waddle Dee
			
			// ----- WADDLE DEE -----
			glm::mat4 modelWaddle = glm::mat4(1.0f);
			modelWaddle = glm::translate(modelWaddle, posicionWaddle);
			modelWaddle = glm::rotate(modelWaddle, glm::radians(rotacionWaddle), glm::vec3(0.0f, 1.0f, 0.0f));
			modelWaddle = glm::rotate(modelWaddle, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	
			modelWaddle = glm::rotate(modelWaddle, glm::radians(inclinacionCuerpoZ), glm::vec3(0.0f, 0.0f, 1.0f)); // 🔹 oscilación
			modelWaddle = glm::scale(modelWaddle, glm::vec3(escalaWaddle));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelWaddle));
			CuerpoWaddle_M.RenderModel();

			// ----- Brazo Derecho -----
			glm::mat4 modelBrazoDerechoWaddle = modelWaddle;
			modelBrazoDerechoWaddle = glm::translate(modelBrazoDerechoWaddle, glm::vec3(-1.2f, 0.0f, 0.0f));
			modelBrazoDerechoWaddle = glm::rotate(modelBrazoDerechoWaddle, glm::radians(anguloBrazos), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoDerechoWaddle));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			BrazoWaddleD_M.RenderModel();

			// ----- Brazo Izquierdo -----
			glm::mat4 modelBrazoIzquierdoWaddle = modelWaddle;
			modelBrazoIzquierdoWaddle = glm::translate(modelBrazoIzquierdoWaddle, glm::vec3(1.2f, 0.0f, 0.0f));
			modelBrazoIzquierdoWaddle = glm::rotate(modelBrazoIzquierdoWaddle, glm::radians(-anguloBrazos), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoIzquierdoWaddle));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			BrazoWaddleI_M.RenderModel();

			// ----- Pierna Derecha -----
			glm::mat4 piernaD = modelWaddle;
			piernaD = glm::translate(piernaD, glm::vec3(-0.5f, 0.0f, -1.0f));
			piernaD = glm::rotate(piernaD, glm::radians(anguloPiernas), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(piernaD));
			PiernaWaddleD_M.RenderModel();

			// ----- Pierna Izquierda -----
			glm::mat4 piernaI = modelWaddle;
			piernaI = glm::translate(piernaI, glm::vec3(0.5f, 0.0f, -1.0f));
			piernaI = glm::rotate(piernaI, glm::radians(-anguloPiernas), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(piernaI));
			PiernaWaddleI_M.RenderModel();

			

			//===============================================ESCENARIO=================================================
//----- RING -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(100.0f, 0.01f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Ring_M.RenderModel();


			//----- ARBOL -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-135.0f, 0.01f, 140.0f));
			model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -225 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol_M.RenderModel();

			//----- CENTRO POKEMON -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-115.0f, 0.01f, -130.0f));
			model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			CentroPokemon_M.RenderModel();

			//-----ENTRADA PREHISPANICA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-180.0f, 0.01f, 0.0f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			EntradaPrehispanica_M.RenderModel();


			//----- PIKACHU -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(105.0f, 15.0f, 5.0f));
			model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -225 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Pikachu_M.RenderModel();


			//----- KIRBY -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(85.0f, 20.0f, -15.0f));
			model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Kirby_M.RenderModel();


			//----- TRIBUNA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(150.0f, 0.01f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Tribuna_M.RenderModel();


			
			//----- TRAILER -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(90.0f, 0.01f, -140.0f));
			model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Trailer_M.RenderModel();


			//----- POLICIA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-130.0f, 0.01f, 20.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
			model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(x,z,-y)
			//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Policia_M.RenderModel();

			//----- BOTE BASURA VACIO -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(30.0f, 0.01f, -18.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			BoteBasuraVacio_M.RenderModel();


			//----- CONTENEDORES ECOLOGICOS -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(30.0f, 0.01f, 20.0f));
			model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ContenedoresEcologicos_M.RenderModel();

			//----- MESA SOMBRILLA 1 -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.01f, 50.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MesaSombrilla_M.RenderModel();

			//----- MESA SOMBRILLA 1 -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-60.0f, 0.01f, 50.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MesaSombrilla_M.RenderModel();

			//----- MESA SOMBRILLA 2 -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-120.0f, 0.01f, 50.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MesaSombrilla_M.RenderModel();

			//----- MESA SOMBRILLA 4 -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.01f, -60.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MesaSombrilla_M.RenderModel();

			//----- MESA SOMBRILLA 5 -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-120.0f, 0.01f, -60.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MesaSombrilla_M.RenderModel();

			//----- MESA SOMBRILLA 6 -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.01f, -140.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MesaSombrilla_M.RenderModel();

			

			//----- ESTATUAS -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-60.0f, 0.01f, 150.0f));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Estatuas_M.RenderModel();

		
			//----- BANCA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-140.0f, 0.01f, -25.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Banca_M.RenderModel();

			//----- BANCA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-105.0f, 0.01f, -25.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Banca_M.RenderModel();

			//----- BANCA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-70.0f, 0.01f, -25.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Banca_M.RenderModel();

			//----- BANCA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-35.0f, 0.01f, -25.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Banca_M.RenderModel();

			//----- BANCA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.01f, -25.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Banca_M.RenderModel();


			//----- Puesto Tacos -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(70.0f, 0.01f, 130.0f));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -225 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			PuestoTacos_M.RenderModel();

			//----- Puesto Papas -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-40.0f, 0.01f, -140.0f));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			PuestoPapas_M.RenderModel();


			
			//---- CAMINO -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(250.0f, 0.2f, 0.3f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //(z,y,x)
			modelaux = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.094f));
			//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();


			//----- LAMPARA POKEMON -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(30.0f, 0.01f, -100.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			PokeLampara_M.RenderModel();

			// JERARQUÍA (PointLight 0)
			glm::vec3 posLuzPokeLocal = glm::vec3(0.0f, 0.0f, 4.0f); // altura sobre la lámpara
			glm::vec3 posLuzPokeGlobal = glm::vec3(model * glm::vec4(posLuzPokeLocal, 1.0f));
			pointLights[0].SetPos(posLuzPokeGlobal); // SetPos agregado en PointLight.h

			//----- VIGAS LUZ -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(100.0f, 0.01f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			VigasLuz_M.RenderModel();

			//----- BOLA LUZ ----- JERARQUIA (Lampara de centro de ring)
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.8f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			BolaLuz_M.RenderModel();

			// JERARQUÍA (PointLight 1)
			glm::vec3 posLuzRingLocal = glm::vec3(0.0f, 0.0f, -4.0f); // centro de la bola
			glm::vec3 posLuzRingGlobal = glm::vec3(model * glm::vec4(posLuzRingLocal, 1.0f));
			pointLights[1].SetPos(posLuzRingGlobal);

			//----- LITWICK -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-60.0f, 0.01f, -60.0f));
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Litwick_M.RenderModel();

			// JERARQUÍA (PointLight 2)
			glm::vec3 posLuzLidLocal = glm::vec3(0.0f, 0.8f, 0.0f); // un poco encima de la vela
			glm::vec3 posLuzLidGlobal = glm::vec3(model * glm::vec4(posLuzLidLocal, 1.0f));
			pointLights[2].SetPos(posLuzLidGlobal);

			//----- ANTORCHA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(20.0f, 0.01f, 100.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Anotorcha_M.RenderModel();

			// JERARQUÍA (PointLight 3)
			glm::vec3 posLuzAntLocal = glm::vec3(0.0f, 3.0f, 0.0f); // sobre la llama
			glm::vec3 posLuzAntGlobal = glm::vec3(model * glm::vec4(posLuzAntLocal, 1.0f));
			pointLights[3].SetPos(posLuzAntGlobal);

			//shaderList[0].SetPointLights(pointLights, pointLightCount);

			//---- ANUNCIO PELEA (LETRERO )-----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(130.0f, 0.01f, 140.0f));
			model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Anuncio_M.RenderModel();

			// JERARQUÍA (SpotLight 1)
			glm::vec3 posLetrLocal = glm::vec3(0.0f, 0.0f, 0.0f);   // en el centro del letrero
			glm::vec3 dirLetrLocal = glm::vec3(0.0f, -1.0f, 0.0f);  // hacia -Z local (ajusta si quieres)

			glm::vec3 posLetrGlobal = glm::vec3(model * glm::vec4(posLetrLocal, 1.0f));
			glm::vec3 dirLetrGlobal = glm::normalize(glm::vec3(model * glm::vec4(dirLetrLocal, 0.0f)));
			spotLights[1].SetFlash(posLetrGlobal, dirLetrGlobal);

			//----- LAMPARA -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-60.0f, 0.01f, 20.0f));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			//model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Lampara_M.RenderModel();

			// JERARQUÍA (SpotLight 2)
			glm::vec3 posLampLocal = glm::vec3(0.0f, 5.0f, 5.0f);   // origen de la lámpara
			glm::vec3 dirLampLocal = glm::vec3(0.0f, -1.0f, 0.0f);  // hacia abajo en espacio local

			glm::vec3 posLampGlobal = glm::vec3(model * glm::vec4(posLampLocal, 1.0f));
			glm::vec3 dirLampGlobal = glm::normalize(glm::vec3(model * glm::vec4(dirLampLocal, 0.0f)));
			spotLights[2].SetFlash(posLampGlobal, dirLampGlobal);

			// ======== SPOTLIGHTS ACTIVAS (Z = letrero, X = lámpara) =========
			SpotLight spotsActivas[3];
			int spotsActivos = 0;

			spotsActivas[spotsActivos++] = spotLights[0];

			// Letrero -> Z
			if (mainWindow.getLuzSpotLetrero()) {
				spotsActivas[spotsActivos++] = spotLights[1];
			}

			// Lámpara -> X
			if (mainWindow.getLuzSpotLampara()) {
				spotsActivas[spotsActivos++] = spotLights[2];
			}

			// ÚNICA llamada: solo mando las que estén ON
			//shaderList[0].SetSpotLights(spotsActivas, spotsActivos);



			// ====== DÍA / NOCHE según skybox ======
			bool esDia = useFirstSkybox;  // skybox1 = día, skybox2 = noche

			// Direccional: ON de día, OFF de noche
			if (esDia) {
				shaderList[0].SetDirectionalLight(&mainLight);
			}
			else {
				// apago direccional con intensidades 0 (sin tocar tu mainLight creada)
				DirectionalLight offDir(1.0f, 1.0f, 1.0f,
					0.0f, 0.0f,
					0.0f, 0.0f, -1.0f);
				shaderList[0].SetDirectionalLight(&offDir);
			}

			// Puntuales: OFF de día, ON de noche
			PointLight ptsActivas[MAX_POINT_LIGHTS];
			int ptsActivos = 0;

			if (!esDia) {
				for (int i = 0; i < pointLightCount; ++i) {
					ptsActivas[ptsActivos++] = pointLights[i];
				}
			}

			// === ÚNICAS subidas este frame ===
			shaderList[0].SetPointLights(ptsActivas, ptsActivos);
			shaderList[0].SetSpotLights(spotsActivas, spotsActivos);





			//---- MUROS 1 CON JERARQUIA -----
			//-----MURO PREHISPANIC0 ----- 
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-180.0f, 0.01f, 65.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 2 -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			
			//----- muro 3 -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel(); 

			//----- muro 4 puede que se quite-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 5 vuelta -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(25.0f, -26.0f, 0.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 6-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 7-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();
			
			//----- muro 8-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 9-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 10-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 11-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 12-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 13 vuelta -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(25.0f, -26.0f, 0.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 14-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();
			
			//----- muro 15-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 16-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();


			//---- MUROS 2 CON JERARQUIA -----
			//-----MURO PREHISPANIC0 ----- 
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-180.0f, 0.01f, -65.5f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //(x,z,-y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 2-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();
			
			//----- muro 3-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 4 puede que se quite -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 5 vuelta -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(25.0f, 25.5f, 0.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 6-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();
			
			//----- muro 7-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 8-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 9-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 10-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 11-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 12-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 13 vuelta -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(25.0f, 25.5f, 0.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico_M.RenderModel();

			//----- muro 14-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -51.75f, 0.0f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();
			
			//----- muro 15-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();

			//----- muro 16-----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 51.75f, 0.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			MuroPrehispanico2_M.RenderModel();
			









			//---- CAMINO -----
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-170.0f, 0.2f, 0.3f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //(z,y,x)
			modelaux = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.094f));
			//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //(z,x, y)
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();

			//---- camino 2 -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -51.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.094f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();

			//---- camino 3 -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -51.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.094f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();

			//---- camino 4 -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -51.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.094f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();

			//---- camino 5 -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -51.0f));
			modelaux = model;
			modelaux2 = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.094f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();

			//---- camino 6.1 -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(42.3f, 0.0f, -12.5f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //(x,z,-y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.11f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();

			//---- camino 7.1 -----
			model = modelaux;
			model = glm::translate(model, glm::vec3(-51.4f, 0.0f, -17.4f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //(x,z,-y)
			modelaux = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.14f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();

			//---- camino 6.2 -----
			model = modelaux2;
			model = glm::translate(model, glm::vec3(-40.3f, 0.0f, -12.5f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //(x,z,-y)
			modelaux2 = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.11f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();

			//---- camino 7.2 -----
			model = modelaux2;
			model = glm::translate(model, glm::vec3(-51.4f, 0.0f, 19.4f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //(x,z,-y)
			modelaux2 = model;
			model = glm::scale(model, glm::vec3(0.094f, 0.094f, 0.14f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Camino_M.RenderModel();

			
			glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}



void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				//resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}


	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1 && reproduciranimacion>0)
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora'\n");
			reproduciranimacion = 0;
			
		}
	}


	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}


	
	if (keys[GLFW_KEY_P] )
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			guardoArchivo = 0;
			reinicioFrame++;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}

	// --- CARGAR ARCHIVOS PASADOS ---

// --- CARGAR ARCHIVOS PASADOS ---
	if (keys[GLFW_KEY_C])
	{
		if (cicloCarga < 1)
		{
			CargarKeyframesMetaKnight(KeyFrame, FrameIndex, "keyframes_guardados.txt");
			cicloCarga++;
			banderaCarga = 0; // permite volver a usar la tecla V una vez

			printf("Reproduciendo keyframes cargados...\n");

			// --- Reproducir todos los keyframes cargados ---
			for (int i = 0; i < FrameIndex; i++)
			{
				printf("\n--- Keyframe %d ---\n", i + 1);
				printf("movMetaKnight_x: %.2f\n", KeyFrame[i].movMetaKnight_x);
				printf("movMetaKnight_y: %.2f\n", KeyFrame[i].movMetaKnight_y);
				printf("movMetaKnight_z: %.2f\n", KeyFrame[i].movMetaKnight_z);
				printf("giroMetaKnight_x: %.2f\n", KeyFrame[i].giroMetaKnight_x);
				printf("giroMetaKnight_y: %.2f\n", KeyFrame[i].giroMetaKnight_y);
				printf("giroMetaKnight_z: %.2f\n", KeyFrame[i].giroMetaKnight_z);

				// Si quieres que se vea en pantalla cada frame, llama a tu función de interpolación o render
				// interpolacionEntreFrames(KeyFrame[i], i);

				std::this_thread::sleep_for(std::chrono::milliseconds(150)); // pausa para visualizar en consola
			}

			printf("\nReproducción completa de los keyframes.\n");
		}
	}
	

	// --- HABILITAR NUEVA CARGA ---
	if (keys[GLFW_KEY_V])
	{
		if (banderaCarga < 1)
		{
			cicloCarga = 0;     // resetea para poder volver a cargar
			banderaCarga++;     // evita que se imprima infinitamente
			printf("Ya puedes volver a cargar animaciones con C.\n");
		}
	}

	if (keys[GLFW_KEY_M])
	{
		if (!playMetaKnight && (FrameIndex > 1))
		{
			// Reinicia los elementos de Meta Knight
			resetElements(); // opcional

			// Primera interpolación específica para Meta Knight
			interpolationMetaKnight(playIndexMK);  

			playMetaKnight = true;
			playIndexMK = 0;
			i_curr_stepsMK = 0;
		}
	}


	if (keys[GLFW_KEY_G])
	{
		if (guardoArchivo < 1) {
			guardoArchivo++;
			GuardarKeyframesMetaKnight(KeyFrame, FrameIndex, "keyframes_guardados.txt");
			printf("Keyframes agregados al archivo. Presiona P para poder guardar nuevamente.\n");

		}

	}

	if (keys[GLFW_KEY_F] && GLFW_PRESS)
	{
		pokebolaAbierta = !pokebolaAbierta; // alterna entre abierta y cerrada
	}


	if (keys[GLFW_KEY_H] && GLFW_PRESS)
	{
		carroAvanzando = !carroAvanzando; 
	}


	if (keys[GLFW_KEY_J] )
	{
		magolorEnMovimiento = true;  
		magolorSubiendo = true;      
	}

	
	// --- Camara avatar ---
	if (keys[GLFW_KEY_1] )
	{
		camaraTerceraPersonaActiva = !camaraTerceraPersonaActiva;
		camaraAereaActiva = false;

		if (camaraTerceraPersonaActiva)
			printf("Cámara en tercera persona ACTIVADA\n");
		else
			printf("Cámara libre ACTIVADA\n");
	}

	if (camaraTerceraPersonaActiva)
	{
		float delta = deltaTime * velocidadWaddle;
		bool caminando = false;

		// ----- Movimiento -----
		if (keys[GLFW_KEY_W])
		{
			posicionWaddle.x += sin(glm::radians(rotacionWaddle)) * delta;
			posicionWaddle.z += cos(glm::radians(rotacionWaddle)) * delta;
			caminando = true;
		}
		if (keys[GLFW_KEY_S])
		{
			posicionWaddle.x -= sin(glm::radians(rotacionWaddle)) * delta;
			posicionWaddle.z -= cos(glm::radians(rotacionWaddle)) * delta;
			caminando = true;
		}

		// ----- Giro suave -----
		if (keys[GLFW_KEY_A])
			rotacionWaddle += velocidadGiro * deltaTime;
		if (keys[GLFW_KEY_D])
			rotacionWaddle -= velocidadGiro * deltaTime;

		// ====== Animación del cuerpo ======
		if (caminando)
		{
			
			float factorAnim = velocidadWaddle * 0.3f;

			// Brazos
			if (sentidoBrazo)
			{
				anguloBrazos += factorAnim * deltaTime;
				if (anguloBrazos > 45.0f)
					sentidoBrazo = false;
			}
			else
			{
				anguloBrazos -= factorAnim * deltaTime;
				if (anguloBrazos < -45.0f)
					sentidoBrazo = true;
			}

			// Piernas
			if (sentidoPierna)
			{
				anguloPiernas += factorAnim * deltaTime;
				if (anguloPiernas > 45.0f)
					sentidoPierna = false;
			}
			else
			{
				anguloPiernas -= factorAnim * deltaTime;
				if (anguloPiernas < -45.0f)
					sentidoPierna = true;
			}

			// Inclinación del cuerpo en Z (de -20° a +20°)
			if (sentidoCuerpo)
			{
				inclinacionCuerpoZ += (factorAnim * velocidadWaddle * 40.0f) * deltaTime;
				if (inclinacionCuerpoZ > 45.0f)
					sentidoCuerpo = false;
			}
			else
			{
				inclinacionCuerpoZ -= (factorAnim * 40.0f) * deltaTime;
				if (inclinacionCuerpoZ < -45.0f)
					sentidoCuerpo = true;
			}
		}
		else
		{
			// Regresa a posición neutral
			anguloBrazos *= 0.9f;
			anguloPiernas *= 0.9f;
			inclinacionCuerpoZ *= 0.9f;
		}
	}




	// --- Cámara aérea ---

	if (keys[GLFW_KEY_2] && GLFW_PRESS )
	{
		camaraAereaActiva = !camaraAereaActiva;
		camaraTerceraPersonaActiva = false;

		if (camaraAereaActiva)
		{
			camera.setCameraPosition(glm::vec3(0.0f,400.0f, 0.0f));
			camera.setYaw(-90.0f);
			camera.setPitch(-60.0f);

			
		}
		else
		{
			// Cuando se desactiva, colocar la cámara libre en otra posición
			camera.setCameraPosition(glm::vec3(0.0f, 100.0f, 50.0f)); // posición deseada
			camera.setYaw(-6.0f);   // yaw original de la cámara libre
			camera.setPitch(0.0f);  // pitch original de la cámara libre


		}
	}


	if (camaraAereaActiva)
	{
		float delta = deltaTime * 3.0f;
		glm::vec3 pos = camera.getCameraPosition();

		if (keys[GLFW_KEY_W]) pos.z -= delta;
		if (keys[GLFW_KEY_S]) pos.z += delta;
		if (keys[GLFW_KEY_A]) pos.x -= delta;
		if (keys[GLFW_KEY_D]) pos.x += delta;
		if (keys[GLFW_KEY_Q]) pos.y += delta;
		if (keys[GLFW_KEY_E]) pos.y -= delta;

		if (pos.y < 10.0f) pos.y = 10.0f;
		if (pos.y > 10000.f) pos.y = 10000.0f;

		camera.setCameraPosition(pos);

		camera.setYaw(-90.0f);
		camera.setPitch(-60.0f);
	}



}
