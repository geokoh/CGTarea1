/* Tarea Programada #2 
 * Autor: Mauricio Castillo Vega
 * Curso: Computer Graphics 2-2018
 * Profesor: Dr. Francisco Torres
 */

#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "rayTrace.h"

unsigned int ancho = 1008;
unsigned int alto = 567;
unsigned int cantColores = 3;
unsigned int numeroEsferas = 20;

//Variables globales y de funcionamiento
float pixeles[567*1008*3];

Esfera esferas[20];

Vector e;
float d = 10;

Vector w;
Vector u;
Vector v;


float izq = -8.064, der = 8.064;
float abajo = -4.536, arriba = 4.536;

Vector luces[3];
float luz = 0.7;
int numeroLuces = 3;
float intensidadAmbiente = 0.2;

Color colorFondo;

bool llaveAntiAlias = false, llaveRefleccion = false;
bool llaveTransparencia = false, llaveProfundidad = false;

// Inicio de las funciones de trabajo


int crearBMP(const char *filename, int ancho, int alto, float *rgb)
{
    int i, j, ipos;
    int bytesPorLinea;
    char *linea;

    FILE *file;
    struct BMPHeader bmph;

    /* The length of each line must be a multiple of 4 bytes */

    bytesPorLinea = (3 * (ancho + 1) / 4) * 4;

    strcpy(bmph.bfType, "BM");
    bmph.bfOffBits = 54;
    bmph.bfSize = bmph.bfOffBits + bytesPorLinea * alto;
    bmph.bfReserved = 0;
    bmph.biSize = 40;
    bmph.biWidth = ancho;
    bmph.biHeight = alto;
    bmph.biPlanes = 1;
    bmph.biBitCount = 24;
    bmph.biCompression = 0;
    bmph.biSizeImage = bytesPorLinea * alto;
    bmph.biXPelsPerMeter = 0;
    bmph.biYPelsPerMeter = 0;
    bmph.biClrUsed = 0;       
    bmph.biClrImportant = 0; 

    file = fopen (filename, "wb");
    if (file == NULL) return(0);
  
    fwrite(&bmph.bfType, 2, 1, file);
    fwrite(&bmph.bfSize, 4, 1, file);
    fwrite(&bmph.bfReserved, 4, 1, file);
    fwrite(&bmph.bfOffBits, 4, 1, file);
    fwrite(&bmph.biSize, 4, 1, file);
    fwrite(&bmph.biWidth, 4, 1, file);
    fwrite(&bmph.biHeight, 4, 1, file);
    fwrite(&bmph.biPlanes, 2, 1, file);
    fwrite(&bmph.biBitCount, 2, 1, file);
    fwrite(&bmph.biCompression, 4, 1, file);
    fwrite(&bmph.biSizeImage, 4, 1, file);
    fwrite(&bmph.biXPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biYPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biClrUsed, 4, 1, file);
    fwrite(&bmph.biClrImportant, 4, 1, file);
	
	//printf("bytesPorLinea %ld\n", sizeof(bytesPorLinea) );

    linea = (unsigned char*)malloc(bytesPorLinea);

    //printf("linea %ld \n", sizeof(linea));
    if (linea == NULL)
    {
        fprintf(stderr, "Can't allocate memory for BMP file.\n");
        return(0);
    }

    //(j*ancho*3) + (i*3)
    printf("ciclo for\n");
    for (i = 0; i < ancho-1; i++)
    {
        for (j = 0; j < alto-1; j++)
        {
            ipos = 3*(j*ancho+i);
            printf("pos: %d, i: %d, j: %d\n",ipos,i,j);
           	
            linea[3*i] = (char)1; //(char) rgb[ipos]*255;
            linea[3*i+1] = (char)1; //(char)rgb[ipos+1]*255;
            linea[3*i+2] = (char)1; //(char)rgb[ipos+2]*255;
            printf("r:%f,g:%f, b:%f\n", rgb[ipos]*255,rgb[ipos+1]*255,rgb[ipos+2]*255);
            printf("linea r:%d,g:%d,b:%d\n", linea[3*i],linea[3*i+1],linea[3*i+2]);
        }
        fwrite(linea, bytesPorLinea, 1, file);
    }

    free(linea);
    fclose(file);
    return 1;
}

