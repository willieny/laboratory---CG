#ifndef __BIBUTIL_H
#define __BIBUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.1415926
#endif

// Define a estrutura de um vértice
typedef struct {
	GLfloat x,y,z;
} VERT;

// Define a estrutura de uma face
typedef struct {
	GLint nv;		
	GLint *vert;	
	GLint *norm;	
	GLint *tex;		
	GLint mat;	
	GLint texid;	
} FACE;

// Define a estrutura de um objeto 3D
typedef struct {
	GLint numVertices;
	GLint numFaces;
	GLint numNormais;
	GLint numTexcoords;
	bool normais_por_vertice;	
	bool tem_materiais;						
	GLint dlist;	// display list, se houver				
	VERT *vertices;
	VERT *normais;
	FACE *faces;
} OBJ;

// Define um material
typedef struct {
	char nome[20];	// identificacao do objeto
	GLfloat ka[4];	// ambiente
	GLfloat kd[4];	// difuso
	GLfloat ks[4];	// especular
	GLfloat ke[4];	// emissao
	GLfloat spec;	// especularidade
} MAT;

void Normaliza(VERT &norm);
void ProdutoVetorial (VERT &v1, VERT &v2, VERT &vresult);
void VetorNormal(VERT vert1, VERT vert2, VERT vert3, VERT &n);
void RotaZ(VERT &in, VERT &out, float ang);
void RotaY(VERT &in, VERT &out, float ang);
void RotaX(VERT &in, VERT &out, float ang);


OBJ *CarregaObjeto(char *nomeArquivo, bool mipmap);
void CriaDisplayList(OBJ *obj);
void DesabilitaDisplayList(OBJ *ptr);
void DesenhaObjeto(OBJ *obj);
void SetaModoDesenho(char modo);


void LiberaObjeto(OBJ *obj);
void LiberaMateriais();


float CalculaQPS(void);
void Escreve2D(float x, float y, char *str);

void CalculaNormaisPorFace(OBJ *obj);

MAT *ProcuraMaterial(char *nome);

#endif
