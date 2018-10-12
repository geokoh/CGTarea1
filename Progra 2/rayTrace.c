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
    // The image data is stored bottom-to-top, left-to-right
    for (int y = 0; y < alto; y++)
        for (int x = 0; x < ancho; x++) {
            ipos = (3*y*ancho) +(x*3); //
            
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

void leerCaracter(){
    char caracter[100];
    char salida[100];

    fgets(caracter,100,file);
    int i=0;
    while(caracter[i]!='20H'){
        salida = caracter[i];
        i++;
    }
    printf("%s\n", salida);
    //}

    //while((caracter = fgetc(file))!= EOF){ //&& isspace(caracter)){
    //    printf("%d",caracter);
    //}

    //printf("%d\n", salida);
    //return caracter;
}

int leerInt()
{
    int x;

    leerCaracter();
    //atol(x);
    
    return x;
}

float leerFloat()
{
    float x;

    leerCaracter();
        
    return x;
}

void inicioVectores() {

    ancho = leerInt();
    printf("%d\n", ancho);
    alto = leerInt();
    cantColores = leerInt();
    numeroEsferas = leerInt();
    facAtenuacion = leerFloat();
    luz = leerFloat();
    numeroLuces = leerInt();
    intensidadAmbiente = leerFloat();

    int r,g,b;
    float x,y,z;

    r = leerInt();
    g = leerInt();
    b = leerInt();

    colorFondo = setColor(r, g, b);

    x = leerFloat();
    y = leerFloat();
    z = leerFloat(); 
    e = setVector(x, y, z);

    x = leerFloat();
    y = leerFloat();
    z = leerFloat();
    Vector vecSuperior = setVector(x, y, z);

    x = leerFloat();
    y = leerFloat();
    z = leerFloat();
    Vector vistaDireccion = setVector(x, y, z);


    w = escalaVector(vistaDireccion,-1/magnitud(vistaDireccion));
    Vector vecPCruz = pCruz(vecSuperior, w);
    u = escalaVector(vecPCruz,1/magnitud(vecPCruz));
    v = pCruz(w, u);

    for (int i = 0; i < numeroLuces; ++i)
    {   
        x = leerFloat();
        y = leerFloat();
        z = leerFloat();
        luces[i] = setVector(x,y,z);
        luces[i] = escalaVector(luces[i],1/magnitud(luces[i]));
    }
/*
    luces[2] = setVector(0,0,15);
    luces[2] = escalaVector(luces[2],1/magnitud(luces[2]));

    luces[1] = setVector(0,-1,-1);
    luces[1] = escalaVector(luces[1],1/magnitud(luces[1]));

    luces[0] = setVector(0,1,-1);
    luces[0] = escalaVector(luces[0],1/magnitud(luces[0]));

    luces[3] = setVector(-1,1,1);
    luces[3] = escalaVector(luces[3],1/magnitud(luces[3]));
*/
    for (int i = 0; i < numeroEsferas; ++i){
        esferas[i].r = leerFloat();
        x = leerFloat();
        y = leerFloat();
        z = leerFloat();
        esferas[i].c = setVector(x, y, z);

        r = leerInt();
        g = leerInt();
        b = leerInt();
        esferas[i].color = setColor(r,g,b);
        esferas[i].id = 3;
        esferas[i].ri = 1;
        esferas[i].refleccion = 1;
    }
//    
/*esferas[1].r = 0.5;
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
*/
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
    leerCaracter();
    //int y = leerInt;
    //printf("%d\n",y);
    //inicioVectores();
    //display();

    return 0;
}