int write_truecolor_tga() {
    FILE *fp = fopen("out.tga", "w");
    if (fp == NULL) return 0;

// The image header
    char header[18] = {0}; // char = byte
    header[2] = 2; // truecolor
    header[12] = ancho & 0xFF;
    header[13] = (ancho >> 8) & 0xFF;
    header[14] = alto & 0xFF;
    header[15] = (alto >> 8) & 0xFF;
    header[16] = 24; // bits per pixel

    fwrite((const char *) &header, 1, sizeof(header), fp);
	int ipos;
// The image data is stored bottom-to-top, left-to-right
    for (int y = 0; y < alto; y++)
        for (int x = 0; x < ancho; x++) {
        	ipos = (3*y*ancho) +(x*3); //
            if (ipos == 0) {
                printf("r: %f g: %f b: %f\n",pixeles[ipos], pixeles[ipos+1], pixeles[ipos+2] );
            }
            fputc((int) (pixeles[ipos+2] * 255), fp);
            fputc((int) (pixeles[ipos+1] * 255), fp);
            fputc((int) (pixeles[ipos+0] * 255), fp);
        }

// The file footer
    static const char footer[26] =
            "\0\0\0\0" // no extension area
            "\0\0\0\0" // no developer directory
            "TRUEVISION-XFILE" // yep, this is a TGA file
            ".";
    fwrite((const char *) &footer, 1, sizeof(footer), fp);

    fclose(fp);
    return 1;
}

