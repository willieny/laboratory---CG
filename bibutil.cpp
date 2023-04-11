#include <math.h>
#include <string.h>
#include <vector>
#include "bibutil.h"

#define DEBUG

using namespace std;

vector<OBJ*> _objetos(0);
vector<MAT*> _materiais(0);

char _modo = 's'; // desenha sólidos sem textura

int _numquadro =0, _tempo, _tempoAnterior = 0;
float _ultqps = 0;

#ifndef __FREEGLUT_EXT_H__
void glutBitmapString(void *fonte,char *texto)
{
	while (*texto)
    	glutBitmapCharacter(fonte, *texto++);
}
#endif

void Normaliza(VERT &norm)
{
	float tam = sqrt(norm.x*norm.x
			+norm.y*norm.y
			+norm.z*norm.z);
	if(tam == 0) return;
	norm.x /= tam;
	norm.y /= tam;
	norm.z /= tam;
}

void ProdutoVetorial (VERT &v1, VERT &v2, VERT &vresult)
{
	vresult.x = v1.y * v2.z - v1.z * v2.y;
	vresult.y = v1.z * v2.x - v1.x * v2.z;
	vresult.z = v1.x * v2.y - v1.y * v2.x;
}

void VetorNormal(VERT vert1, VERT vert2, VERT vert3, VERT &n)
{
    VERT vet1, vet2;
    
    vet1.x = vert1.x - vert2.x;
    vet1.y = vert1.y - vert2.y;
    vet1.z = vert1.z - vert2.z;
    vet2.x = vert3.x - vert2.x;
    vet2.y = vert3.y - vert2.y;
    vet2.z = vert3.z - vert2.z;
    ProdutoVetorial(vet2, vet1, n);
    Normaliza(n);
}

void RotaZ(VERT &in, VERT &out, float ang)
{
	float arad = ang*M_PI/180.0;
	out.x =  in.x * cos(arad) + in.y * sin(arad);
	out.y = -in.x * sin(arad) + in.y * cos(arad);
	out.z =  in.z;
}

void RotaY(VERT &in, VERT &out, float ang)
{
	float arad = ang*M_PI/180.0;
	out.x = in.x * cos(arad) - in.z * sin(arad);
	out.y = in.y;
	out.z = in.x * sin(arad) + in.z * cos(arad);
}

void RotaX(VERT &in, VERT &out, float ang)
{
	float arad = ang*M_PI/180.0;
	out.x =  in.x;
	out.y =  in.y * cos(arad) + in.z * sin(arad);
	out.z = -in.y * sin(arad) + in.z * cos(arad);
}

int leNum(char **face, char *sep)
{
	char temp[10];
	int pos = 0;
	while(**face)
	{
		if(**face == '/' || **face==' ') 
		{
			*sep = **face;
			(*face)++;
			break;
		}
		temp[pos++] = **face;
		(*face)++;
	}
	temp[pos]=0;
	if(!pos) 
		return -1;
	return atoi(temp);
}

int _procuraMaterial(char *nome)
{
	unsigned int i;
	for(i=0;i<_materiais.size();++i)
		if(!strcmp(nome,_materiais[i]->nome))
			return i;
	return -1;
}

MAT *ProcuraMaterial(char *nome)
{
	int pos = _procuraMaterial(nome);
	if(pos == -1) return NULL;
	else return _materiais[pos];
}

