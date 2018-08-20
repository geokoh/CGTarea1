
#include <stdio.h>
#include "malloc.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

bool key;
int resolucion, lineas, veces;

Matriz **valores_0;
Matriz **valores_1;


void plot(int x, int y){
	// Representa el pixel cuando la llave booleana estC!
	// setea en true
	if(key){
		printf("X= %d , Y= %d . \n", x, y);
	}
}

void line(int x0,int y0,int x1,int y1){

	float m,b,y;
	int i, roundedY;

	m = (y1-y0)/(x1-x0);
	b = y0-m*x0;

	for(i=x0; i <= x1; i++){
		y = m*i + b;
		roundedY = round(y);
		plot(i, roundedY);
	}
}

void line2(int x0,int y0,int x1,int y1){

	float m, y;
	int i, roundedY;
	m= (y1-y0)/(x1-x0);
	y = y0;

	for(i=x0; i<=x1; i++){
	    roundedY = round(y);
		plot(i, roundedY);
		y +=m;
	}
}

void line3(int x0,int y0,int x1,int y1){

	float x, y, paso_x, paso_y;
	int i, ancho, roundedX, roundedY;

	ancho = fmax(abs(x1-x0),abs(y1-y0));
	printf("%d\n", ancho);
	paso_x = (x1-x0)/ancho;
	paso_y = (y1-y0)/ancho;

	x= x0; y=y0;

	for(i=0; i <= ancho; i++){
	    roundedX = round(x);
	    roundedY = round(y);
		plot(roundedX, roundedY);
		x+=paso_x;
		y+=paso_y;
	}
}

void line4(int x0,int y0,int x1,int y1){

	int delta_E, delta_NE, d, Xp, Yp;
	int delta_y, delta_x, incX, incY;

	delta_x = abs(x1-x0);
	delta_y = abs(y1-y0);


	Xp = x0; Yp = y0; //valores iniciales

	plot(Xp,Yp); //pinta iniciales

	incX=1;incY=1; //incrementos para los puntos

	if(x1 < x0) incX = -1; //decremento para cuando el punto 2 en menor al 1
	if(y1 < y0) incY = -1;

	if (delta_x > delta_y){

		d = 2* delta_y-delta_x;
		delta_NE = 2*(delta_y-delta_x);
		delta_E = 2*delta_y;

		for(int i=0; i<delta_x;i++) {
			if(d <= 0){ //Pintar E
				Xp+=incX;
				d = d+ delta_E;
			}else{
				//Pintar NE
				Xp+=incX; 
				Yp+=incY;
				d= d+delta_NE;
			}
			plot(Xp,Yp);
		}
	}else{
		d = 2* delta_x-delta_y;
		delta_NE = 2*(delta_x-delta_y);
		delta_E = 2*delta_x;

		for (int i=0; i<delta_y; i++){
			if(d <= 0){ //Pintar E
				Yp+=incY;
				d = d+ delta_E;
			}else{
				//Pintar NE
				Xp+=incX; 
				Yp+=incY;
				d= d+delta_NE;
			}
			plot(Xp,Yp);
		}
	}
}