void inicioVectores() {

    colorFondo = setColor(0, 0, 0);


    e = setVector(10, 0, -15);

    Vector vecSuperior = setVector(0, 0, 1);
    Vector vistaDireccion = setVector(-1, 0, 2);
    w = escalaVector(vistaDireccion,-1/magnitud(vistaDireccion));
    Vector vecPCruz = pCruz(vecSuperior, w);
    u = escalaVector(vecPCruz,1/magnitud(vecPCruz));
    v = pCruz(w, u);


    luces[0] = setVector(0,-1,-1);
    luces[0] = escalaVector(luces[0],1/magnitud(luces[0]));

    luces[1] = setVector(1,-1,1);
    luces[1] = escalaVector(luces[1],1/magnitud(luces[1]));

    luces[2] = setVector(-1,1,1);
    luces[2] = escalaVector(luces[2],1/magnitud(luces[2]));

    esferas[0].r = 6;
    esferas[0].c = setVector(-5, 0, 0);
    esferas[0].color = setColor(100,100,100);
    esferas[0].id = 3;
    esferas[0].ri = 1;
    esferas[0].refleccion = 1;

    esferas[1].r = 0.5;
    esferas[1].c = setVector(0.75, 0, 3.2);
    esferas[1].color = setColor(235,220,178);
    esferas[1].id = 8;
    esferas[1].ri = 1;
    esferas[1].refleccion = 1;

    esferas[2].r = 0.5;
    esferas[2].c = setVector(0.75, -1.2, 2.6 );
    esferas[2].color = setColor(235,220,178);
    esferas[2].id = 8;
    esferas[2].ri = 1;
    esferas[2].refleccion = 1; 

    esferas[3].r = 0.5;
    esferas[3].c = setVector(0.75, 1.2, 2.6 );
    esferas[3].color = setColor(235,220,178);
    esferas[3].id = 8;
    esferas[3].ri = 1;
    esferas[3].refleccion = 1; 

    esferas[4].r = 0.5;
    esferas[4].c = setVector(0.75, 2.4, 1.6);
    esferas[4].color =  setColor(0, 69, 122);
    esferas[4].id = 6;
    esferas[4].ri = 1;
    esferas[4].refleccion = 1;

    esferas[5].r = 0.5;
    esferas[5].c = setVector(0.75, -2.4, 1.6);
    esferas[5].color = setColor(0, 69, 122);
    esferas[5].id = 7;
    esferas[5].ri = 1;
    esferas[5].refleccion = 1;

    esferas[6].r = 0.5;
    esferas[6].c = setVector(1, 0, 1.6);
    esferas[6].color = setColor(235,220,178);
    esferas[6].id = 8;
    esferas[6].ri = 1;
    esferas[6].refleccion = 1;

    esferas[7].r = 0.5;
    esferas[7].c = setVector(1, 1.2, 0.8);
    esferas[7].color = setColor(0, 69, 122);
    esferas[7].id = 0;
    esferas[7].ri = 1;
    esferas[7].refleccion = 1;

    esferas[8].r = 0.5;
    esferas[8].c = setVector(1, -1.2, 0.8);
    esferas[8].color = setColor(0, 69, 122);
    esferas[8].id = 2;
    esferas[8].ri = 1;
    esferas[8].refleccion = 1;

    esferas[9].r = 0.5;
    esferas[9].c = setVector(1, 0, 0 );
    esferas[9].color = setColor(49, 36, 184);
    esferas[9].id = 8;
    esferas[9].ri = 1;
    esferas[9].refleccion = 1; 

    esferas[10].r = 0.5;
    esferas[10].c = setVector(1, 2.4, 0);
    esferas[10].color = setColor(173,96,144);
    esferas[10].id = 8;
    esferas[10].ri = 1;
    esferas[10].refleccion = 1;  

    esferas[11].r = 0.5;
    esferas[11].c = setVector(1, -2.4, 0);
    esferas[11].color = setColor(18, 1, 53);
    esferas[11].id = 8;
    esferas[11].ri = 1;
    esferas[11].refleccion = 1;

    esferas[12].r = 0.5;
    esferas[12].c = setVector(1, 1.2, -0.8);
    esferas[12].color = setColor(49, 36, 184);
    esferas[12].id = 3;
    esferas[12].ri = 1;
    esferas[12].refleccion = 1; 

    esferas[13].r = 0.5;
    esferas[13].c = setVector(1, -1.2, -0.8);
    esferas[13].color = setColor(49, 36, 184);
    esferas[13].id = 1;
    esferas[13].ri = 1;
    esferas[13].refleccion = 1;

    esferas[14].r = 0.5;
    esferas[14].c = setVector(1, 2.4, -1.6);
    esferas[14].color = setColor(173,96,144);
    esferas[14].id = 2;
    esferas[14].ri = 1;
    esferas[14].refleccion = 1;
    
    esferas[15].r = 0.5;
    esferas[15].c = setVector(1, -2.4, -1.6);
    esferas[15].color = setColor(18, 1, 53);
    esferas[15].id = 5;
    esferas[15].ri = 1;
    esferas[15].refleccion = 1; 

    esferas[16].r = 0.5;
    esferas[16].c = setVector(1, 0, -1.6);
    esferas[16].color = setColor(49, 36, 184);
    esferas[16].id = 8;
    esferas[16].ri = 1;
    esferas[16].refleccion = 1; 

    esferas[17].r = 0.5;
    esferas[17].c = setVector(1, 0, -3.2);
    esferas[17].color = setColor(201,211,206);
    esferas[17].id = 8;
    esferas[17].ri = 1;
    esferas[17].refleccion = 1; 

	esferas[18].r = 0.5;
    esferas[18].c = setVector(1, 1.2, -2.6 );
    esferas[18].color = setColor(173,96,144);
    esferas[18].id = 8;
    esferas[18].ri = 1;
    esferas[18].refleccion = 1; 

    esferas[19].r = 0.5;
    esferas[19].c = setVector(1, -1.2, -2.6 );
    esferas[19].color = setColor(18, 1, 53);
    esferas[19].id = 8;
    esferas[19].ri = 1;
    esferas[19].refleccion = 1; 
}

float calculoInterseccion(Rayo rayo, Esfera esfera){
	Vector restaVec = restaVector(rayo.origen,esfera.c);
	float multiVector = pPunto(rayo.direccion,rayo.direccion);
	float discriminante = pPunto(rayo.direccion, restaVec);
	discriminante *= discriminante;
	discriminante -= (multiVector*(pPunto(restaVec,restaVec)-pow(esfera.r,2.0)));

	if(discriminante >=0){

		float t = pPunto(escalaVector(rayo.direccion,-1),restaVec);
		float t1 = (t + sqrt(discriminante))/multiVector;
		float t2 = (t - sqrt(discriminante))/multiVector;

		if (t1 > 0 && t2>0){
			if(t1 < t2){
				return t1;
			}else{
				return t2;
			}
		}else if(t1>0){
			return t1;
		}else if(t2>0){
			return t2;
		}else{
			return -1;
		}
	}else{
		return -1;
	}
}

bool sombra(Rayo rayo){
	for (int i = 0; i < numeroEsferas; i++)
	{
		if (calculoInterseccion(rayo,esferas[i])>0)
		{
			return true;
		}
	}
	return false;
}

