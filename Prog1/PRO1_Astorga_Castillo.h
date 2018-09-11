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
 
#define maxVer 10000
#define maxWD 800
#define maxHT 800

typedef struct
{
	int x;
	int y;
} Matriz;

typedef struct
{
	double valor;
} MatrizOp;

typedef struct 
{
	float r;
	float g;
	float b;
}Color;

typedef struct edgebucket 
{
    int ymax;
    float xofymin;
    float slopeinverse;
}EdgeBucket;

typedef struct edgetabletup
{     
    int countEdgeBucket;
    EdgeBucket buckets[maxVer];
}EdgeTableTuple;