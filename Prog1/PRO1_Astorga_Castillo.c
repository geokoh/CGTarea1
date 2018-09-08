/*
 * Instituto Tecnologico de Costa Rica
 * Escuela de Ingenieria en Computacion
 * Computer Graphics
 *
 * Programa: CG-PROG1
 * Archivo: PROG1_Astorga_Castillo.c
 */

#include "PRO1_Astorga_Castillo.h"
#include "malloc.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

int resolucion = 800;

/* Matrices para las provincias
** 0: Guanacaste 1: Puntarenas Peninsula
** 2: Alajuela 3: Heredia
** 4: Limon 5: Cartago
** 6: San Jose 7: Puntarenas Costa
*/
Matriz **valores_0;
Matriz **valores_1;
Matriz **valores_2;
Matriz **valores_3;
Matriz **valores_4;
Matriz **valores_5;
Matriz **valores_6;
Matriz **valores_7;

/* Ints para la cantidad de vertices
** lineas: Guanacaste lineas1: Puntarenas Peninsula
** lineas3: Alajuela lineas4: Heredia
** lineas5: Limon lineas6: Cartago
** lineas7: San Jose lineas 8: Puntarenas Costa
*/
int lineas, lineas1, lineas3, lineas4,lineas5, lineas6, lineas7, lineas8;

int INSIDE = 0;
int LEFT = 1;
int RIGHT = 2;
int BOTTOM = 4;
int TOP = 8;

int y_max;
int x_max;
int y_min;
int x_min;

void plot(int x, int y){
	// Representa el pixel
		glBegin(GL_POINTS);
		glVertex2i(x,y);
		glEnd();
}