float choqueEscena(Rayo rayo, Choque* choque){

	float valor = 0;
	float valorFinal = 9999;
	float resultado = -1;

	for(int i=0; i<numeroEsferas; i++){
		valor = calculoInterseccion(rayo,esferas[i]);
		if (valor > 0 && valor < valorFinal){
			resultado = valor;
			choque->esfera = &esferas[i];
		}
	}
	choque->t = resultado;
}

Rayo analisisRayoVision(float i, float j, Vector origen){
	Rayo rayo;

    float us = izq + (der-izq) * (i+0.5) / ancho;
    float vs = abajo + (arriba-abajo) * (j+0.5) / alto;

    rayo.origen = origen;
    rayo.direccion = restaVector(sumaVector(escalaVector(u,us),escalaVector(v,vs)), rayo.origen);
    rayo.direccion = escalaVector(rayo.direccion,1/magnitud(rayo.direccion));

    return rayo;
}

Rayo analisisRayoVisionNormal(float i, float j){
	Rayo rayo;

    float us = izq + (der-izq) * (i+0.5) / ancho;
    float vs = abajo + (arriba-abajo) * (j+0.5) / alto;

    rayo.origen = e;
    rayo.direccion = sumaVector(escalaVector(w,-1*d),sumaVector(escalaVector(u,us), escalaVector(v,vs)));
    rayo.direccion = escalaVector(rayo.direccion,1/magnitud(rayo.direccion));

    return rayo;
}

Color blur(Vector vector,Color color, int numluces){
	float valor = pPunto(vector,luces[numluces]);
	float max = (valor > 0) ? valor : 0;
	float escala = luz * max;

	return aumentoColor(color,escala);
}

Color mejoraColor(Rayo rayo, Vector vectorIn, int numLuz){
	Color color = setColor(250,250,250);
	unsigned int potencia = 40;

	Vector vectorVista = escalaVector(rayo.direccion,1/magnitud(rayo.direccion));
	Vector vector = sumaVector(vectorVista,luces[numLuz]);
	vector = escalaVector(vector,1/magnitud(vector));

	float ppVectores = pPunto(vectorIn,vector);
	float max = (ppVectores > 0)? ppVectores : 0;

	max = pow(max,potencia);

	float escala = max * luz;

	return aumentoColor(color,escala);
}

Color ambiente(Color color){
	return aumentoColor(color,intensidadAmbiente);
}

Color choqueRayo(Rayo rayo, int valor){
	Choque choque;
	choqueEscena(rayo,&choque);

	if(choque.t > 0.000001){
		choque.p = sumaVector(rayo.origen, escalaVector(rayo.direccion,choque.t-0.01));
        choque.n = escalaVector(restaVector(choque.p, choque.esfera->c),-1/choque.esfera->r);
        choque.n = escalaVector(choque.n,1/magnitud(choque.n));
        return shade(choque, rayo, valor);
    }

    return colorFondo;
}

Rayo calculoSombra(Vector vector, Vector iluminacion){
	Rayo sombra;
	sombra.origen = vector;
	sombra.direccion = escalaVector(iluminacion,-1);
	return sombra;
}

Vector refleccion(Vector direccion, Vector normal){
	Vector vector = escalaVector(direccion,1/magnitud(direccion));
	return restaVector(vector,escalaVector(normal,2*pPunto(vector,normal)));
}

bool refraccion(Vector vectorInA, Vector vectorInB, float valor, Vector *temp){

	Vector temp1 = escalaVector(restaVector(vectorInA, escalaVector(vectorInB, pPunto(vectorInA, vectorInB))),1/valor);
    float raiz = 1 - (1 - pow(pPunto(vectorInA, vectorInB), 2.0)) / pow(valor, 2.0);
    
    if (raiz < 0) {
        return false;
    }

    Vector temp2 = escalaVector(vectorInB,sqrt(raiz));
    *temp = restaVector(temp1, temp2);
    *temp = escalaVector(*temp,1/magnitud(*temp));

    return true;
}

Color atenuacion(float kr, float kg, float kb, Color color) {
    Color salida;
    salida.r = color.r * kr;
    salida.g = color.g * kg;
    salida.b = color.b * kb;
    return salida;
}