void _leMateriais(char *nomeArquivo)
{
	char aux[256];
	FILE *fp;
	MAT *ptr;
	fp = fopen(nomeArquivo,"r");

	while(!feof(fp))
	{
		fgets(aux,255,fp);
		if (strlen(aux)>1)
			aux[strlen(aux)-1]=0;	
		if(aux[0]=='#') continue;
		if(!strncmp(aux,"newmtl",6)) 
		{
			if(_procuraMaterial(&aux[7])!=-1)
			{
				ptr = NULL;
				continue;
			}
			if((ptr = (MAT *) malloc(sizeof(MAT)))==NULL)
			{
				exit(1);
			}
			_materiais.push_back(ptr);
			strcpy(ptr->nome,&aux[7]);
			ptr->ke[0] = ptr->ke[1] = ptr->ke[2] = 0.0;
		}
		if(!strncmp(aux,"Ka ",3)) 
		{
			if(ptr==NULL) continue;
			sscanf(aux,"Ka %f %f %f",&ptr->ka[0],&ptr->ka[1],&ptr->ka[2]);
		}
		if(!strncmp(aux,"Kd ",3)) 
		{
			if(ptr==NULL) continue;
			sscanf(aux,"Kd %f %f %f",&ptr->kd[0],&ptr->kd[1],&ptr->kd[2]);
		}
		if(!strncmp(aux,"Ks ",3)) 
		{
			if(ptr==NULL) continue;
			sscanf(aux,"Ks %f %f %f",&ptr->ks[0],&ptr->ks[1],&ptr->ks[2]);
		}
		if(!strncmp(aux,"Ns ",3)) 
		{
			if(ptr==NULL) continue;
			sscanf(aux,"Ns %f",&ptr->spec);
			
			ptr->spec = ptr->spec/1000.0 * 128;
		}
		if(!strncmp(aux,"d ",2)) 
		{
			if(ptr==NULL) continue;

			float alpha;
			sscanf(aux,"d %f",&alpha);
			ptr->ka[3] = alpha;
			ptr->kd[3] = alpha;
			ptr->ks[3] = alpha;
		}
	}
	fclose(fp);
}

