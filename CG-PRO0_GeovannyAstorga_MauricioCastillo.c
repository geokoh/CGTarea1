/*
 * Instituto Tecnologico de Costa Rica
 * Escuela de Ingenieria en Computacion
 * Computer Graphics
 *
 * Programa: CG-PROG0
 * Archivo: CG-PROG0_GeovannyAstorga_MaurcioCastillo.c
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
 // cambiar al punto h
#include <stdio.h>

bool key;
int resolucion,lineas, veces;

void line(int x0,int y0,int x1,int y1){

	long double m,b,y;
	int i;

	m = (y1-y0)/(x1-x0);
	b = y0-m*x0;

	for(i=x0; i <= x1; i++){
		y = m*i + b;
		plot(i,round(y));
	}
}

void line2(int x0,int y0,int x1,int y1){

	long double m, y;
	int i;
	m= (y1-y0)/(x1-x0);
	y = y0

	for(i=x0; i<=x1; i++){
		plot(i, round(y));
		y +=m;
	}

}

void line3(int x0,int y0,int x1,int y1){

	long double x, y, paso_x, paso_y;
	int i, ancho;

	ancho = max(abs(x1-x0),abs(y1-y0));
	paso_x = (x1-x0)/ancho;
	paso_y = (y1-y0)/ancho;

	x= x0; y=y0;

	for(i=0; i <= ancho; i++){
		plot(round(x),round(y));
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
	if(y2 < y0) incY = -1;

	if (delta_y < delta_x){

		d = 2* delta_y-delta_x;
		delta_NE = 2*(delta_y-delta_x);
		delta_E = 2*delta_y;

		while(Xp < x1){
			if(d <= 0){ //Pintar E
				Xp++;
				d = d+ delta_E;
			}else{
				//Pintar NE
				Xp++; Yp++;
				d= d+delta_NE;
			}
			plot(Xp,Yp);
		}
	}else{

		d = 2* delta_x-delta_y;
		delta_NE = 2*(delta_x-delta_y);
		delta_E = 2*delta_x

		while(Yp < y1){
			if(d <= 0){ //Pintar E
				Yp++;
				d = d+ delta_E;
			}else{
				//Pintar NE
				Xp++; Yp++;
				d= d+delta_NE;
			}
			plot(Xp,Yp);
		}
	}
}

void plot(int X, int y){
	// Representa el pixel cuando la llave booleana está 
	// setea en true 
	if(key){
		glBegin(GL_POINTS);
		glVertex2i(x,y);
		glEnd();
	}
}

void benchmark(){

	key = false;

	if (key){
		glutInit(&argc, argv);
	  	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	  	glutInitWindowSize(resolucion,resolucion);
	  	glutCreateWindow("CG-PROG0_GeovannyAstorga_MaurcioCastillo");
	  	glClear(GL_COLOR_BUFFER_BIT);
	  	gluOrtho2D(-0.5, resolucion +0.5, -0.5, resolucion + 0.5);
	  	glutDisplayFunc(benchmark);
	  	glutMainLoop();
		}

}

int main (int argc, char** argv) {

	printf( "Introduzca (Resolucion, Numero de Lineas, Numero de veces\n");
	scanf("%d %d %d ", &resolucion, &lineas, &veces);

	benchmark();
  	//cambiar si se crean varias ventanas *****
}