void bresenham(int x0,int y0,int x1,int y1){

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

int computeCode(double x, double y){
    // el default es dentro de la ventana
    int code = INSIDE;
 
    if (x < x_min)       // izquierda
        code |= LEFT;
    else if (x > x_max)  // derecha
        code |= RIGHT;
    if (y < y_min)       // abajo
        code |= BOTTOM;
    else if (y > y_max)  // arriba
        code |= TOP;
 
    return code;
}

void cohenSutherlandClip(double x1, double y1,double x2, double y2){
    
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
 
    
    bool accept = false;
 
    while (true)
    {
        if ((code1 == 0) && (code2 == 0))
        {
            // caso trivial donde los dos puntos estC!n dentro de la ventana
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // caso trivial donde los dos codigos son iguales y se rechasa
            break;
        }
        else
        {
            // caso donde por lo menos uno de los puntos estC! por fuera de la venta
            int code_out;
            double x, y;
 
            //se prueba el codigo de los puntos para ver cual de los dos estC! fuera de la ventana
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;
 
            // se encuentra la interseccion entre el punto y la ventana
            if (code_out & TOP)
            {
                // el punto estC! arriba
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            }
            else if (code_out & BOTTOM)
            {
                // el punto estC! abajo
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            }
            else if (code_out & RIGHT)
            {
                // el punto estC! a la derecha
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            }
            else if (code_out & LEFT)
            {
                // el punto estC! a la izquierda
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }
 
            //se reemplaza el punto fuera de la ventana por la interseccion que se encontro
            if (code_out == code1)
            {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    if (accept)
    {
        printf("Linea dibujada de (%f, %f) a (%f, %f) \n", x1, y1, x2, y2);
    }
    else
        printf("Linea fuera de la ventana \n");
}

MatrizOp** Zoom(int Sx,int Sy){

	//Inicialización de la matriz de operaciones
	MatrizOp **matriz_zoom;
	matriz_zoom = (MatrizOp **)malloc(3 * sizeof(MatrizOp*));
	for (int i = 0; i < 3; i++){
       matriz_zoom[i] = (MatrizOp *)malloc(3 * sizeof(MatrizOp));
	}
   for(int i = 0; i < 3; i++) {
   		for(int j=0;j<3; j++){
   			if(i==j){
   				if(j==1){
   					matriz_zoom[i][j].valor = Sx;
   				}
   				if(j==2){
   					matriz_zoom[i][j].valor = Sy;
   				}
   				if(j==3){
   					matriz_zoom[i][j].valor = 1;
   				}
   			}else{
   				matriz_zoom[i][j].valor = 0;
   			}
   		}
   }
   return matriz_zoom;
}

MatrizOp** Pan(int delta_x,int delta_y){

	//Inicialización de la matriz de operaciones
	MatrizOp **matriz_pan;
	matriz_pan = (MatrizOp **)malloc(3 * sizeof(MatrizOp*));
	for (int i = 0; i < 3; i++){
       matriz_pan[i] = (MatrizOp *)malloc(3 * sizeof(MatrizOp));
	}

    for(int i = 0; i < 3; i++) {
   		for(int j=0;j<3; j++){
   			if(i==j){
   				//Se agregan los valores de la diagonal
				matriz_pan[i][j].valor = 1;
   			}else{
   				if((j==2) && (i==0)){
   					matriz_pan[i][j].valor = delta_x;
   				}else{
   					if((j==2) && (i==1)){
   						matriz_pan[i][j].valor = delta_y;
   					}else{
   						matriz_pan[i][j].valor = 0;
   					}
   				}
   			}
   		}
   }
   return matriz_pan;
}

MatrizOp** RotateLeft(double alfa){

	//Inicialización de la matriz de operaciones
	MatrizOp **matriz_RotL;
	matriz_RotL = (MatrizOp **)malloc(3 * sizeof(MatrizOp*));
	for (int i = 0; i < 3; i++){
       matriz_RotL[i] = (MatrizOp *)malloc(3 * sizeof(MatrizOp));
	}
	//Valores de rotación con cos y sin 
	matriz_RotL[0][0].valor = cos(alfa);
	matriz_RotL[0][1].valor = -1*sin(alfa);
	matriz_RotL[1][0].valor = sin(alfa);
	matriz_RotL[1][1].valor = cos(alfa);
	
	//Ceros de la matriz para la coordenadas homogeneas
	matriz_RotL[0][2].valor = 0;
	matriz_RotL[1][2].valor = 0;
	matriz_RotL[2][0].valor = 0;
	matriz_RotL[2][1].valor = 0;
	matriz_RotL[2][2].valor = 1;
}

MatrizOp** RotateRight(double alfa){

	//Inicialización de la matriz de operaciones
	MatrizOp **matriz_RotR;
	matriz_RotR = (MatrizOp **)malloc(3 * sizeof(MatrizOp*));
	for (int i = 0; i < 3; i++){
       matriz_RotR[i] = (MatrizOp *)malloc(3 * sizeof(MatrizOp));
	}
	//Valores de rotación con cos y sin 
	matriz_RotR[0][0].valor = cos(alfa);
	matriz_RotR[0][1].valor = -1*sin(alfa);
	matriz_RotR[1][0].valor = sin(alfa);
	matriz_RotR[1][1].valor = cos(alfa);

	//Ceros de la matriz para la coordenadas homogeneas
	matriz_RotR[0][2].valor = 0;
	matriz_RotR[1][2].valor = 0;
	matriz_RotR[2][0].valor = 0;
	matriz_RotR[2][1].valor = 0;
	matriz_RotR[2][2].valor = 1;
}

void IniciaMatrices(){

	/* Se encarga de la incializacion de las matrices que se utilicen en el código*/

	//la cantidad de vertices por provincia
    lineas = 23;
    lineas1 = 8;
    lineas3 = 19;
    lineas4 = 9;
    lineas5 = 23;
    lineas6 = 12;
    lineas7 = 19;
    lineas8 = 38;
    int i,j;

    //Inicializa las matrices
    valores_0 = (Matriz **)malloc(lineas * sizeof(Matriz*));
	valores_1 = (Matriz **)malloc(lineas1 * sizeof(Matriz*));
	valores_2 = (Matriz **)malloc(lineas3 * sizeof(Matriz*));
	valores_3 = (Matriz **)malloc(lineas4 * sizeof(Matriz*));
	valores_4 = (Matriz **)malloc(lineas5 * sizeof(Matriz*));
	valores_5 = (Matriz **)malloc(lineas6 * sizeof(Matriz*));
	valores_6 = (Matriz **)malloc(lineas7 * sizeof(Matriz*));
	valores_7 = (Matriz **)malloc(lineas8 * sizeof(Matriz*));
	//Agrega las filas
  	for (i = 0; i < lineas; i++) {valores_0[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
	for (i = 0; i < lineas1; i++){valores_1[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
	for (i = 0; i < lineas3; i++){valores_2[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
	for (i = 0; i < lineas4; i++){valores_3[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
	for (i = 0; i < lineas5; i++){valores_4[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
	for (i = 0; i < lineas6; i++){valores_5[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
	for (i = 0; i < lineas7; i++){valores_6[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
	for (i = 0; i < lineas8; i++){valores_7[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
}

void CoordMapas(){

	/* 	Agrega las coordenadas iniciales del mapa
		a las diferentes matrices para su procesamiento
		posterior
	*/

    //Vertices Guanacaste
    valores_0[0][0].x = 5*10; 
    valores_0[0][1].y = 43*10;
    valores_0[1][0].x = 3*10;
    valores_0[1][1].y = 41*10;
    valores_0[2][0].x = 1*10;
    valores_0[2][1].y = 39*10;
    valores_0[3][0].x = 2*10;
    valores_0[3][1].y = 38*10;
    valores_0[4][0].x = 2*10;
    valores_0[4][1].y = 33*10;
    valores_0[5][0].x = 1*10;
    valores_0[5][1].y = 31*10;
    valores_0[6][0].x = 2*10;
    valores_0[6][1].y = 28*10;
    valores_0[7][0].x = 4*10;
    valores_0[7][1].y = 25*10;
    valores_0[8][0].x = 7*10;
    valores_0[8][1].y = 24*10;
    valores_0[9][0].x = 10*10;
    valores_0[9][1].y = 23*10;
    valores_0[10][0].x = 9*10;
    valores_0[10][1].y = 25*10;
    valores_0[11][0].x = 10*10;
    valores_0[11][1].y = 27*10;
    valores_0[12][0].x = 9*10;
    valores_0[12][1].y = 30*10;
    valores_0[13][0].x = 11*10;
    valores_0[13][1].y = 29*10;
    valores_0[14][0].x = 15*10;
    valores_0[14][1].y = 29*10;
    valores_0[15][0].x = 16*10;
    valores_0[15][1].y = 31*10;
    valores_0[16][0].x = 15*10;
    valores_0[16][1].y = 33*10;
    valores_0[17][0].x = 13*10;
    valores_0[17][1].y = 35*10;
    valores_0[18][0].x = 11*10;
    valores_0[18][1].y = 36*10;
    valores_0[19][0].x = 9*10;
    valores_0[19][1].y = 36*10;
    valores_0[20][0].x = 7*10;
    valores_0[20][1].y = 39*10;
    valores_0[21][0].x = 9*10;
    valores_0[21][1].y = 42*10;
    valores_0[22][0].x = 6*10;
    valores_0[22][1].y = 42*10;
    //Fin
    // Vertices Puntarenas Peninsula
    valores_1[0][0].x = 10*10; 
    valores_1[0][1].y = 23*10;
    valores_1[1][0].x = 11*10;
    valores_1[1][1].y = 21*10;
    valores_1[2][0].x = 12*10;
    valores_1[2][1].y = 20*10;
    valores_1[3][0].x = 14*10;
    valores_1[3][1].y = 22*10;
    valores_1[4][0].x = 16*10;
    valores_1[4][1].y = 23*10;
    valores_1[5][0].x = 15*10;
    valores_1[5][1].y = 25*10;
    valores_1[6][0].x = 10*10;
    valores_1[6][1].y = 27*10;
    valores_1[7][0].x = 9*10;
    valores_1[7][1].y = 25*10;
    //Fin
    // Vertices Alajuela
    valores_2[0][0].x = 11*10; 
    valores_2[0][1].y = 41*10;
    valores_2[1][0].x = 9*10;
    valores_2[1][1].y = 42*10;
    valores_2[2][0].x = 7*10;
    valores_2[2][1].y = 39*10;
    valores_2[3][0].x = 9*10;
    valores_2[3][1].y = 36*10;
    valores_2[4][0].x = 11*10;
    valores_2[4][1].y = 36*10;
    valores_2[5][0].x = 13*10;
    valores_2[5][1].y = 35*10;
    valores_2[6][0].x = 15*10;
    valores_2[6][1].y = 33*10;
    valores_2[7][0].x = 16*10;
    valores_2[7][1].y = 31*10;
    valores_2[8][0].x = 16*10;
    valores_2[8][1].y = 29*10;
    valores_2[9][0].x = 19*10;
    valores_2[9][1].y = 29*10;
    valores_2[10][0].x = 20*10;
    valores_2[10][1].y = 24*10;
    valores_2[11][0].x = 25*10;
    valores_2[11][1].y = 25*10;
    valores_2[12][0].x = 25*10;
    valores_2[12][1].y = 37*10;
    valores_2[13][0].x = 24*10;
    valores_2[13][1].y = 39*10;
    valores_2[14][0].x = 23*10;
    valores_2[14][1].y = 40*10;
    valores_2[15][0].x = 20*10;
    valores_2[15][1].y = 40*10;
    valores_2[16][0].x = 18*10;
    valores_2[16][1].y = 41*10;
    valores_2[17][0].x = 16*10;
    valores_2[17][1].y = 40*10;
    valores_2[18][0].x = 15*10;
    valores_2[18][1].y = 39*10;
    //Fin
    //Vertices Heredia
    valores_3[0][0].x = 25*10; 
    valores_3[0][1].y = 37*10;
    valores_3[1][0].x = 25*10;
    valores_3[1][1].y = 25*10;
    valores_3[2][0].x = 28*10;
    valores_3[2][1].y = 26*10;
    valores_3[3][0].x = 28*10;
    valores_3[3][1].y = 29*10;
    valores_3[4][0].x = 30*10;
    valores_3[4][1].y = 29*10;
    valores_3[5][0].x = 30*10;
    valores_3[5][1].y = 32*10;
    valores_3[6][0].x = 32*10;
    valores_3[6][1].y = 37*10;
    valores_3[7][0].x = 30*10;
    valores_3[7][1].y = 36*10;
    valores_3[8][0].x = 28*10;
    valores_3[8][1].y = 37*10;
    //Fin
    // Vertices Limon
    valores_4[0][0].x = 30*10; 
    valores_4[0][1].y = 28*10;
    valores_4[1][0].x = 30*10;
    valores_4[1][1].y = 29*10;
    valores_4[2][0].x = 30*10;
    valores_4[2][1].y = 32*10;
    valores_4[3][0].x = 32*10;
    valores_4[3][1].y = 37*10;
    valores_4[4][0].x = 34*10;
    valores_4[4][1].y = 36*10;
    valores_4[5][0].x = 36*10;
    valores_4[5][1].y = 35*10;
    valores_4[6][0].x = 37*10;
    valores_4[6][1].y = 33*10;
    valores_4[7][0].x = 40*10;
    valores_4[7][1].y = 31*10;
    valores_4[8][0].x = 45*10;
    valores_4[8][1].y = 24*10;
    valores_4[9][0].x = 49*10;
    valores_4[9][1].y = 19*10;
    valores_4[10][0].x = 51*10;
    valores_4[10][1].y = 18*10;
    valores_4[11][0].x = 48*10;
    valores_4[11][1].y = 19*10;
    valores_4[12][0].x = 47*10;
    valores_4[12][1].y = 17*10;
    valores_4[13][0].x = 46*10;
    valores_4[13][1].y = 10*10;
    valores_4[14][0].x = 44*10;
    valores_4[14][1].y = 12*10;
    valores_4[15][0].x = 41*10;
    valores_4[15][1].y = 14*10;
    valores_4[16][0].x = 39*10;
    valores_4[16][1].y = 16*10;
    valores_4[17][0].x = 37*10;
    valores_4[17][1].y = 16*10;
    valores_4[18][0].x = 36*10;
    valores_4[18][1].y = 20*10;
    valores_4[19][0].x = 37*10;
    valores_4[19][1].y = 22*10;
    valores_4[20][0].x = 38*10;
    valores_4[20][1].y = 26*10;
    valores_4[21][0].x = 34*10;
    valores_4[21][1].y = 26*10;
    valores_4[22][0].x = 32*10;
    valores_4[22][1].y = 27*10;
    //Fin
    // Vertices Cartago
    valores_5[0][0].x = 30*10; 
    valores_5[0][1].y = 28*10;
    valores_5[1][0].x = 29*10;
    valores_5[1][1].y = 26*10;
    valores_5[2][0].x = 27*10;
    valores_5[2][1].y = 24*10;
    valores_5[3][0].x = 28*10;
    valores_5[3][1].y = 22*10;
    valores_5[4][0].x = 31*10;
    valores_5[4][1].y = 20*10;
    valores_5[5][0].x = 33*10;
    valores_5[5][1].y = 19*10;
    valores_5[6][0].x = 35*10;
    valores_5[6][1].y = 18*10;
    valores_5[7][0].x = 36*10;
    valores_5[7][1].y = 20*10;
    valores_5[8][0].x = 37*10;
    valores_5[8][1].y = 22*10;
    valores_5[9][0].x = 38*10;
    valores_5[9][1].y = 26*10;
    valores_5[10][0].x = 34*10;
    valores_5[10][1].y = 26*10;
    valores_5[11][0].x = 32*10;
    valores_5[11][1].y = 27*10;
    //Fin
    // Vertices San Jose
    valores_6[0][0].x = 20*10; 
    valores_6[0][1].y = 24*10;
    valores_6[1][0].x = 21*10;
    valores_6[1][1].y = 20*10;
    valores_6[2][0].x = 23*10;
    valores_6[2][1].y = 21*10;
    valores_6[3][0].x = 27*10;
    valores_6[3][1].y = 20*10;
    valores_6[4][0].x = 30*10;
    valores_6[4][1].y = 17*10;
    valores_6[5][0].x = 35*10;
    valores_6[5][1].y = 13*10;
    valores_6[6][0].x = 37*10;
    valores_6[6][1].y = 16*10;
    valores_6[7][0].x = 36*10;
    valores_6[7][1].y = 20*10;
    valores_6[8][0].x = 35*10;
    valores_6[8][1].y = 18*10;
    valores_6[9][0].x = 33*10;
    valores_6[9][1].y = 19*10;
    valores_6[10][0].x = 31*10;
    valores_6[10][1].y = 20*10;
    valores_6[11][0].x = 28*10;
    valores_6[11][1].y = 22*10;
    valores_6[12][0].x = 27*10;
    valores_6[12][1].y = 24*10;
    valores_6[13][0].x = 29*10;
    valores_6[13][1].y = 26*10;
    valores_6[14][0].x = 30*10;
    valores_6[14][1].y = 28*10;
    valores_6[15][0].x = 30*10;
    valores_6[15][1].y = 29*10;
    valores_6[16][0].x = 28*10;
    valores_6[16][1].y = 29*10;
    valores_6[17][0].x = 28*10;
    valores_6[17][1].y = 26*10;
    valores_6[18][0].x = 25*10;
    valores_6[18][1].y = 25*10;
    //Fin
    // Vertices Puntarenas Costa
    valores_7[0][0].x = 35*10; 
    valores_7[0][1].y = 13*10;
    valores_7[1][0].x = 30*10;
    valores_7[1][1].y = 17*10;
    valores_7[2][0].x = 27*10;
    valores_7[2][1].y = 20*10;
    valores_7[3][0].x = 23*10;
    valores_7[3][1].y = 21*10;
    valores_7[4][0].x = 21*10;
    valores_7[4][1].y = 20*10;
    valores_7[5][0].x = 20*10;
    valores_7[5][1].y = 24*10;
    valores_7[6][0].x = 19*10;
    valores_7[6][1].y = 29*10;
    valores_7[7][0].x = 16*10;
    valores_7[7][1].y = 29*10;
    valores_7[8][0].x = 16*10;
    valores_7[8][1].y = 31*10;
    valores_7[9][0].x = 15*10;
    valores_7[9][1].y = 29*10;
    valores_7[10][0].x = 11*10;
    valores_7[10][1].y = 29*10;
    valores_7[11][0].x = 13*10;
    valores_7[11][1].y = 28*10;
    valores_7[12][0].x = 16*10;
    valores_7[12][1].y = 26*10;
    valores_7[13][0].x = 18*10;
    valores_7[13][1].y = 24*10;
    valores_7[14][0].x = 19*10;
    valores_7[14][1].y = 20*10;
    valores_7[15][0].x = 21*10;
    valores_7[15][1].y = 19*10;
    valores_7[16][0].x = 26*10;
    valores_7[16][1].y = 17*10;
    valores_7[17][0].x = 29*10;
    valores_7[17][1].y = 16*10;
    valores_7[18][0].x = 30*10;
    valores_7[18][1].y = 14*10;
    valores_7[19][0].x = 34*10;
    valores_7[19][1].y = 12*10;
    valores_7[20][0].x = 35*10; 
    valores_7[20][1].y = 8*10;
    valores_7[21][0].x = 34*10;
    valores_7[21][1].y = 7*10;
    valores_7[22][0].x = 33*10;
    valores_7[22][1].y = 4*10;
    valores_7[23][0].x = 35*10;
    valores_7[23][1].y = 2*10;
    valores_7[24][0].x = 38*10;
    valores_7[24][1].y = 2*10;
    valores_7[25][0].x = 36*10;
    valores_7[25][1].y = 5*10;
    valores_7[26][0].x = 38*10;
    valores_7[26][1].y = 7*10;
    valores_7[27][0].x = 40*10;
    valores_7[27][1].y = 5*10;
    valores_7[28][0].x = 42*10;
    valores_7[28][1].y = 2*10;
    valores_7[29][0].x = 41*10;
    valores_7[29][1].y = 1*10;
    valores_7[30][0].x = 42*10;
    valores_7[30][1].y = 1*10;
    valores_7[31][0].x = 45*10;
    valores_7[31][1].y = 3*10;
    valores_7[32][0].x = 44*10;
    valores_7[32][1].y = 7*10;
    valores_7[33][0].x = 46*10;
    valores_7[33][1].y = 10*10;
    valores_7[34][0].x = 44*10;
    valores_7[34][1].y = 12*10;
    valores_7[35][0].x = 41*10;
    valores_7[35][1].y = 14*10;
    valores_7[36][0].x = 39*10;
    valores_7[36][1].y = 16*10;
    valores_7[37][0].x = 37*10;
    valores_7[37][1].y = 16*10;
    //Fin
}

void lineasMapa(){
	
	int i,j;
	// Guanacaste
	glColor3f(0,1,0); //color verde
    //Pinta desde el (x0,y0) al (x1, y1) y asi sucesivamente
	for( j = 0 ; j < (lineas - 1); j++ ) {
		bresenham(valores_0[j][0].x,valores_0[j][1].y,valores_0[(j + 1)][0].x,valores_0[(j + 1)][1].y);
	}

    // llega al ultimo punto de la matriz y pinta una linea desde este hasta (x0, y0)
	if (j == (lineas -1)) {
		bresenham(valores_0[j][0].x,valores_0[j][1].y,valores_0[0][0].x,valores_0[0][1].y);
	}
	glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Puntarenas Peninsula
	glColor3f(1,0.4,0); //Anaranjado

	for( j = 0 ; j < (lineas1 - 1); j++ ) {
		bresenham(valores_1[j][0].x,valores_1[j][1].y,valores_1[(j + 1)][0].x,valores_1[(j + 1)][1].y);
	}

	if (j == (lineas1 -1)) {
		bresenham(valores_1[j][0].x,valores_1[j][1].y,valores_1[0][0].x,valores_1[0][1].y);
	}
	glFlush();

 //--------------------------------------------------------------------------------------------------------- 
    //Alajuela	
	glColor3f(1,0,0); //Rojo 

	for( j = 0 ; j < (lineas3 - 1); j++ ) {
		bresenham(valores_2[j][0].x,valores_2[j][1].y,valores_2[(j + 1)][0].x,valores_2[(j + 1)][1].y);
	}

	if (j == (lineas3 -1)) {
		bresenham(valores_2[j][0].x,valores_2[j][1].y,valores_2[0][0].x,valores_2[0][1].y);
	}
	glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Heredia
	glColor3f(1,1,0); // Amarillo
 
	for( j = 0 ; j < (lineas4 - 1); j++ ) {
		bresenham(valores_3[j][0].x,valores_3[j][1].y,valores_3[(j + 1)][0].x,valores_3[(j + 1)][1].y);
	}

	if (j == (lineas4 -1)) {
		bresenham(valores_3[j][0].x,valores_3[j][1].y,valores_3[0][0].x,valores_3[0][1].y);
	}
	glFlush();

 //--------------------------------------------------------------------------------------------------------- 
    //Limon
	glColor3f(0,1,1); //Cyan
 
	for( j = 0 ; j < (lineas5 - 1); j++ ) {
		bresenham(valores_4[j][0].x,valores_4[j][1].y,valores_4[(j + 1)][0].x,valores_4[(j + 1)][1].y);
	}

	if (j == (lineas5 -1)) {
		bresenham(valores_4[j][0].x,valores_4[j][1].y,valores_4[0][0].x,valores_4[0][1].y);
	}

	glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Cartago
	glColor3f(0,0,1); //Azul
 
	for( j = 0 ; j < (lineas6 - 1); j++ ) {
		bresenham(valores_5[j][0].x,valores_5[j][1].y,valores_5[(j + 1)][0].x,valores_5[(j + 1)][1].y);
	}

	if (j == (lineas6 -1)) {
		bresenham(valores_5[j][0].x,valores_5[j][1].y,valores_5[0][0].x,valores_5[0][1].y);
	}
	glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    // San Jose
	glColor3f(1,1,1); // Blanco
 
	for( j = 0 ; j < (lineas7 - 1); j++ ) {
		bresenham(valores_6[j][0].x,valores_6[j][1].y,valores_6[(j + 1)][0].x,valores_6[(j + 1)][1].y);
	}

	if (j == (lineas7 -1)) {
		bresenham(valores_6[j][0].x,valores_6[j][1].y,valores_6[0][0].x,valores_6[0][1].y);
	}
	glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Puntarenas Costa
	glColor3f(1,0.4,0); //Anaranjado
 
	for( j = 0 ; j < (lineas8 - 1); j++ ) {
		bresenham(valores_7[j][0].x,valores_7[j][1].y,valores_7[(j + 1)][0].x,valores_7[(j + 1)][1].y);
	}

	if (j == (lineas8 -1)) {
		bresenham(valores_7[j][0].x,valores_7[j][1].y,valores_7[0][0].x,valores_7[0][1].y);
	}
	glFlush();
}


int main(int argc,char** argv){

	IniciaMatrices();
	CoordMapas();

	glutInit(&argc, argv);
   	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  	glutInitWindowSize(resolucion+10,resolucion+10);
  	glutCreateWindow("PROG0_GeovannyAstorga_MaurcioCastillo");
  	glClear(GL_COLOR_BUFFER_BIT);
  	glMatrixMode(GL_PROJECTION);
  	gluOrtho2D(-0.5, resolucion +0.5, -0.5, resolucion + 0.5);
  	glutDisplayFunc(lineasMapa);
  	glutMainLoop();
}