OBJ *CarregaObjeto(char *nomeArquivo, bool mipmap)
{
	int i;
	int vcont,ncont,fcont,tcont;
	int material;
	char aux[256];
	FILE *fp;
	OBJ *obj;
	
	fp = fopen(nomeArquivo, "r"); 

#ifdef DEBUG
#endif

	if(fp == NULL)
          return NULL;

	if ( ( obj = (OBJ *) malloc(sizeof(OBJ)) ) == NULL)
		return NULL;

	obj->numVertices  = 0;
	obj->numFaces     = 0;
	obj->numNormais   = 0;
	obj->numTexcoords = 0;
	obj->normais_por_vertice = false;
	obj->tem_materiais = false;
	obj->dlist = -1;	

	obj->vertices = NULL;
	obj->faces = NULL;
	obj->normais = NULL;

	while(!feof(fp))
	{
		fgets(aux,255,fp);
		if(!strncmp(aux,"v ",2)) 
			obj->numVertices++;
		if(!strncmp(aux,"f ",2)) 
			obj->numFaces++;
		if(!strncmp(aux,"vn ",3)) 
			obj->numNormais++;
		if(!strncmp(aux,"vt ",3))
			obj->numTexcoords++;
	}
	rewind(fp);

#ifdef DEBUG
#endif

	if ( ( obj->vertices = (VERT *) malloc((sizeof(VERT)) * obj->numVertices) ) == NULL )
		return NULL;

	if ( ( obj->faces = (FACE *) malloc((sizeof(FACE)) * obj->numFaces) ) == NULL )
		return NULL;

	if(obj->numNormais)
		if ( ( obj->normais = (VERT *) malloc((sizeof(VERT)) * obj->numNormais) ) == NULL )
			return NULL;

	vcont = 0;
	ncont = 0;
	tcont = 0;
	fcont = 0;
	material = -1;

	float minx,miny,minz;
	float maxx,maxy,maxz;

	while(!feof(fp))
	{
		fgets(aux,255,fp);
		aux[strlen(aux)-1]=0;	
		if(aux[0]=='#') continue;

		
		if(!strncmp(aux,"mtllib",6))
		{
				_leMateriais(&aux[7]);
				obj->tem_materiais = true;
		}
		if(!strncmp(aux,"usemtl",6))
		{
			material = _procuraMaterial(&aux[7]);
		}
		if(!strncmp(aux,"v ",2))
		{
			sscanf(aux,"v %f %f %f",&obj->vertices[vcont].x,
					&obj->vertices[vcont].y,
					&obj->vertices[vcont].z);
			if(!vcont)
			{
				minx = maxx = obj->vertices[vcont].x;
				miny = maxy = obj->vertices[vcont].y;
				minz = maxz = obj->vertices[vcont].z;
			}
			else
			{
				if(obj->vertices[vcont].x < minx) minx = obj->vertices[vcont].x;
				if(obj->vertices[vcont].y < miny) miny = obj->vertices[vcont].y;
				if(obj->vertices[vcont].z < minz) minz = obj->vertices[vcont].z;
				if(obj->vertices[vcont].x > maxx) maxx = obj->vertices[vcont].x;
				if(obj->vertices[vcont].y > maxy) maxy = obj->vertices[vcont].y;
				if(obj->vertices[vcont].z > maxz) maxz = obj->vertices[vcont].z;
			}
			vcont++;
		}
		if(!strncmp(aux,"vn ",3))
		{
			sscanf(aux,"vn %f %f %f",&obj->normais[ncont].x,
					&obj->normais[ncont].y,
					&obj->normais[ncont].z);
			ncont++;
			obj->normais_por_vertice = true;
		}
		if(!strncmp(aux,"f ",2))
		{
			char *ptr = aux+2;
			obj->faces[fcont].mat = material;
			int vi[10],ti[10],ni[10];
			char sep;
			obj->faces[fcont].nv = 0;
			int nv = 0;
			bool tem_t = false;
			bool tem_n = false;
			while(*ptr)
			{
				vi[nv] = leNum(&ptr,&sep);
				if(sep==' ')
				{
					nv++;
					continue;
				}
				int aux = leNum(&ptr,&sep);
				if(aux!=-1)
				{
					ti[nv] = aux;
					tem_t = true;
				}
				if(sep==' ')
				{
					nv++;
					continue;
				}
				aux = leNum(&ptr,&sep);
				if(aux!=-1)
				{
					ni[nv] = aux;
					tem_n = true;
				}
				nv++;
			}

			obj->faces[fcont].nv = nv;
			obj->faces[fcont].vert = (int *) malloc(sizeof(int)*nv);
			if(tem_n) obj->faces[fcont].norm = (int *) malloc(sizeof(int)*nv);
				else obj->faces[fcont].norm = NULL;
			if(tem_t) obj->faces[fcont].tex  = (int *) malloc(sizeof(int)*nv);
				else obj->faces[fcont].tex = NULL;
			for(i=0;i<nv;++i)
			{

				obj->faces[fcont].vert[i] = vi[i]-1;
				if(tem_n) obj->faces[fcont].norm[i] = ni[i]-1;
				if(tem_t) obj->faces[fcont].tex[i]  = ti[i]-1;
			}
			fcont++;
		}
	}
#ifdef DEBUG
#endif
	fclose(fp);
	_objetos.push_back(obj);
	return obj;
}

// Seta o modo do desenho: w=wireframe, s=sólidos sem textura
// t=sólidos com textura
void SetaModoDesenho(char modo)
{
	if(modo!='w' && modo!='s' && modo!='t') return;
	_modo = modo;
}

