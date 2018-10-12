/* Tarea Programada #2 
 * Autor: Mauricio Castillo Vega
 * Autor: Geovanny Astorga Lopez
 * Curso: Computer Graphics 2-2018
 * Profesor: Dr. Francisco Torres
 * Fuentes Principales:
 * https://gist.github.com/veb/4a184a1f53e5fa9fea02
 * https://github.com/trentmwillis/ray-tracer
 */

#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "rayTrace.h"

unsigned int ancho;
unsigned int alto;
unsigned int cantColores;
unsigned int numeroEsferas;

//Variables globales y de funcionamiento
float pixeles[5008*5000*3];

Esfera esferas[10000];
Vector luces[1000];

int resultado[10];
float resultadoFloat[10];

Vector e;
float d = 10;

Vector w;
Vector u;
Vector v;

float facAtenuacion;

float izq = -8.064, der = 8.064;
float abajo = -4.536, arriba = 4.536;

float luz;
int numeroLuces;
float intensidadAmbiente;

Color colorFondo;

FILE *file; 

// Inicio de las funciones de trabajo


int escribirArchivo() {

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
    for (int y = 0; y < alto; y++)
        for (int x = 0; x < ancho; x++) {
            ipos = (3*y*ancho) +(x*3); //
            
            fputc((int) (pixeles[ipos+2] * 255), fp);
            fputc((int) (pixeles[ipos+1] * 255), fp);
            fputc((int) (pixeles[ipos+0] * 255), fp);
        }

    // The file footer
    static const char footer[26] =
            "\0\0\0\0" 
            "\0\0\0\0" 
            "TRUEVISION-XFILE" 
            ".";
    fwrite((const char *) &footer, 1, sizeof(footer), fp);

    fclose(fp);
    return 1;
}



void leerInt()
{
    int x = 0;
    char * token;
    char caracter[100];
    if((fgets(caracter, 100, file))!= EOF){
    	token = strtok(caracter, " ");
	    while( token != NULL ) {
	      resultado[x] = atoi(token);
	      x++;
	      token = strtok(NULL, " ");
	   }
	    
    }
    

}

void leerFloat()
{
    int x = 0;
    char * token;
    char * token2;
    char caracter[100];
    if((fgets(caracter, 100, file))!= EOF){
        token = strtok(caracter, " ");
	    while( token != NULL ) {;
	      resultadoFloat[x] = strtof(token, &token2);
	      x++;
	      token = strtok(NULL, " ");
	    }
    }
    
}

void inicioVectores() {
	leerInt();
    ancho = resultado[0];
    alto = resultado[1];
    leerInt();
    cantColores = resultado[0];
    leerInt();
    numeroEsferas = resultado[0];
    leerFloat();
    facAtenuacion = resultadoFloat[0];
    leerFloat();
    luz = resultadoFloat[0];
    leerInt();
    numeroLuces = resultado[0];
    leerFloat();
    intensidadAmbiente = resultadoFloat[0];

    int r,g,b;
    float x,y,z;
    leerInt();
    r = resultado[0];
    g = resultado[1];
    b = resultado[2];

    colorFondo = setColor(r, g, b);
    leerFloat();
    x = resultadoFloat[0];
    y = resultadoFloat[1];
    z = resultadoFloat[2];
    e = setVector(x, y, z);

    leerFloat();
    x = resultadoFloat[0];
    y = resultadoFloat[1];
    z = resultadoFloat[2];
    Vector vecSuperior = setVector(x, y, z);

    leerFloat();
    x = resultadoFloat[0];
    y = resultadoFloat[1];
    z = resultadoFloat[2];
    Vector vistaDireccion = setVector(x, y, z);


    w = escalaVector(vistaDireccion,-1/magnitud(vistaDireccion));
    Vector vecPCruz = pCruz(vecSuperior, w);
    u = escalaVector(vecPCruz,1/magnitud(vecPCruz));
    v = pCruz(w, u);
    for (int i = 0; i < numeroLuces; ++i)
    {   
    	leerFloat();
    	x = resultadoFloat[0];
    	y = resultadoFloat[1];
    	z = resultadoFloat[2];
        luces[i] = setVector(x,y,z);
        luces[i] = escalaVector(luces[i],1/magnitud(luces[i]));
    }

    for (int i = 0; i < numeroEsferas; ++i){

    	leerFloat();
        esferas[i].r = resultadoFloat[0];
        x = resultadoFloat[1];
    	y = resultadoFloat[2];
    	z = resultadoFloat[3];
        esferas[i].c = setVector(x, y, z);

        x = resultadoFloat[4];
    	y = resultadoFloat[5];
    	z = resultadoFloat[6];
        esferas[i].color = setColor(x,y,z);
        esferas[i].id = 3;
        esferas[i].ri = 1;
        esferas[i].refleccion = 1;


    }

 
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

Color atenuacion(float factor, Color color) {
    Color salida;
    salida.r = color.r * factor;
    salida.g = color.g * factor;
    salida.b = color.b * factor;
    return salida;
}

Color shade(Choque choque,Rayo rayo, int valor){
    Color color = setColor(255,255,255);

    color = ambiente(choque.esfera->color);

    for (int i = 0; i < numeroLuces; i++){
        for (int j = 0; j < numeroEsferas; j++)
        {
            if (calculoInterseccion(rayo,esferas[j])>0) {
                color = atenuacion(facAtenuacion,color);
                color = sumaColor(color, blur(choque.n, choque.esfera->color, i));
                color = sumaColor(color, mejoraColor(rayo, choque.n, i));
            }
        }
    }
    return color;
}

void display(){

    srand(time(NULL));


    for (int i=0; i<ancho-1; i++) {
        for (int j=0; j<alto-1; j++) {
            
            Color color;

            color = choqueRayo(analisisRayoVision(i,j,e), 5);

            int ipos = 3*(j*ancho+i);
            setColorPixel(color, &pixeles[ipos]);
        }
    }
    printf("fin Ray-Tracer\n");
    escribirArchivo();
    printf("Fin creacioArchivo\n");
}

int main(int argc, char** argv){
    
    char* ruta;

    if (argc != 2){
        printf("Los argumentos son insuficientes o tiene argumentos de más\n");
        exit(1);
    }

    if(argc == 2){
        ruta = argv[1];
        file = fopen(ruta,"r");
        if(file == NULL){
            printf("fallo\n");
            exit(0);
        }
    }

    inicioVectores();
    display();

    return 0;
}