#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>



class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	bool getLuzlampara() const { return lamparaEncendida; }

	//PuertaIzquierda
	bool getPuertaAbierta() const { return puertaAbierta; }
	void setPuertaAbierta(bool estado) { puertaAbierta = estado; }
	float getAnguloPuertaI() const { return anguloPuertaI; }
	void setAnguloPuertaI(float angulo) { anguloPuertaI = angulo; }
	bool getPuertaMoviendose() const { return puertaMoviendose; }
	void setPuertaMoviendose(bool estado) { puertaMoviendose = estado; }

	//PuertaDerecha
	bool getPuertaDerechaMoviendose() const { return puertaDerechaMoviendose; }
	void setPuertaDerechaMoviendose(bool estado) { puertaDerechaMoviendose = estado; }
	float getPuertaD_X() const { return posPuertaD_X; }
	void setPuertaD_X(float angulo) { posPuertaD_X = angulo; }
	float getPuertaD_Y() const { return posPuertaD_Y; }
	void setPuertaD_Y(float angulo) { posPuertaD_Y = angulo; }
	float getPuertaD_Z() const { return posPuertaD_Z; }
	void setPuertaD_Z(float angulo) { posPuertaD_Z = angulo; }
	bool getPuertaDerechaAbierta() const { return puertaDerechaAbierta; }
	void setPuertaDerechaAbierta(bool estado) { puertaDerechaAbierta = estado; }

	//Dado
	float getGiraDado() const { return giraDado; }
	void setGiraDado(float angulo) { giraDado = angulo; }

	float getCaeDado() const { return caeDado; }
	void setCaeDado(float valor) { caeDado = valor; }

	bool getDadoGirando() const { return dadoGirando; }
	void setDadoGirando(bool estado) { dadoGirando = estado; }

	bool getDadoCayendo() const { return dadoCayendo; }
	void setDadoCayendo(bool estado) { dadoCayendo = estado; }

	bool getDadoAterrizado() const { return dadoAterrizado; }
	void setDadoAterrizado(bool estado) { dadoAterrizado = estado; }

	int getDadoNumero() const { return dadoNumero; }
	void setDadoNumero(int num) { dadoNumero = num; }





	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz,articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6, Setactual;
	bool keys[1024];
	bool puertaAbierta;
	bool puertaMoviendose;
	bool puertaDerechaMoviendose;
	bool puertaDerechaAbierta;
	float anguloPuertaI;
	float posPuertaD_X;
	float posPuertaD_Y ;
	float posPuertaD_Z;
	float giraDado;      // ángulo de giro en grados
	float caeDado ;       // desplazamiento vertical
	bool dadoGirando ;   // indica si el dado está girando
	bool dadoCayendo ;// indica si el dado está en caída
	bool dadoAterrizado;
	int dadoNumero;
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	float giroY = 0.0f;           // ángulo actual de giro Y
	bool lamparaEncendida;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
	

};