Color shade(Choque choque,Rayo rayo, int valor){
	Color color = setColor(0,0,0);

	color = ambiente(choque.esfera->color);

	for (int i = 0; i < numeroLuces; i++){
		Rayo rayoSombra = calculoSombra(choque.p,luces[i]);

        if (!sombra(rayoSombra)) {
            color = sumaColor(color, blur(choque.n, choque.esfera->color, i));
            color = sumaColor(color, mejoraColor(rayo, choque.n, i));
        }
	}

	if (llaveRefleccion && choque.esfera->refleccion && valor > 0) {
        Rayo rayoRefleccion;
        rayoRefleccion.origen = choque.p;
       	rayoRefleccion.direccion = refleccion(rayo.direccion, choque.n);
        color = sumaColor(color, aumentoColor(choqueRayo(rayoRefleccion, valor-1), 0.25));
    }

    if (llaveTransparencia && choque.esfera->ri != 1 && valor > 0) {
        Vector vectorR = refleccion(rayo.direccion, choque.n);
        float kr, kg, kb;
        Rayo rayo1, rayo2;
        Vector temp;
        float valTemp;


        if (pPunto(rayo.direccion, choque.n) < 0) {
            refraccion(rayo.direccion, choque.n, choque.esfera->ri, &temp);
            valTemp = pPunto(escalaVector(rayo.direccion,-1), choque.n);
        } else {
            if (refraccion(rayo.direccion, escalaVector(choque.n,-1), 1/choque.esfera->ri, &temp)) {
                valTemp = pPunto(temp, choque.n);
            } else {
                rayo1.origen= choque.p;
                rayo1.direccion = vectorR;
                return sumaColor(color, choqueRayo(rayo1, valor-1));
            }
        }

        float r0 = pow(choque.esfera->ri-1, 2.0) / pow(choque.esfera->ri+1, 2.0);
        float r1 = r0 + (1-r0) * pow(1-valTemp, 5.0);

        rayo1.origen = choque.p;
        rayo1.direccion = vectorR;

        rayo2.origen = choque.p;
        rayo2.direccion = temp;

        return sumaColor(color,sumaColor(aumentoColor(choqueRayo(rayo1, valor-1), r1), 
        	aumentoColor(choqueRayo(rayo2, valor-1), (1-r1))));
    }
    return color;
}

Color AntiAlias(int antiX, int antiY){
	float muestras = 6;
	Color color = setColor(0,0,0);
	float x,y;
	float r;

	for(int i=0;i<muestras;i++){
		for(int j=0;j<muestras;j++){
			r = (rand()%100)/100.0f;

			x = (float)antiX + ((float)i+r)/muestras;
			y = (float)antiY + ((float)j+r)/muestras;

			Vector ojo = e;


			if(llaveProfundidad){
				ojo.y += ((float)j+r)/muestras;
				ojo.z += ((float)i+r)/muestras;
			}

			Rayo rayoVision = analisisRayoVision(x,y,ojo);

			color = sumaColor(color,choqueRayo(rayoVision,3));
		}
	}

	return aumentoColor(color,1/pow(muestras,2.0));
}

void display(){

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    srand(time(NULL));

	for (int i=0; i<ancho-1; i++) {
        for (int j=0; j<alto-1; j++) {
            
            Color color;

            if (llaveAntiAlias || llaveProfundidad) {
                color = AntiAlias(i,j);

            } else {
                color = choqueRayo(analisisRayoVision(i,j,e), 5);

            }
            int ipos = 3*(j*ancho+i);
            //printf("pos: %d, i: %d, j: %d\n",ipos,i,j);
            setColorPixel(color, &pixeles[ipos]);
        }
    }
    printf("fin Ray-Tracer\n");
    write_truecolor_tga();
    //crearBMP("test.bmp",ancho,alto,&pixeles);
    printf("Fin creacioArchivo\n");
    //glDrawPixels(ancho, alto, GL_RGB, GL_FLOAT, pixeles);

    // Reset buffer for next frame
    //glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char** argv){
	

	//glutInit(&argc, argv);
	inicioVectores();
	display();

    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    //glutInitWindowSize(ancho, alto);

    //glutCreateWindow("CAP 4730 | Final Project | Advanced Ray-Tracer");
    //glutDisplayFunc(display);
    //glutReshapeFunc(reshape);
    //glutIdleFunc(idle);

    //glutMainLoop();

	return 0;
}