// Desenha um objeto 3D passado como parâmetro
void DesenhaObjeto(OBJ *obj)
{
	int i;	
	GLenum prim = GL_POLYGON;	
	GLfloat branco[4] = { 1.0, 1.0, 1.0, 1.0 };	

	// gera uma nova display ou chama a já associada
	if(obj->dlist >= 1000)
		glNewList(obj->dlist-1000,GL_COMPILE_AND_EXECUTE);
	else if(obj->dlist > -1)
	{
		glCallList(obj->dlist);
		return;
	}

	// se objeto for desenhado em wireframe
	if(_modo=='w') prim = GL_LINE_LOOP;

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_TEXTURE_2D);

	// se objeto tiver material, desabilita COLOR_MATERIAL
	if(obj->tem_materiais)
		glDisable(GL_COLOR_MATERIAL);

	// Varre todas as faces do objeto
	for(i=0; i<obj->numFaces; i++)
	{
		if(!obj->normais_por_vertice)
			glNormal3f(obj->normais[i].x,obj->normais[i].y,obj->normais[i].z);

		// verifica se existe material associado a face
		if(obj->faces[i].mat != -1)
		{
			int mat = obj->faces[i].mat;
			glMaterialfv(GL_FRONT,GL_AMBIENT,_materiais[mat]->ka);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,_materiais[mat]->kd);
			glMaterialfv(GL_FRONT,GL_SPECULAR,_materiais[mat]->ks);
			glMaterialfv(GL_FRONT,GL_EMISSION,_materiais[mat]->ke);
			glMaterialf(GL_FRONT,GL_SHININESS,_materiais[mat]->spec);
		}

		// Inicia a face - percorre todos os vertices da face
		glBegin(prim);
		for(int vf=0; vf<obj->faces[i].nv;++vf)
		{
			// envia a normal, se houver normais definidas para cada face
			if(obj->normais_por_vertice)
				glNormal3f(obj->normais[obj->faces[i].norm[vf]].x,
				obj->normais[obj->faces[i].norm[vf]].y,
				obj->normais[obj->faces[i].norm[vf]].z);

			glVertex3f(obj->vertices[obj->faces[i].vert[vf]].x,
		    	obj->vertices[obj->faces[i].vert[vf]].y,
			obj->vertices[obj->faces[i].vert[vf]].z);
		}
		glEnd();
	} 
	
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	
	// se for uma nova display list
	if(obj->dlist >= 1000)
	{
		glEndList();
		obj->dlist-=1000;
	}
}

void _liberaObjeto(OBJ *obj)
{
	if (obj->vertices != NULL)  free(obj->vertices);
	if (obj->normais != NULL)   free(obj->normais);
	for(int i=0; i<obj->numFaces;++i)
	{
		if (obj->faces[i].vert != NULL) free(obj->faces[i].vert);
		if (obj->faces[i].norm != NULL) free(obj->faces[i].norm);
	}
	if (obj->faces != NULL) free(obj->faces);
	free(obj);
}

// Libera a memória ocupada pelos objetos
void LiberaObjeto(OBJ *obj)
{
	unsigned int o;
	if(obj==NULL)	
	{
		for(o=0;o<_objetos.size();++o)
			_liberaObjeto(_objetos[o]);
		_objetos.clear();
	}
	else
	{
		vector<OBJ*>::iterator it = _objetos.begin();
		for(it = _objetos.begin(); it<_objetos.end(); ++it)
			if(*it == obj)
				break;
		_objetos.erase(it);
		_liberaObjeto(obj);
	}
}

// Libera a memória ocupada pelos materiais
void LiberaMateriais()
{
	unsigned int i;
#ifdef DEBUG
#endif
	for(i=0;i<_materiais.size();++i)
	{
#ifdef DEBUG

#endif
		free(_materiais[i]);
	}
	_materiais.clear();
}

void CalculaNormaisPorFace(OBJ *obj)
{
	int i;
	if(obj->normais_por_vertice) return;
	if ( ( obj->normais = (VERT *) malloc((sizeof(VERT)) * obj->numFaces) ) == NULL )
			return;
	for(i=0; i<obj->numFaces; i++)
	VetorNormal(obj->vertices[obj->faces[i].vert[0]],
		obj->vertices[obj->faces[i].vert[1]],
		obj->vertices[obj->faces[i].vert[2]],obj->normais[i]);
}
