#include <GL/glut.h>


#include <math.h>
#include <time.h>



typedef struct
{
	float r;
	float g;
	float b;
}Color;

typedef struct
{
	float x;
	float y;
	float z;
}Vector;

typedef struct
{
	float r;
	Vector c;
	Color color;
	int id;
	float ri;
	int refleccion;
}Esfera;

typedef struct
{
	Vector direccion;
	Vector origen;
}Rayo;

typedef struct
{
	Esfera* esfera;
	Vector n;
	Vector p;
	float t;
}Choque;

//Funciones para el uso de las estructuras 

Color setColor(float r,float g, float b){
	Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

Color sumaColor(Color color1, Color color2){
	return (setColor(color1.r+color2.r,color1.g+color2.g,color1.b+color2.b));
}

Color aumentoColor(Color color, float aumento){
	return (setColor(color.r * aumento,color.g * aumento,color.b * aumento));
}

Vector setVector(float x,float y, float z){
	Vector vector;
	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}

float magnitud(Vector vector){
    return sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
}

float pPunto(Vector vector1, Vector vector2){
	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);
}

Vector pCruz(Vector vector1,Vector vector2){
	return setVector(vector1.y*vector2.z - vector1.z*vector2.y, vector1.z*vector2.x
	 - vector1.x*vector2.z, vector1.x*vector2.y - vector1.y*vector2.x);
}

Vector escalaVector(Vector vector, float escala){
	return setVector(vector.x * escala, vector.y * escala, vector.z * escala);
}

Vector sumaVector(Vector vector1, Vector vector2){
	return setVector(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z);
}

Vector restaVector(Vector vector1, Vector vector2){
	return sumaVector(vector1,escalaVector(vector2,-1));
}

void setColorPixel(Color pixel, Color* pixeles){
	pixeles->r = pixel.r/255;
	pixeles->g = pixel.g/255;
	pixeles->b = pixel.b/255;
}

GLboolean sombra(Rayo rayo);
GLboolean refraccion(Vector vectorInA, Vector vectorInB, float valor, Vector *vector);


float calculoInterseccion(Rayo rayo, Esfera esfera);
float choqueEscena(Rayo rayo, Choque* choque);

Rayo analisisRayoVision(float i, float j, Vector origen);
Rayo analisisRayoVisionNormal(float i, float j);
Rayo calculoSombra(Vector vector, Vector iluminacion);

Vector refleccion(Vector direccion, Vector normal);

Color blur(Vector vector,Color color, int numluces);
Color atenuacion(float kr, float kg, float kb, Color color);
Color ambiente(Color color);
Color choqueRayo(Rayo rayo, int valor);
Color AntiAlias(int antiX, int antiY);
Color shade(Choque choque,Rayo rayo, int valor);
Color mejoraColor(Rayo rayo, Vector vectorIn, int numLuz);