void benchmark(){
	
	clock_t inicio, fin;
   	double tiempo;
   	int i,j;

	for(int cont = 0; cont<2;cont++){
		if (!key){
			printf("\n Tiempos de EjeccuciC3n Puros\n");
		}else{
			printf("\n Tiempos de EjeccuciC3n Graficos\n");
		}

//---------------------------------------------------------------------------------------------------------		
		inicio = clock();
		for( i = 0 ; i < veces  ; i++ ) {
			for( j = 0 ; j < (lineas) ; j++ ) {
				line(valores_0[j][0].x,valores_0[j][1].y,valores_1[j][0].x,valores_1[j][1].y);
				printf(" Pos: %d ** x0: %d, y0: %d ", i+1,valores_0[j][0].x,valores_0[j][1].y);
				printf("** x1: %d  y1: %d\n", valores_1[j][0].x,valores_1[j][1].y);
			}
		}
		fin = clock();
		tiempo = (double) fin-inicio;
		printf(" Tiempo: Algoritmo fuerza bruta = %f \n",tiempo);
		if(key){
			//glFlush();
			sleep(2);
		}
//---------------------------------------------------------------------------------------------------------		
		//if(key) glColor3f(1,0,0);
		inicio = clock();
		for( i = 0 ; i < veces  ; i++ ) {
			for( j = 0 ; j < (lineas)  ; j++ ) {
				line2(valores_0[j][0].x,valores_0[j][1].y,valores_1[j][0].x,valores_1[j][1].y);
				printf(" Pos: %d ** x0: %d, y0: %d ", i+1,valores_0[j][0].x,valores_0[j][1].y);
				printf("** x1: %d  y1: %d\n", valores_1[j][0].x,valores_1[j][1].y);
			}
		}
		fin = clock();
		tiempo = (double) fin-inicio;
		printf(" Tiempo: Algoritmo incremental = %f\n",tiempo);
		if(key){
			//glFlush();
			sleep(2);
		}
//---------------------------------------------------------------------------------------------------------
		/*
		if(key) glColor3f(0,1,0);
		inicio = clock();
		for( i = 0 ; i < veces  ; i++ ) {
			for( j = 0 ; j < (lineas)  ; j++ ) {
				line3(valores_0[j][0].x,valores_0[j][1].y,valores_1[j][0].x,valores_1[j][1].y);
				printf(" Pos: %d ** x0: %d, y0: %d ", i+1,valores_0[j][0].x,valores_0[j][1].y);
				printf("** x1: %d  y1: %d\n", valores_1[j][0].x,valores_1[j][1].y);
			}
		}
		fin = clock();
		tiempo = (double) fin-inicio;
		printf(" Tiempo: Algoritmo incremental vers 2 = %f\n",tiempo);
		if(key){
			glFlush();
			sleep(2);
		}
		*/
//------------------------------------------------------------------------------------------------------------	
		//if(key) glColor3f(0,0,1);
		inicio = clock();
		for( i = 0 ; i < veces  ; i++ ) {
			for( j = 0 ; j < (lineas); j++ ) {
				line4(valores_0[j][0].x,valores_0[j][1].y,valores_1[j][0].x,valores_1[j][1].y);
				printf(" Pos: %d ** x0: %d, y0: %d ", i+1,valores_0[j][0].x,valores_0[j][1].y);
				printf("** x1: %d  y1: %d\n", valores_1[j][0].x,valores_1[j][1].y);
			}
		}
		fin = clock();
		tiempo = (double) fin-inicio;
		printf(" Tiempo: Algoritmo de Bresenham = %f\n",tiempo);
		if(key){
			//glFlush();
			sleep(2);
		}
//------------------------------------------------------------------------------------------------------------
		printf("%d\n",cont );
		key = true;
	}
	
}

int main (int argc, char** argv) {

    clock_t inicio, fin;
    double tiempo;
    key = false;
    inicio = clock();
    printf( "Introduzca: Resolucion, Numero de Lineas, Numero de veces\n");
    scanf("%d %d %d", &resolucion, &lineas, &veces);
    int i, j;
    time_t t;
    srand((unsigned) time(&t));

    printf("\n Matriz de Puntos:\n");
	valores_0 = (Matriz **)malloc(lineas * sizeof(Matriz*));
	valores_1 = (Matriz **)malloc(lineas * sizeof(Matriz*));

  	for (i = 0; i < lineas; i++) 
      {
       valores_0[i] = (Matriz *)malloc(lineas * sizeof(Matriz));
       valores_1[i] = (Matriz *)malloc(lineas * sizeof(Matriz));
      }

  	for (i = 0; i < lineas; i++){
        valores_0[i][0].x = 94;
        valores_0[i][1].y = 70;
        printf(" Pos: %d ** x0: %d, y0: %d ", i+1,valores_0[i][0].x,valores_0[i][1].y);
        valores_1[i][0].x = 95;
        valores_1[i][1].y = 81;
        printf("** x1: %d  y1: %d\n", valores_1[i][0].x,valores_1[i][1].y);
      }

	    
    benchmark();
}

