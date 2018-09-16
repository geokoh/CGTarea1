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
int Ymax,Xmax,Xmin,Ymin;

int flag = 1;

Color Guanacaste,Alajuela,Puntarenas,Heredia;
Color Limon,Cartago,SanJose;

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

Matriz **valores_0p;
Matriz **valores_1p;
Matriz **valores_2p;
Matriz **valores_3p;
Matriz **valores_4p;
Matriz **valores_5p;
Matriz **valores_6p;
Matriz **valores_7p;

Matriz **clipper;

MatrizOp **traslado;

/* Ints para la cantidad de vertices
** lineas: Guanacaste lineas2: Puntarenas Peninsula
** lineas3: Alajuela lineas4: Heredia
** lineas5: Limon lineas6: Cartago
** lineas7: San Jose lineas 8: Puntarenas Costa
*/
int lineas, lineas2, lineas3, lineas4,lineas5, lineas6, lineas7, lineas8;
int lineas_p, lineas2_p, lineas3_p, lineas4_p,lineas5_p, lineas6_p, lineas7_p, lineas8_p;

int INSIDE = 0;
int LEFT = 1;
int RIGHT = 2;
int BOTTOM = 4;
int TOP = 8;

int y_max;
int x_max;
int y_min;
int x_min;

//File *Archivo;

EdgeTableTuple EdgeTable[maxWD], ActiveEdgeTuple;



void plot(int x, int y){
	// Representa el pixel
		glBegin(GL_POINTS);
		glVertex2i(x,y);
		glEnd();
}

