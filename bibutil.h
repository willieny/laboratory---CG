#ifndef __BIBUTIL_H
#define __BIBUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.1415926
#endif

typedef struct {
	GLfloat x,y,z;
} VERT;


typedef struct {
	GLint nv;		
	GLint *vert;	
	GLint *norm;	
	GLint *tex;		
	GLint mat;		
	GLint texid;	
} FACE;


typedef struct {
	GLfloat s,t,r;
} TEXCOORD;


typedef struct {
	GLint numVertices;
	GLint numFaces;
	GLint numNormais;
	GLint numTexcoords;
	bool normais_por_vertice;	
	bool tem_materiais;						
	GLint dlist;				
	VERT *vertices;
	VERT *normais;
	FACE *faces;
	TEXCOORD *texcoords;
} OBJ;


typedef struct {
	char nome[20];	
	GLfloat ka[4];	
	GLfloat kd[4];	
	GLfloat ks[4];	
	GLfloat ke[4];	
	GLfloat spec;	
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
