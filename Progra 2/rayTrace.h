#include <GL/glut.h>

#include <math.h>
#include <time.h>

struct BMPHeader
{
    char bfType[2];       /* "BM" */
    int bfSize;           /* Size of file in bytes */
    int bfReserved;       /* set to 0 */
    int bfOffBits;        /* Byte offset to actual bitmap data (= 54) */
    int biSize;           /* Size of BITMAPINFOHEADER, in bytes (= 40) */
    int biWidth;          /* Width of image, in pixels */
    int biHeight;         /* Height of images, in pixels */
    short biPlanes;       /* Number of planes in target device (set to 1) */
    short biBitCount;     /* Bits per pixel (24 in this case) */
    int biCompression;    /* Type of compression (0 if no compression) */
    int biSizeImage;      /* Image size, in bytes (0 if no compression) */
    int biXPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biYPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biClrUsed;        /* Number of colors in the color table (if 0, use 
                             maximum allowed by biBitCount) */
    int biClrImportant;   /* Number of important colors.  If 0, all colors 
                             are important */
};

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
	if(pixeles->r > 1) {
		pixeles->r = 1.0;
	}
	pixeles->g = pixel.g/255;
	if(pixeles->g > 1) {
		pixeles->g = 1.0;
	}
	pixeles->b = pixel.b/255;
	if(pixeles->b > 1) {
		pixeles->b = 1.0;
	}

	
}

Color shade(Choque choque,Rayo rayo, int valor);