void plot_(int x0,int y0,int x1, int y1){
    glBegin(GL_LINES);
    glVertex2i(x0,y0);
    glVertex2i(x1,y1);
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

void sort(EdgeTableTuple *entrada){
    int i,j;
    EdgeBucket temp;

    for(i=1; i < entrada->countEdgeBucket; i++){
        temp.ymax = entrada->buckets[i].ymax;
        temp.xofymin = entrada->buckets[i].xofymin;
        temp.slopeinverse = entrada->buckets[i].slopeinverse;

        j = i- 1;

        while ((temp.xofymin < entrada->buckets[j].xofymin) && (j>=0)){
            entrada->buckets[j+1].ymax = entrada->buckets[j].ymax;
            entrada->buckets[j+1].xofymin = entrada->buckets[j].xofymin;
            entrada->buckets[j+1].slopeinverse = entrada->buckets[j].slopeinverse;
            j= j-1;
        }
        entrada->buckets[j+1].ymax = temp.ymax;
        entrada->buckets[j+1].xofymin = temp.xofymin;
        entrada->buckets[j+1].slopeinverse = temp.slopeinverse;
    }
}

void almacenaTupla(EdgeTableTuple *entrada,int ymaxAlmacena,int xMinAlmacena,float min){
    (entrada->buckets[(entrada)->countEdgeBucket]).ymax = ymaxAlmacena;
    (entrada->buckets[(entrada)->countEdgeBucket]).xofymin = (float)xMinAlmacena;
    (entrada->buckets[(entrada)->countEdgeBucket]).slopeinverse = min;

    sort(entrada);
    (entrada->countEdgeBucket)++;
}

void almacenaPuntoTabla(int x0,int y0,int x1 ,int y1){
    float m, min;
    int ymaxAlmacena, xMinAlmacena, scaline;

    if (x1==x0){
        min = 0.000000;
    }else{
        m = ((float)(y1-y0))/((float)(x1-x0));

        if (y1==y0)
            return ;

        min = (float)1.0/m;
    }

    if (y0>y1){
        //almacenaTupla(&EdgeTable[y1],y1,x1,min);
        scaline = y1;
        ymaxAlmacena = y0;
        xMinAlmacena = x1;
    }else{
        //almacenaTupla(&EdgeTable[y0],y1,x0,min);
        scaline = y0;
        ymaxAlmacena = y1;
        xMinAlmacena = x0;
    }

    almacenaTupla(&EdgeTable[scaline],ymaxAlmacena,xMinAlmacena,min);
}

void EliminarEje(EdgeTableTuple *Tupla, int yy){
    int i,j;
    for (i=0; i < Tupla->countEdgeBucket;i++){
        if(Tupla->buckets[i].ymax == yy){

            //printf("\nRemoved at %d\n", yy );

            for (j = i; j < Tupla->countEdgeBucket -1;j++){
                Tupla->buckets[j].ymax =Tupla->buckets[j+1].ymax;
                Tupla->buckets[j].xofymin =Tupla->buckets[j+1].xofymin;
                Tupla->buckets[j].slopeinverse = Tupla->buckets[j+1].slopeinverse;
            }
            Tupla->countEdgeBucket--;
            i--;
        }
    }
}

void actualizaSlope(EdgeTableTuple *Tupla){
    int i;
    for (i=0; i<Tupla->countEdgeBucket;i++){
        (Tupla->buckets[i]).xofymin = (Tupla->buckets[i]).xofymin + (Tupla->buckets[i]).slopeinverse;
    }
}

void scaline(){
    int i, j, x0,ymax0,x1,ymax1, Flag = 0, contador;

    for (i = 0; i < maxHT; i++){
        for(j=0; j < EdgeTable[i].countEdgeBucket;j++){
            almacenaTupla(&ActiveEdgeTuple,EdgeTable[i].buckets[j].ymax,
                EdgeTable[i].buckets[j].xofymin,EdgeTable[i].buckets[j].slopeinverse);
        }

        EliminarEje(&ActiveEdgeTuple,i);

        sort(&ActiveEdgeTuple);

        j = 0;
        Flag = 0;
        contador = 0;
        x0 = 0;
        x1 = 0;
        ymax0 = 0;
        ymax1 = 0;

        while (j<ActiveEdgeTuple.countEdgeBucket){
            if(contador%2==0){
                x0 = (int)(ActiveEdgeTuple.buckets[j].xofymin);
                ymax0 = ActiveEdgeTuple.buckets[j].ymax;

                if (x0==x1){
                    if (((x0==ymax0)&&(x1!=ymax1))||((x0!=ymax0)&&(x1==ymax1))){
                        x1=x0;
                        ymax1=ymax0;
                    }else{
                        contador++;
                    }
                }else{
                    contador++;
                }
            }else{
                x1 = (int)ActiveEdgeTuple.buckets[j].xofymin;
                ymax1 = ActiveEdgeTuple.buckets[j].ymax;

                Flag = 0;

                if (x0 == x1){
                    if (((x0==ymax0)&&(x1!=ymax1))||((x0!=ymax0)&&(x1==ymax1))){
                        x0 = x1;
                        ymax0 = ymax1;
                    }else{
                        contador++;
                        Flag = 1;
                    }
                }else{
                    contador++;
                    Flag =1;
                }

                if(Flag){
                    plot_(x0,i,x1,i);
                }
            }
            j++;
        }
        actualizaSlope(&ActiveEdgeTuple);
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

void aplicarZoom() {
    int i;
    int Xc = CentrarX();
    int Yc = CentrarY();

    for (i = 0; i < lineas; i++) {
        valores_0[i][0].x = (-Xc + ((Xc + valores_0[i][0].x) * traslado[0][0].valor));
        valores_0[i][1].y = (-Yc + ((Yc + valores_0[i][1].y) * traslado[1][1].valor));

    }

    for (i = 0; i < lineas2; i++) {
        valores_1[i][0].x = (-Xc + ((Xc + valores_1[i][0].x) * traslado[0][0].valor));
        valores_1[i][1].y = (-Yc + ((Yc + valores_1[i][1].y) * traslado[1][1].valor));

    }

    for (i = 0; i < lineas3; i++) {
        valores_2[i][0].x = (-Xc + ((Xc + valores_2[i][0].x) * traslado[0][0].valor));
        valores_2[i][1].y = (-Yc + ((Yc + valores_2[i][1].y) * traslado[1][1].valor));

    }

    for (i = 0; i < lineas4; i++) {
        valores_3[i][0].x = (-Xc + ((Xc + valores_3[i][0].x) * traslado[0][0].valor));
        valores_3[i][1].y = (-Yc + ((Yc + valores_3[i][1].y) * traslado[1][1].valor));

    }

    for (i = 0; i < lineas5; i++) {
        valores_4[i][0].x = (-Xc + ((Xc + valores_4[i][0].x) * traslado[0][0].valor));
        valores_4[i][1].y = (-Yc + ((Yc + valores_4[i][1].y) * traslado[1][1].valor));

    }

    for (i = 0; i < lineas6; i++) {
        valores_5[i][0].x = (-Xc + ((Xc + valores_5[i][0].x) * traslado[0][0].valor));
        valores_5[i][1].y = (-Yc + ((Yc + valores_5[i][1].y) * traslado[1][1].valor));

    }

    for (i = 0; i < lineas7; i++) {
        valores_6[i][0].x = (-Xc + ((Xc + valores_6[i][0].x) * traslado[0][0].valor));
        valores_6[i][1].y = (-Yc + ((Yc + valores_6[i][1].y) * traslado[1][1].valor));

    }

    for (i = 0; i < lineas8; i++) {
        valores_7[i][0].x = (-Xc + ((Xc + valores_7[i][0].x) * traslado[0][0].valor));
        valores_7[i][1].y = (-Yc + ((Yc + valores_7[i][1].y) * traslado[1][1].valor));

    }

}

MatrizOp** Zoom(double Sx,double Sy){

	
   for(int i = 0; i < 3; i++) {
   		for(int j=0;j<3; j++){
   			if(i==j){
   				if(j==0){
   					traslado[i][j].valor = Sx;
   				}
   				if(j==1){
   					traslado[i][j].valor = Sy;
   				}
   				if(j==2){
   					traslado[i][j].valor = 1;
   				}
   			}else{
   				traslado[i][j].valor = 0;
   			}
   		}
   }
   return traslado;
}

void aplicarPan() {
    int i;

    for (i = 0; i < lineas; i++) {
        valores_0[i][0].x = valores_0[i][0].x + traslado[0][2].valor;
        valores_0[i][1].y = valores_0[i][1].y + traslado[1][2].valor;

    }

    for (i = 0; i < lineas2; i++) {
        valores_1[i][0].x = valores_1[i][0].x + traslado[0][2].valor;
        valores_1[i][1].y = valores_1[i][1].y + traslado[1][2].valor;

    }

    for (i = 0; i < lineas3; i++) {
        valores_2[i][0].x = valores_2[i][0].x + traslado[0][2].valor;
        valores_2[i][1].y = valores_2[i][1].y + traslado[1][2].valor;

    }

    for (i = 0; i < lineas4; i++) {
        valores_3[i][0].x = valores_3[i][0].x + traslado[0][2].valor;
        valores_3[i][1].y = valores_3[i][1].y + traslado[1][2].valor;

    }

    for (i = 0; i < lineas5; i++) {
        valores_4[i][0].x = valores_4[i][0].x + traslado[0][2].valor;
        valores_4[i][1].y = valores_4[i][1].y + traslado[1][2].valor;

    }

    for (i = 0; i < lineas6; i++) {
        valores_5[i][0].x = valores_5[i][0].x + traslado[0][2].valor;
        valores_5[i][1].y = valores_5[i][1].y + traslado[1][2].valor;

    }

    for (i = 0; i < lineas7; i++) {
        valores_6[i][0].x = valores_6[i][0].x + traslado[0][2].valor;
        valores_6[i][1].y = valores_6[i][1].y + traslado[1][2].valor;

    }

    for (i = 0; i < lineas8; i++) {
        valores_7[i][0].x = valores_7[i][0].x + traslado[0][2].valor;
        valores_7[i][1].y = valores_7[i][1].y + traslado[1][2].valor;

    }

}

void Pan(int delta_x,int delta_y){

	//Inicialización de la matriz de operaciones

    for(int i = 0; i < 3; i++) {
   		for(int j=0;j<3; j++){
   			if(i==j){
   				//Se agregan los valores de la diagonal
				traslado[i][j].valor = 1;
   			}else{
   				if((j==2) && (i==0)){
   					traslado[i][j].valor = delta_x;
   				}else{
   					if((j==2) && (i==1)){
   						traslado[i][j].valor = delta_y;
   					}else{
   						traslado[i][j].valor = 0;
   					}
   				}
   			}
   		}
   }

   
}


void aplicarRotarLeft() {
    int i;

    for (i = 0; i < lineas; i++) {
        valores_0[i][0].x = (valores_0[i][0].x * traslado[0][0].valor) + (valores_0[i][1].y * traslado[0][1].valor);
        valores_0[i][1].y = (valores_0[i][0].x * traslado[1][0].valor) + (valores_0[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas2; i++) {
        valores_1[i][0].x = (valores_1[i][0].x * traslado[0][0].valor) + (valores_1[i][1].y * traslado[0][1].valor);
        valores_1[i][1].y = (valores_1[i][0].x * traslado[1][0].valor) + (valores_1[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas3; i++) {
        valores_2[i][0].x = (valores_2[i][0].x * traslado[0][0].valor) + (valores_2[i][1].y * traslado[0][1].valor);
        valores_2[i][1].y = (valores_2[i][0].x * traslado[1][0].valor) + (valores_2[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas4; i++) {
        valores_3[i][0].x = (valores_3[i][0].x * traslado[0][0].valor) + (valores_3[i][1].y * traslado[0][1].valor);
        valores_3[i][1].y = (valores_3[i][0].x * traslado[1][0].valor) + (valores_3[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas5; i++) {
        valores_4[i][0].x = (valores_4[i][0].x * traslado[0][0].valor) + (valores_4[i][1].y * traslado[0][1].valor);
        valores_4[i][1].y = (valores_4[i][0].x * traslado[1][0].valor) + (valores_4[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas6; i++) {
        valores_5[i][0].x = (valores_5[i][0].x * traslado[0][0].valor) + (valores_5[i][1].y * traslado[0][1].valor);
        valores_5[i][1].y = (valores_5[i][0].x * traslado[1][0].valor) + (valores_5[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas7; i++) {
        valores_6[i][0].x = (valores_6[i][0].x * traslado[0][0].valor) + (valores_6[i][1].y * traslado[0][1].valor);
        valores_6[i][1].y = (valores_6[i][0].x * traslado[1][0].valor) + (valores_6[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas8; i++) {
        valores_7[i][0].x = (valores_7[i][0].x * traslado[0][0].valor) + (valores_7[i][1].y * traslado[0][1].valor);
        valores_7[i][1].y = (valores_7[i][0].x * traslado[1][0].valor) + (valores_7[i][1].y * traslado[1][1].valor);
    }

}
void RotateLeft(double alfa){


	//Valores de rotación con cos y sin 
	traslado[0][0].valor = cos(alfa);
	traslado[0][1].valor = -1*sin(alfa);
	traslado[1][0].valor = sin(alfa);
	traslado[1][1].valor = cos(alfa);
	
	//Ceros de la matriz para la coordenadas homogeneas
	traslado[0][2].valor = 0;
	traslado[1][2].valor = 0;
	traslado[2][0].valor = 0;
	traslado[2][1].valor = 0;
	traslado[2][2].valor = 1;
}

void aplicarRotarRight() {
    int i;

    for (i = 0; i < lineas; i++) {
        valores_0[i][0].x = (valores_0[i][0].x * traslado[0][0].valor) + (valores_0[i][1].y * traslado[0][1].valor);
        valores_0[i][1].y = (valores_0[i][0].x * traslado[1][0].valor) + (valores_0[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas2; i++) {
        valores_1[i][0].x = (valores_1[i][0].x * traslado[0][0].valor) + (valores_1[i][1].y * traslado[0][1].valor);
        valores_1[i][1].y = (valores_1[i][0].x * traslado[1][0].valor) + (valores_1[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas3; i++) {
        valores_2[i][0].x = (valores_2[i][0].x * traslado[0][0].valor) + (valores_2[i][1].y * traslado[0][1].valor);
        valores_2[i][1].y = (valores_2[i][0].x * traslado[1][0].valor) + (valores_2[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas4; i++) {
        valores_3[i][0].x = (valores_3[i][0].x * traslado[0][0].valor) + (valores_3[i][1].y * traslado[0][1].valor);
        valores_3[i][1].y = (valores_3[i][0].x * traslado[1][0].valor) + (valores_3[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas5; i++) {
        valores_4[i][0].x = (valores_4[i][0].x * traslado[0][0].valor) + (valores_4[i][1].y * traslado[0][1].valor);
        valores_4[i][1].y = (valores_4[i][0].x * traslado[1][0].valor) + (valores_4[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas6; i++) {
        valores_5[i][0].x = (valores_5[i][0].x * traslado[0][0].valor) + (valores_5[i][1].y * traslado[0][1].valor);
        valores_5[i][1].y = (valores_5[i][0].x * traslado[1][0].valor) + (valores_5[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas7; i++) {
        valores_6[i][0].x = (valores_6[i][0].x * traslado[0][0].valor) + (valores_6[i][1].y * traslado[0][1].valor);
        valores_6[i][1].y = (valores_6[i][0].x * traslado[1][0].valor) + (valores_6[i][1].y * traslado[1][1].valor);

    }

    for (i = 0; i < lineas8; i++) {
        valores_7[i][0].x = (valores_7[i][0].x * traslado[0][0].valor) + (valores_7[i][1].y * traslado[0][1].valor);
        valores_7[i][1].y = (valores_7[i][0].x * traslado[1][0].valor) + (valores_7[i][1].y * traslado[1][1].valor);
    }

}

void RotateRight(double alfa){

	//Valores de rotación con cos y sin 
	traslado[0][0].valor = cos(alfa);
	traslado[0][1].valor = -1*sin(alfa);
	traslado[1][0].valor = sin(alfa);
	traslado[1][1].valor = cos(alfa);

	//Ceros de la matriz para la coordenadas homogeneas
	traslado[0][2].valor = 0;
	traslado[1][2].valor = 0;
	traslado[2][0].valor = 0;
	traslado[2][1].valor = 0;
	traslado[2][2].valor = 1;
}

void IniciaLineas() {
    lineas = 23;
    lineas2 = 8;
    lineas3 = 19;
    lineas4 = 9;
    lineas5 = 23;
    lineas6 = 12;
    lineas7 = 19;
    lineas8 = 38;

    lineas_p = 23;
    lineas2_p = 8;
    lineas3_p = 19;
    lineas4_p = 9;
    lineas5_p = 23;
    lineas6_p = 12;
    lineas7_p = 19;
    lineas8_p = 38;
}

void IniciaMatrices(){

	/* Se encarga de la incializacion de las matrices que se utilicen en el código*/

	//la cantidad de vertices por provincia

    int i,j;

    //Inicializa las matrices
    valores_0 = (Matriz **)malloc(lineas  * sizeof(Matriz*));
	valores_1 = (Matriz **)malloc(lineas2 * sizeof(Matriz*));
	valores_2 = (Matriz **)malloc(lineas3 * sizeof(Matriz*));
	valores_3 = (Matriz **)malloc(lineas4 * sizeof(Matriz*));
	valores_4 = (Matriz **)malloc(lineas5 * sizeof(Matriz*));
	valores_5 = (Matriz **)malloc(lineas6 * sizeof(Matriz*));
	valores_6 = (Matriz **)malloc(lineas7 * sizeof(Matriz*));
	valores_7 = (Matriz **)malloc(lineas8 * sizeof(Matriz*));

	valores_0p = (Matriz **)malloc(50  * sizeof(Matriz*));
    valores_1p = (Matriz **)malloc(50 * sizeof(Matriz*));
    valores_2p = (Matriz **)malloc(50 * sizeof(Matriz*));
    valores_3p = (Matriz **)malloc(50 * sizeof(Matriz*));
    valores_4p = (Matriz **)malloc(50 * sizeof(Matriz*));
    valores_5p = (Matriz **)malloc(50 * sizeof(Matriz*));
    valores_6p = (Matriz **)malloc(50 * sizeof(Matriz*));
    valores_7p = (Matriz **)malloc(50 * sizeof(Matriz*));
    clipper = (Matriz **)malloc(50 * sizeof(Matriz*));
    //Agrega las filas
    for (i = 0; i < 50; i++) {valores_0p[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < 50; i++){valores_1p[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < 50; i++){valores_2p[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < 50; i++){valores_3p[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < 50; i++){valores_4p[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < 50; i++){valores_5p[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < 50; i++){valores_6p[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < 50; i++){valores_7p[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < 50; i++){clipper[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}

    for (i = 0; i < lineas; i++) {valores_0[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < lineas2; i++){valores_1[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < lineas3; i++){valores_2[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < lineas4; i++){valores_3[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < lineas5; i++){valores_4[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < lineas6; i++){valores_5[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < lineas7; i++){valores_6[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}
    for (i = 0; i < lineas8; i++){valores_7[i] = (Matriz *)malloc(lineas * sizeof(Matriz));}

    traslado = (MatrizOp **)malloc(3 * sizeof(MatrizOp*));
    for (int i = 0; i < 3; i++){
       traslado[i] = (MatrizOp *)malloc(3 * sizeof(MatrizOp));
    }   

    for (i = 0; i < maxHT; i++){EdgeTable[i].countEdgeBucket=0;}
    ActiveEdgeTuple.countEdgeBucket=0;
}

void clear(){
    for (int i = 0; i < maxHT; i++){EdgeTable[i].countEdgeBucket=0;}
    ActiveEdgeTuple.countEdgeBucket=0;
}

int CentrarX(){
    return -(valores_2[11][0].x);
}

int CentrarY(){
    return -(valores_2[11][1].y);
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

    //pintar
    valores_0p[0][0].x = 5*10; 
    valores_0p[0][1].y = 43*10;
    valores_0p[1][0].x = 3*10;
    valores_0p[1][1].y = 41*10;
    valores_0p[2][0].x = 1*10;
    valores_0p[2][1].y = 39*10;
    valores_0p[3][0].x = 2*10;
    valores_0p[3][1].y = 38*10;
    valores_0p[4][0].x = 2*10;
    valores_0p[4][1].y = 33*10;
    valores_0p[5][0].x = 1*10;
    valores_0p[5][1].y = 31*10;
    valores_0p[6][0].x = 2*10;
    valores_0p[6][1].y = 28*10;
    valores_0p[7][0].x = 4*10;
    valores_0p[7][1].y = 25*10;
    valores_0p[8][0].x = 7*10;
    valores_0p[8][1].y = 24*10;
    valores_0p[9][0].x = 10*10;
    valores_0p[9][1].y = 23*10;
    valores_0p[10][0].x = 9*10;
    valores_0p[10][1].y = 25*10;
    valores_0p[11][0].x = 10*10;
    valores_0p[11][1].y = 27*10;
    valores_0p[12][0].x = 9*10;
    valores_0p[12][1].y = 30*10;
    valores_0p[13][0].x = 11*10;
    valores_0p[13][1].y = 29*10;
    valores_0p[14][0].x = 15*10;
    valores_0p[14][1].y = 29*10;
    valores_0p[15][0].x = 16*10;
    valores_0p[15][1].y = 31*10;
    valores_0p[16][0].x = 15*10;
    valores_0p[16][1].y = 33*10;
    valores_0p[17][0].x = 13*10;
    valores_0p[17][1].y = 35*10;
    valores_0p[18][0].x = 11*10;
    valores_0p[18][1].y = 36*10;
    valores_0p[19][0].x = 9*10;
    valores_0p[19][1].y = 36*10;
    valores_0p[20][0].x = 7*10;
    valores_0p[20][1].y = 39*10;
    valores_0p[21][0].x = 9*10;
    valores_0p[21][1].y = 42*10;
    valores_0p[22][0].x = 6*10;
    valores_0p[22][1].y = 42*10;
    //valores_0[23][0].x = 5*10; 
    //valores_0[23][1].y = 43*10;
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

    valores_1p[0][0].x = 10*10; 
    valores_1p[0][1].y = 23*10;
    valores_1p[1][0].x = 11*10;
    valores_1p[1][1].y = 21*10;
    valores_1p[2][0].x = 12*10;
    valores_1p[2][1].y = 20*10;
    valores_1p[3][0].x = 14*10;
    valores_1p[3][1].y = 22*10;
    valores_1p[4][0].x = 16*10;
    valores_1p[4][1].y = 23*10;
    valores_1p[5][0].x = 15*10;
    valores_1p[5][1].y = 25*10;
    valores_1p[6][0].x = 10*10;
    valores_1p[6][1].y = 27*10;
    valores_1p[7][0].x = 9*10;
    valores_1p[7][1].y = 25*10;

    //pintar

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

    //pintar
    valores_2p[0][0].x = 11*10; 
    valores_2p[0][1].y = 41*10;
    valores_2p[1][0].x = 9*10;
    valores_2p[1][1].y = 42*10;
    valores_2p[2][0].x = 7*10;
    valores_2p[2][1].y = 39*10;
    valores_2p[3][0].x = 9*10;
    valores_2p[3][1].y = 36*10;
    valores_2p[4][0].x = 11*10;
    valores_2p[4][1].y = 36*10;
    valores_2p[5][0].x = 13*10;
    valores_2p[5][1].y = 35*10;
    valores_2p[6][0].x = 15*10;
    valores_2p[6][1].y = 33*10;
    valores_2p[7][0].x = 16*10;
    valores_2p[7][1].y = 31*10;
    valores_2p[8][0].x = 16*10;
    valores_2p[8][1].y = 29*10;
    valores_2p[9][0].x = 19*10;
    valores_2p[9][1].y = 29*10;
    valores_2p[10][0].x = 20*10;
    valores_2p[10][1].y = 24*10;
    valores_2p[11][0].x = 25*10;
    valores_2p[11][1].y = 25*10;
    valores_2p[12][0].x = 25*10;
    valores_2p[12][1].y = 37*10;
    valores_2p[13][0].x = 24*10;
    valores_2p[13][1].y = 39*10;
    valores_2p[14][0].x = 23*10;
    valores_2p[14][1].y = 40*10;
    valores_2p[15][0].x = 20*10;
    valores_2p[15][1].y = 40*10;
    valores_2p[16][0].x = 18*10;
    valores_2p[16][1].y = 41*10;
    valores_2p[17][0].x = 16*10;
    valores_2p[17][1].y = 40*10;
    valores_2p[18][0].x = 15*10;
    valores_2p[18][1].y = 39*10;

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

    //pintar
    valores_3p[0][0].x = 25*10; 
    valores_3p[0][1].y = 37*10;
    valores_3p[1][0].x = 25*10;
    valores_3p[1][1].y = 25*10;
    valores_3p[2][0].x = 28*10;
    valores_3p[2][1].y = 26*10;
    valores_3p[3][0].x = 28*10;
    valores_3p[3][1].y = 29*10;
    valores_3p[4][0].x = 30*10;
    valores_3p[4][1].y = 29*10;
    valores_3p[5][0].x = 30*10;
    valores_3p[5][1].y = 32*10;
    valores_3p[6][0].x = 32*10;
    valores_3p[6][1].y = 37*10;
    valores_3p[7][0].x = 30*10;
    valores_3p[7][1].y = 36*10;
    valores_3p[8][0].x = 28*10;
    valores_3p[8][1].y = 37*10;

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

    //pintar

    valores_4p[0][0].x = 30*10; 
    valores_4p[0][1].y = 28*10;
    valores_4p[1][0].x = 30*10;
    valores_4p[1][1].y = 29*10;
    valores_4p[2][0].x = 30*10;
    valores_4p[2][1].y = 32*10;
    valores_4p[3][0].x = 32*10;
    valores_4p[3][1].y = 37*10;
    valores_4p[4][0].x = 34*10;
    valores_4p[4][1].y = 36*10;
    valores_4p[5][0].x = 36*10;
    valores_4p[5][1].y = 35*10;
    valores_4p[6][0].x = 37*10;
    valores_4p[6][1].y = 33*10;
    valores_4p[7][0].x = 40*10;
    valores_4p[7][1].y = 31*10;
    valores_4p[8][0].x = 45*10;
    valores_4p[8][1].y = 24*10;
    valores_4p[9][0].x = 49*10;
    valores_4p[9][1].y = 19*10;
    valores_4p[10][0].x = 51*10;
    valores_4p[10][1].y = 18*10;
    valores_4p[11][0].x = 48*10;
    valores_4p[11][1].y = 19*10;
    valores_4p[12][0].x = 47*10;
    valores_4p[12][1].y = 17*10;
    valores_4p[13][0].x = 46*10;
    valores_4p[13][1].y = 10*10;
    valores_4p[14][0].x = 44*10;
    valores_4p[14][1].y = 12*10;
    valores_4p[15][0].x = 41*10;
    valores_4p[15][1].y = 14*10;
    valores_4p[16][0].x = 39*10;
    valores_4p[16][1].y = 16*10;
    valores_4p[17][0].x = 37*10;
    valores_4p[17][1].y = 16*10;
    valores_4p[18][0].x = 36*10;
    valores_4p[18][1].y = 20*10;
    valores_4p[19][0].x = 37*10;
    valores_4p[19][1].y = 22*10;
    valores_4p[20][0].x = 38*10;
    valores_4p[20][1].y = 26*10;
    valores_4p[21][0].x = 34*10;
    valores_4p[21][1].y = 26*10;
    valores_4p[22][0].x = 32*10;
    valores_4p[22][1].y = 27*10;

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

    //pintar
    valores_5p[0][0].x = 30*10; 
    valores_5p[0][1].y = 28*10;
    valores_5p[1][0].x = 29*10;
    valores_5p[1][1].y = 26*10;
    valores_5p[2][0].x = 27*10;
    valores_5p[2][1].y = 24*10;
    valores_5p[3][0].x = 28*10;
    valores_5p[3][1].y = 22*10;
    valores_5p[4][0].x = 31*10;
    valores_5p[4][1].y = 20*10;
    valores_5p[5][0].x = 33*10;
    valores_5p[5][1].y = 19*10;
    valores_5p[6][0].x = 35*10;
    valores_5p[6][1].y = 18*10;
    valores_5p[7][0].x = 36*10;
    valores_5p[7][1].y = 20*10;
    valores_5p[8][0].x = 37*10;
    valores_5p[8][1].y = 22*10;
    valores_5p[9][0].x = 38*10;
    valores_5p[9][1].y = 26*10;
    valores_5p[10][0].x = 34*10;
    valores_5p[10][1].y = 26*10;
    valores_5p[11][0].x = 32*10;
    valores_5p[11][1].y = 27*10;
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

    //pintar
    valores_6p[0][0].x = 20*10; 
    valores_6p[0][1].y = 24*10;
    valores_6p[1][0].x = 21*10;
    valores_6p[1][1].y = 20*10;
    valores_6p[2][0].x = 23*10;
    valores_6p[2][1].y = 21*10;
    valores_6p[3][0].x = 27*10;
    valores_6p[3][1].y = 20*10;
    valores_6p[4][0].x = 30*10;
    valores_6p[4][1].y = 17*10;
    valores_6p[5][0].x = 35*10;
    valores_6p[5][1].y = 13*10;
    valores_6p[6][0].x = 37*10;
    valores_6p[6][1].y = 16*10;
    valores_6p[7][0].x = 36*10;
    valores_6p[7][1].y = 20*10;
    valores_6p[8][0].x = 35*10;
    valores_6p[8][1].y = 18*10;
    valores_6p[9][0].x = 33*10;
    valores_6p[9][1].y = 19*10;
    valores_6p[10][0].x = 31*10;
    valores_6p[10][1].y = 20*10;
    valores_6p[11][0].x = 28*10;
    valores_6p[11][1].y = 22*10;
    valores_6p[12][0].x = 27*10;
    valores_6p[12][1].y = 24*10;
    valores_6p[13][0].x = 29*10;
    valores_6p[13][1].y = 26*10;
    valores_6p[14][0].x = 30*10;
    valores_6p[14][1].y = 28*10;
    valores_6p[15][0].x = 30*10;
    valores_6p[15][1].y = 29*10;
    valores_6p[16][0].x = 28*10;
    valores_6p[16][1].y = 29*10;
    valores_6p[17][0].x = 28*10;
    valores_6p[17][1].y = 26*10;
    valores_6p[18][0].x = 25*10;
    valores_6p[18][1].y = 25*10;

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

    //pintar
    valores_7p[0][0].x = 35*10; 
    valores_7p[0][1].y = 13*10;
    valores_7p[1][0].x = 30*10;
    valores_7p[1][1].y = 17*10;
    valores_7p[2][0].x = 27*10;
    valores_7p[2][1].y = 20*10;
    valores_7p[3][0].x = 23*10;
    valores_7p[3][1].y = 21*10;
    valores_7p[4][0].x = 21*10;
    valores_7p[4][1].y = 20*10;
    valores_7p[5][0].x = 20*10;
    valores_7p[5][1].y = 24*10;
    valores_7p[6][0].x = 19*10;
    valores_7p[6][1].y = 29*10;
    valores_7p[7][0].x = 16*10;
    valores_7p[7][1].y = 29*10;
    valores_7p[8][0].x = 16*10;
    valores_7p[8][1].y = 31*10;
    valores_7p[9][0].x = 15*10;
    valores_7p[9][1].y = 29*10;
    valores_7p[10][0].x = 11*10;
    valores_7p[10][1].y = 29*10;
    valores_7p[11][0].x = 13*10;
    valores_7p[11][1].y = 28*10;
    valores_7p[12][0].x = 16*10;
    valores_7p[12][1].y = 26*10;
    valores_7p[13][0].x = 18*10;
    valores_7p[13][1].y = 24*10;
    valores_7p[14][0].x = 19*10;
    valores_7p[14][1].y = 20*10;
    valores_7p[15][0].x = 21*10;
    valores_7p[15][1].y = 19*10;
    valores_7p[16][0].x = 26*10;
    valores_7p[16][1].y = 17*10;
    valores_7p[17][0].x = 29*10;
    valores_7p[17][1].y = 16*10;
    valores_7p[18][0].x = 30*10;
    valores_7p[18][1].y = 14*10;
    valores_7p[19][0].x = 34*10;
    valores_7p[19][1].y = 12*10;
    valores_7p[20][0].x = 35*10; 
    valores_7p[20][1].y = 8*10;
    valores_7p[21][0].x = 34*10;
    valores_7p[21][1].y = 7*10;
    valores_7p[22][0].x = 33*10;
    valores_7p[22][1].y = 4*10;
    valores_7p[23][0].x = 35*10;
    valores_7p[23][1].y = 2*10;
    valores_7p[24][0].x = 38*10;
    valores_7p[24][1].y = 2*10;
    valores_7p[25][0].x = 36*10;
    valores_7p[25][1].y = 5*10;
    valores_7p[26][0].x = 38*10;
    valores_7p[26][1].y = 7*10;
    valores_7p[27][0].x = 40*10;
    valores_7p[27][1].y = 5*10;
    valores_7p[28][0].x = 42*10;
    valores_7p[28][1].y = 2*10;
    valores_7p[29][0].x = 41*10;
    valores_7p[29][1].y = 1*10;
    valores_7p[30][0].x = 42*10;
    valores_7p[30][1].y = 1*10;
    valores_7p[31][0].x = 45*10;
    valores_7p[31][1].y = 3*10;
    valores_7p[32][0].x = 44*10;
    valores_7p[32][1].y = 7*10;
    valores_7p[33][0].x = 46*10;
    valores_7p[33][1].y = 10*10;
    valores_7p[34][0].x = 44*10;
    valores_7p[34][1].y = 12*10;
    valores_7p[35][0].x = 41*10;
    valores_7p[35][1].y = 14*10;
    valores_7p[36][0].x = 39*10;
    valores_7p[36][1].y = 16*10;
    valores_7p[37][0].x = 37*10;
    valores_7p[37][1].y = 16*10;
}

void setColores(){
    Guanacaste.r=0;Guanacaste.g=1;Guanacaste.b=0;
    Puntarenas.r=1;Puntarenas.g=0.4;Puntarenas.b=0;
    Alajuela.r=1;Alajuela.g=0;Alajuela.b=0;
    Heredia.r=1;Heredia.g=1;Heredia.b=0;
    Limon.r=0;Limon.g=1;Limon.b=1;
    Cartago.r=0;Cartago.g=0;Cartago.b=1;
    SanJose.r=1;SanJose.g=1;SanJose.b=1;
}

void setColor(Color color){
    glColor3f(color.r,color.g,color.b);
}

void relleno(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (flag){
        int i,j;

        // Guanacaste
        setColor(Guanacaste);
        clear();
        for( j = 0 ; j < (lineas_p - 1); j++ ) {        
            almacenaPuntoTabla(valores_0p[j][0].x,valores_0p[j][1].y,valores_0p[(j + 1)][0].x,valores_0p[(j + 1)][1].y);
        }
        if (j == (lineas_p -1)) {
            almacenaPuntoTabla(valores_0p[j][0].x,valores_0p[j][1].y,valores_0p[0][0].x,valores_0p[0][1].y);
        }
        scaline();
        
        glFlush();
     //--------------------------------------------------------------------------------------------------------- 
        //Puntarenas Peninsula
        setColor(Puntarenas);//Anaranjado
        clear();
        for( j = 0 ; j < (lineas2_p - 1); j++ ) {
            almacenaPuntoTabla(valores_1p[j][0].x,valores_1p[j][1].y,valores_1p[(j + 1)][0].x,valores_1p[(j + 1)][1].y);
        }
        if (j == (lineas2_p -1)) {
            almacenaPuntoTabla(valores_1p[j][0].x,valores_1p[j][1].y,valores_1p[0][0].x,valores_1p[0][1].y);
        }
        scaline();
        glFlush();
     //--------------------------------------------------------------------------------------------------------- 
        //Alajuela  
        setColor(Alajuela); //Rojo 
        clear();
        for( j = 0 ; j < (lineas3_p - 1); j++ ) {
            almacenaPuntoTabla(valores_2p[j][0].x,valores_2p[j][1].y,valores_2p[(j + 1)][0].x,valores_2p[(j + 1)][1].y);
        }
        if (j == (lineas3_p -1)) {
            almacenaPuntoTabla(valores_2p[j][0].x,valores_2p[j][1].y,valores_2p[0][0].x,valores_2p[0][1].y);
        }
        scaline();
        glFlush();
     //--------------------------------------------------------------------------------------------------------- 
        //Heredia
        setColor(Heredia); // Amarillo
        clear();
        for( j = 0 ; j < (lineas4_p - 1); j++ ) {
            almacenaPuntoTabla(valores_3p[j][0].x,valores_3p[j][1].y,valores_3p[(j + 1)][0].x,valores_3p[(j + 1)][1].y);
        }
        if (j == (lineas4_p -1)) {
            almacenaPuntoTabla(valores_3p[j][0].x,valores_3p[j][1].y,valores_3p[0][0].x,valores_3p[0][1].y);
        }
        scaline();
        glFlush();
     //--------------------------------------------------------------------------------------------------------- 
        //Limon
        setColor(Limon); //Cyan
        clear();
        for( j = 0 ; j < (lineas5_p - 1); j++ ) {
            almacenaPuntoTabla(valores_4p[j][0].x,valores_4p[j][1].y,valores_4p[(j + 1)][0].x,valores_4p[(j + 1)][1].y);
        }
        if (j == (lineas5_p -1)) {
            almacenaPuntoTabla(valores_4p[j][0].x,valores_4p[j][1].y,valores_4p[0][0].x,valores_4p[0][1].y);
        }
        scaline();
        glFlush();
     //--------------------------------------------------------------------------------------------------------- 
        //Cartago
        setColor(Cartago); //Azul
        clear();
        for( j = 0 ; j < (lineas6_p - 1); j++ ) {
            almacenaPuntoTabla(valores_5p[j][0].x,valores_5p[j][1].y,valores_5p[(j + 1)][0].x,valores_5p[(j + 1)][1].y);
        }
        if (j == (lineas6_p -1)) {
            almacenaPuntoTabla(valores_5p[j][0].x,valores_5p[j][1].y,valores_5p[0][0].x,valores_5p[0][1].y);
        }
        scaline();
        glFlush();
     //--------------------------------------------------------------------------------------------------------- 
        // San Jose
        setColor(SanJose); // Blanco
        clear();
        for( j = 0 ; j < (lineas7_p - 1); j++ ) {
            almacenaPuntoTabla(valores_6p[j][0].x,valores_6p[j][1].y,valores_6p[(j + 1)][0].x,valores_6p[(j + 1)][1].y);
        }
        if (j == (lineas7_p -1)) {
            almacenaPuntoTabla(valores_6p[j][0].x,valores_6p[j][1].y,valores_6p[0][0].x,valores_6p[0][1].y);
        }
        scaline();
        glFlush();
     //--------------------------------------------------------------------------------------------------------- 
        //Puntarenas Costa
        setColor(Puntarenas); //Anaranjado
        clear();
        for( j = 0 ; j < (lineas8_p - 1); j++ ) {
            almacenaPuntoTabla(valores_7p[j][0].x,valores_7p[j][1].y,valores_7p[(j + 1)][0].x,valores_7p[(j + 1)][1].y);
        }
        if (j == (lineas8_p -1)) {
            almacenaPuntoTabla(valores_7p[j][0].x,valores_7p[j][1].y,valores_7p[0][0].x,valores_7p[0][1].y);
        }
        scaline();
        glFlush();
    }else{
        printf("texturas\n");
    }
}

void lineasMapa(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int i,j;
	// Guanacaste
	setColor(Guanacaste); //color verde
	for( j = 0 ; j < (lineas_p - 1); j++ ) {        
		bresenham(valores_0p[j][0].x,valores_0p[j][1].y,valores_0p[(j + 1)][0].x,valores_0p[(j + 1)][1].y);
	}
    if (j == (lineas_p -1)) {
		bresenham(valores_0p[j][0].x,valores_0p[j][1].y,valores_0p[0][0].x,valores_0p[0][1].y);
	}
	glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Puntarenas Peninsula
	setColor(Puntarenas); //Anaranjado
	for( j = 0 ; j < (lineas2_p - 1); j++ ) {
		bresenham(valores_1p[j][0].x,valores_1p[j][1].y,valores_1p[(j + 1)][0].x,valores_1p[(j + 1)][1].y);
	}
	if (j == (lineas2_p -1)) {
		bresenham(valores_1p[j][0].x,valores_1p[j][1].y,valores_1p[0][0].x,valores_1p[0][1].y);
	}
    glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Alajuela	
	setColor(Alajuela); //Rojo 
	for( j = 0 ; j < (lineas3_p - 1); j++ ) {
		bresenham(valores_2p[j][0].x,valores_2p[j][1].y,valores_2p[(j + 1)][0].x,valores_2p[(j + 1)][1].y);
	}

	if (j == (lineas3_p -1)) {
		bresenham(valores_2p[j][0].x,valores_2p[j][1].y,valores_2p[0][0].x,valores_2p[0][1].y);
	}
    glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Heredia
	setColor(Heredia); // Amarillo
	for( j = 0 ; j < (lineas4_p - 1); j++ ) {
		bresenham(valores_3p[j][0].x,valores_3p[j][1].y,valores_3p[(j + 1)][0].x,valores_3p[(j + 1)][1].y);
	}

	if (j == (lineas4_p -1)) {
		bresenham(valores_3p[j][0].x,valores_3p[j][1].y,valores_3p[0][0].x,valores_3p[0][1].y);
	}
	glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Limon
	setColor(Limon); //Cyan
	for( j = 0 ; j < (lineas5_p - 1); j++ ) {
		bresenham(valores_4p[j][0].x,valores_4p[j][1].y,valores_4p[(j + 1)][0].x,valores_4p[(j + 1)][1].y);
	}

	if (j == (lineas5_p -1)) {
		bresenham(valores_4p[j][0].x,valores_4p[j][1].y,valores_4p[0][0].x,valores_4p[0][1].y);
	}
    glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Cartago
	setColor(Cartago); //Azul
	for( j = 0 ; j < (lineas6_p - 1); j++ ) {
		bresenham(valores_5p[j][0].x,valores_5p[j][1].y,valores_5p[(j + 1)][0].x,valores_5p[(j + 1)][1].y);
	}

	if (j == (lineas6_p -1)) {
		bresenham(valores_5p[j][0].x,valores_5p[j][1].y,valores_5p[0][0].x,valores_5p[0][1].y);
	}
    glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    // San Jose
	setColor(SanJose); // Blanco
	for( j = 0 ; j < (lineas7_p - 1); j++ ) {
		bresenham(valores_6p[j][0].x,valores_6p[j][1].y,valores_6p[(j + 1)][0].x,valores_6p[(j + 1)][1].y);
	}

	if (j == (lineas7_p -1)) {
		bresenham(valores_6p[j][0].x,valores_6p[j][1].y,valores_6p[0][0].x,valores_6p[0][1].y);
	}
    glFlush();
 //--------------------------------------------------------------------------------------------------------- 
    //Puntarenas Costa
	setColor(Puntarenas); //Anaranjado
	for( j = 0 ; j < (lineas8_p - 1); j++ ) {
		bresenham(valores_7p[j][0].x,valores_7p[j][1].y,valores_7p[(j + 1)][0].x,valores_7p[(j + 1)][1].y);
	}

	if (j == (lineas8_p -1)) {
		bresenham(valores_7p[j][0].x,valores_7p[j][1].y,valores_7p[0][0].x,valores_7p[0][1].y);
	}
    glFlush();

    //sleep(5);
   // relleno();
}

void specialKeyInput(int key,int x,int y){
    
    int mod;
    int j;
    switch(key){
        case GLUT_KEY_RIGHT:
            
            Pan(-20, 0);
            printf("fine\n");
            aplicarPan();
            printf("fine\n");
            ClippeoPoligono();
            printf("fine\n");
            relleno();
            printf("flecha derecha\n");
            break;
        case GLUT_KEY_LEFT:
            Pan(20, 0);
            printf("fine\n");
            aplicarPan();
            printf("fine\n");
            ClippeoPoligono();
            printf("fine\n");
            relleno();
            printf("flecha izquierda\n");
            break;
        case GLUT_KEY_UP:
            Pan(0, -20);
            aplicarPan();
            ClippeoPoligono();
            relleno();
            printf("flecha arriba\n");
            break;
        case GLUT_KEY_DOWN:
            Pan(0, 20);
            aplicarPan();
            ClippeoPoligono();
            relleno();
            printf("flecha abajo\n");
            break;
        //default:
        //    mod = glutGetModifiers();
        //    if (mod == GLUT_ACTIVE_CTRL){
        //       printf("yeah shift\n");
        //       break;
        //}
    }
}

void keyboard(unsigned char key,int x,int y){
    
    switch(key){
        case '+':
            Zoom(1.5, 1.5);
            aplicarZoom();
            ClippeoPoligono();
            relleno();
            printf("caracter +\n");
            break;
        case '-':
            Zoom(0.6666, 0.6666);
            aplicarZoom();
            ClippeoPoligono();
            relleno();
            printf("caracter -\n");
            break;
        case 'l':
            RotateLeft(0.523599);
            aplicarRotarLeft(); 
            ClippeoPoligono();
            relleno();
            printf("caracter l\n");
            break;
        case 'r':
            RotateRight(-0.523599);
            aplicarRotarRight();
            ClippeoPoligono(); 
            relleno();
            printf("caracter l\n");
            break;
        case 's':
            printf("solo lineas\n");
            lineasMapa();
            break;
        case 'a':
            printf("relleno colores\n");
            relleno();
            break;
        case 'd':
            printf("relleno texturas\n");
            relleno();
            break;
        case 'q':
            exit(0);
            break;
        case 'z':
            printf("desactiva animacion\n");
            break;
    }
}

int* UniToFrame(int X,int Y){
    int Xf,Yf;
    static int r[2];
    Xf= maxHT*((X-Xmin)/(Xmax-Xmin));
    Yf= maxWD*((Y-Ymin)/(Ymax-Ymin));
    r[0]=Xf;
    r[1]=Yf;

    return r;
}

// Returns x-value of point of intersectipn of two
// lines
int x_intersect(int x1, int y1, int x2, int y2,
                int x3, int y3, int x4, int y4)
{
    int num = (x1*y2 - y1*x2) * (x3-x4) -
              (x1-x2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}
 
// Returns y-value of point of intersectipn of
// two lines
int y_intersect(int x1, int y1, int x2, int y2,
                int x3, int y3, int x4, int y4)
{
    int num = (x1*y2 - y1*x2) * (y3-y4) -
              (y1-y2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}
 
// This functions clips all the edges w.r.t one clip
// edge of clipping area
void clip(Matriz **puntos, int *tamano,
          int x1, int y1, int x2, int y2)
{
    int i;
    int nuevo_tamano = 0;
    // (ix,iy),(kx,ky) are the co-ordinate values of
    // the points
    for (i = 0; i < *tamano; i++)
    {
        // i and k form a line in polygon
        int k = (i+1) % *tamano;
        int ix = puntos[i][0].x, iy = puntos[i][1].y;
        int kx = puntos[k][0].x, ky = puntos[k][1].y;
        
 
        // Calculating position of first point
        // w.r.t. clipper line
        int i_pos = (x2-x1) * (iy-y1) - (y2-y1) * (ix-x1);
 
        // Calculating position of second point
        // w.r.t. clipper line
        int k_pos = (x2-x1) * (ky-y1) - (y2-y1) * (kx-x1);
 
        // Case 1 : When both points are inside
        if (i_pos < 0  && k_pos < 0)
        {
            //Only second point is added
            clipper[nuevo_tamano][0].x = kx;
            clipper[nuevo_tamano][1].y = ky;
            nuevo_tamano++;
        }
 
        // Case 2: When only first point is outside
        else if (i_pos >= 0  && k_pos < 0)
        {
            // Point of intersection with edge
            // and the second point is added
            clipper[nuevo_tamano][0].x = x_intersect(x1,
                              y1, x2, y2, ix, iy, kx, ky);
            clipper[nuevo_tamano][1].y = y_intersect(x1,
                              y1, x2, y2, ix, iy, kx, ky);
            nuevo_tamano++;
 
            clipper[nuevo_tamano][0].x = kx;
            clipper[nuevo_tamano][1].y = ky;
            nuevo_tamano++;
        }
 
        // Case 3: When only second point is outside
        else if (i_pos < 0  && k_pos >= 0)
        {
            //Only point of intersection with edge is added
            clipper[nuevo_tamano][0].x = x_intersect(x1,
                              y1, x2, y2, ix, iy, kx, ky);
            clipper[nuevo_tamano][1].y = y_intersect(x1,
                              y1, x2, y2, ix, iy, kx, ky);
            nuevo_tamano++;
        }
 
        // Case 4: When both points are outside
        else
        {
            //No points are added
        }
    }
 
    // Copying new points into original array
    // and changing the no. of vertices
    *tamano = nuevo_tamano;
    for (i = 0; i < nuevo_tamano; i++)
    {
        puntos[i][0].x = clipper[i][0].x;
        puntos[i][1].y = clipper[i][1].y;
    }
}

void prepararClipping(){
    lineas_p = lineas;
    lineas2_p = lineas2;
    lineas3_p = lineas3;
    lineas4_p = lineas4;
    lineas5_p = lineas5;
    lineas6_p = lineas6;
    lineas7_p = lineas7;
    lineas8_p = lineas8;
    int i;
    for(i = 0; i< lineas; i++){
        valores_0p[i][0].x = valores_0[i][0].x;
        valores_0p[i][1].y = valores_0[i][1].y;
    }
    for(i = 0; i < lineas2; i++){
        valores_1p[i][0].x = valores_1[i][0].x;
        valores_1p[i][1].y = valores_1[i][1].y;
    }
    for(i = 0; i < lineas3; i++){
        valores_2p[i][0].x = valores_2[i][0].x;
        valores_2p[i][1].y = valores_2[i][1].y;
    }
    for(i = 0; i < lineas4; i++){
        valores_3p[i][0].x = valores_3[i][0].x;
        valores_3p[i][1].y = valores_3[i][1].y;
    }
    for(i = 0; i < lineas5; i++){
        valores_4p[i][0].x = valores_4[i][0].x;
        valores_4p[i][1].y = valores_4[i][1].y;
    }
    for(i = 0; i < lineas6; i++){
        valores_5p[i][0].x = valores_5[i][0].x;
        valores_5p[i][1].y = valores_5[i][1].y;
    }
    for(i = 0; i < lineas7; i++){
        valores_6p[i][0].x = valores_6[i][0].x;
        valores_6p[i][1].y = valores_6[i][1].y;
    }

    for(i = 0; i < lineas8; i++){
        valores_7p[i][0].x = valores_7[i][0].x;
        valores_7p[i][1].y = valores_7[i][1].y;
    }
}
 
// Implements Sutherland–Hodgman algorithm
void suthHodgClip(Matriz ** puntos, int *tamano,
                  int pantalla[][2], int tamano_puntos)
{
    int j; 
    //i and k are two consecutive indexes
    for (j=0; j<tamano_puntos; j++)
    {
        int k = (j+1) % tamano_puntos;
    
        // We pass the current array of vertices, it's size
        // and the end points of the selected clipper line
        clip(puntos, tamano, pantalla[j][0],
             pantalla[j][1], pantalla[k][0],
             pantalla[k][1]);
        
    }
 
    
}

void ClippeoPoligono(){

    int tamano_puntos = 4;
    int pantalla[][2] = {{1,1}, {1,resolucion},
                              {resolucion,resolucion}, {resolucion,1} };
    prepararClipping();
    //int apuntador = lineas7_p;
    suthHodgClip(valores_0p, &lineas_p, pantalla,
                 tamano_puntos);
    suthHodgClip(valores_1p, &lineas2_p, pantalla,
                 tamano_puntos);
    suthHodgClip(valores_2p, &lineas3_p, pantalla,
                 tamano_puntos);
    suthHodgClip(valores_3p, &lineas4_p, pantalla,
                 tamano_puntos);
    suthHodgClip(valores_4p, &lineas5_p, pantalla,
                 tamano_puntos);
    suthHodgClip(valores_5p, &lineas6_p, pantalla,
                 tamano_puntos);
    suthHodgClip(valores_6p, &lineas7_p, pantalla,
                 tamano_puntos);
    suthHodgClip(valores_7p, &lineas8_p, pantalla,
                 tamano_puntos);
}

int main(int argc,char** argv){

    IniciaLineas();
	IniciaMatrices();
    setColores();
	CoordMapas();

	glutInit(&argc, argv);
   	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  	glutInitWindowSize(maxWD+10,maxHT+10);
  	glutCreateWindow("PROG0_GeovannyAstorga_MaurcioCastillo");
  	glClear(GL_COLOR_BUFFER_BIT);
  	glMatrixMode(GL_PROJECTION);
  	gluOrtho2D(-0.5, resolucion +0.5, -0.5, resolucion + 0.5);
  	glutDisplayFunc(lineasMapa);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyInput);
  	glutMainLoop();
    return 0;
}