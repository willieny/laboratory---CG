#include <GL/glut.h>
#include <math.h>
#include "bibutil.h"
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

// Define filtro inicial como GL_NEAREST_MIPMAP_LINEAR
int filtro = 4;

// Variaveis para controle da projecao
GLfloat fAspect;
GLfloat ang_cam = 60;

// Objetos
OBJ *plano, *mesa, *porta, *janela, *lamp, *monitor, *teclado, *mouse, *cadeira, *interruptor, *lousa, *lixeira;

// Luminosidade base de uma lampada
#define LOW 0.3

// Define parametros de iluminacao
// Luz 1: pontual no teto, frente
GLfloat luzAmb1[4] = {0.1, 0.1, 0.1, 1};   // luz ambiente
GLfloat luzDif1[4] = {LOW, LOW, LOW, 1.0}; // luz difusa
GLfloat luzEsp1[4] = {0.0, 0.0, 0.0, 1.0}; // luz especular
GLfloat posLuz1[4] = {0, 200, 250, 1};	   // posicao da fonte de luz
// Luz 2: pontual no teto, meio da sala
GLfloat luzDif2[4] = {LOW, LOW, LOW, 1.0}; // luz difusa
GLfloat posLuz2[4] = {0, 200, 0, 1};	   // posicao da fonte de luz
// Luz 3: pontual no teto, atras
GLfloat luzDif3[4] = {LOW, LOW, LOW, 1.0}; // luz difusa
GLfloat posLuz3[4] = {0, 200, -250, 1};	   // posicao da fonte de luz

bool luzes[6] = {true, true, true, false, false};

// Define variaveis para navegacao
GLfloat rotX = 0, rotY = 0, rotX_ini, rotY_ini;
GLfloat obsX = 0, obsY = 150, obsZ = 100, obsY_ini;
int x_ini, y_ini, bot;

// Arquivo de cena e arquivo de camera

// Desenha 4 paredes
void DesenhaParedes(void)
{
	// Insere a matriz de transformação corrente na pilha
	// Parede dos fundos
	glPushMatrix();
	glTranslatef(0, 150, -400);
	glScalef(6, 3, 1);
	glColor3f(0.85, 0.85, 0.85);
	DesenhaObjeto(plano);

	// Retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	glPopMatrix();

	// Parede da frente
	glPushMatrix();
	glTranslatef(0, 150, 400);
	glRotatef(180, 0, 1, 0);
	glScalef(6, 3, 1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// Parede esquerda
	glPushMatrix();
	glTranslatef(300, 150, 0);
	glRotatef(-90, 0, 1, 0);
	glScalef(8, 3, 1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// Parede direita
	glPushMatrix();
	glTranslatef(-300, 150, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(8, 3, 1); // 8 m x 75 cm
	DesenhaObjeto(plano);
	glPopMatrix();
}

void DesenhaPorta()
{
	// Desenha a porta
	glPushMatrix();
	glTranslatef(-170, 0, 397.5);
	glRotatef(-180, 0, 1, 0);
	glScalef(110, 110, 100);
	glColor3f(0.75, 0.75, 0.75);
	DesenhaObjeto(porta);
	glPopMatrix();
}

void DesenhaMesas()
{
	// mesas parede dos fundos
	// mesa
	glPushMatrix();
	glTranslatef(263, 0, 33);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// Retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	glPopMatrix();

	// mesa
	glPushMatrix();
	glTranslatef(263, 0, -90);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesa
	glPushMatrix();
	glTranslatef(263, 0, -213);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesa
	glPushMatrix();
	glTranslatef(263, 0, -336);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesas da parede esquerda
	// mesa
	glPushMatrix();
	glTranslatef(165, 0, -363);
	glRotatef(-180, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesa
	glPushMatrix();
	glTranslatef(42, 0, -363);
	glRotatef(-180, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesa
	glPushMatrix();
	glTranslatef(-81, 0, -363);
	glRotatef(-180, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesa
	glPushMatrix();
	glTranslatef(-204, 0, -363);
	glRotatef(-180, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesas da parede da frente
	// mesa
	glPushMatrix();
	glTranslatef(-263, 0, 33);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesa
	glPushMatrix();
	glTranslatef(-263, 0, -90);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesa
	glPushMatrix();
	glTranslatef(-263, 0, -213);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// mesa
	glPushMatrix();
	glTranslatef(-263, 0, -336);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(1, 1, 1);
	DesenhaObjeto(mesa);
	glPopMatrix();

}

void DesenhaMonitores()
{
	// monitores da parede dos fundos
	// monitor
	glPushMatrix();
	glTranslatef(273, 65, 33);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(0.15, 0.15, 0.15);
	DesenhaObjeto(monitor);
	glPopMatrix();

	// Retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	glPopMatrix();

	// monitor
	glPushMatrix();
	glTranslatef(273, 65, -90);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(monitor);
	glPopMatrix();

	// monitor
	glPushMatrix();
	glTranslatef(273, 65, -213);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(monitor);
	glPopMatrix();

	// monitores da parede esquerda
	// monitor
	glPushMatrix();
	glTranslatef(165, 65, -373);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(monitor);
	glPopMatrix();

	// monitor
	glPushMatrix();
	glTranslatef(42, 65, -373);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(monitor);
	glPopMatrix();

	// monitor
	glPushMatrix();
	glTranslatef(-81, 65, -373);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(monitor);
	glPopMatrix();

	// monitor
	glPushMatrix();
	glTranslatef(-204, 65, -373);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(monitor);
	glPopMatrix();

	// monitores da parede da frente
	// monitor
	glPushMatrix();
	glTranslatef(-273, 65, 33);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(0.15, 0.15, 0.15);
	DesenhaObjeto(monitor);
	glPopMatrix();

	// monitor
	glPushMatrix();
	glTranslatef(-273, 65, -90);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(monitor);
	glPopMatrix();

	// monitor
	glPushMatrix();
	glTranslatef(-273, 65, -213);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(monitor);
	glPopMatrix();

}

void DesenhaTeclados()
{
	// teclados da parede dos fundos
	// teclado
	glPushMatrix();
	glTranslatef(251, 65, 33);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(0.15, 0.15, 0.15);
	DesenhaObjeto(teclado);
	glPopMatrix();

	// Retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	glPopMatrix();

	// teclado
	glPushMatrix();
	glTranslatef(251, 65, -90);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(teclado);
	glPopMatrix();

	// teclado
	glPushMatrix();
	glTranslatef(251, 65, -213);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(teclado);
	glPopMatrix();

	// teclados da parede esquerda
	// teclado
	glPushMatrix();
	glTranslatef(165, 65, -351);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(teclado);
	glPopMatrix();

	// teclado
	glPushMatrix();
	glTranslatef(42, 65, -351);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(teclado);
	glPopMatrix();

	// teclado
	glPushMatrix();
	glTranslatef(-81, 65, -351);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(teclado);
	glPopMatrix();

	// teclado
	glPushMatrix();
	glTranslatef(-204, 65, -351);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(teclado);
	glPopMatrix();

	// teclados da parede da frente
	// teclado
	glPushMatrix();
	glTranslatef(-251, 65, 33);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(0.15, 0.15, 0.15);
	DesenhaObjeto(teclado);
	glPopMatrix();

	// teclado
	glPushMatrix();
	glTranslatef(-251, 65, -90);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(teclado);
	glPopMatrix();

	// teclado
	glPushMatrix();
	glTranslatef(-251, 65, -213);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(teclado);
	glPopMatrix();

}

void DesenhaMouses()
{
	// mouses da parede dos fundos
	// mouse
	glPushMatrix();
	glTranslatef(251, 65, 63);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(0.15, 0.15, 0.15);
	DesenhaObjeto(mouse);
	glPopMatrix();

	// Retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	glPopMatrix();

	// mouse
	glPushMatrix();
	glTranslatef(251, 65, -60);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(mouse);
	glPopMatrix();

	// mouse
	glPushMatrix();
	glTranslatef(251, 65, -183);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(mouse);
	glPopMatrix();

	// mouses da parede esquerda
	// mouse
	glPushMatrix();
	glTranslatef(195, 65, -351);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(mouse);
	glPopMatrix();

	// mouse
	glPushMatrix();
	glTranslatef(72, 65, -351);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(mouse);
	glPopMatrix();

	// mouse
	glPushMatrix();
	glTranslatef(-51, 65, -351);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(mouse);
	glPopMatrix();

	// mouse
	glPushMatrix();
	glTranslatef(-174, 65, -351);
	glRotatef(0, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(mouse);
	glPopMatrix();

	// mouses da parede da frente
	// mouse
	glPushMatrix();
	glTranslatef(-251, 65, 3);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(mouse);
	glPopMatrix();

	// mouse
	glPushMatrix();
	glTranslatef(-251, 65, -120);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(mouse);
	glPopMatrix();

	// mouse
	glPushMatrix();
	glTranslatef(-251, 65, -243);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(mouse);
	glPopMatrix();

}

void DesenhaCadeiras()
{
	// cadeiras da parede dos fundos
	// cadeira
	glPushMatrix();
	glTranslatef(221, 0, 33);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	glColor3f(0, 0, 0.35);
	DesenhaObjeto(cadeira);
	glPopMatrix();

	// Retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	glPopMatrix();

	// cadeira
	glPushMatrix();
	glTranslatef(221, 0, -90);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(cadeira);
	glPopMatrix();

	// cadeira
	glPushMatrix();
	glTranslatef(221, 0, -213);
	glRotatef(90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(cadeira);
	glPopMatrix();

	// cadeiras da parede esquerda
	// cadeira
	glPushMatrix();
	glTranslatef(165, 0, -321);
	glRotatef(180, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(cadeira);
	glPopMatrix();

	// cadeira
	glPushMatrix();
	glTranslatef(42, 0, -321);
	glRotatef(180, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(cadeira);
	glPopMatrix();

	// cadeira
	glPushMatrix();
	glTranslatef(-81, 0, -321);
	glRotatef(180, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(cadeira);
	glPopMatrix();

	// cadeira
	glPushMatrix();
	glTranslatef(-204, 0, -321);
	glRotatef(180, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(cadeira);
	glPopMatrix();

	// cadeiras da parede da frente
	// cadeira
	glPushMatrix();
	glTranslatef(-221, 0, 33);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(cadeira);
	glPopMatrix();

	// cadeira
	glPushMatrix();
	glTranslatef(-221, 0, -90);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(cadeira);
	glPopMatrix();

	// cadeira
	glPushMatrix();
	glTranslatef(-221, 0, -213);
	glRotatef(-90, 0, 1, 0);
	glScalef(90, 90, 90);
	DesenhaObjeto(cadeira);
	glPopMatrix();

}

void DesenhaObjExtra()
{
	// lampada (obj1)
	glPushMatrix();
	glTranslatef(0, 297, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	DesenhaObjeto(lamp);
	glPopMatrix();

	// interruptor
	glPushMatrix();
	glTranslatef(-80, 105, 397.5);
	glRotatef(-180, 0, 1, 0);
	glScalef(120, 120, 120);
	glColor3f(0.90, 0.90, 0.90);
	DesenhaObjeto(interruptor);
	glPopMatrix();

	// lousa
	glPushMatrix();
	glTranslatef(90, 165, 397.5);
	glRotatef(-180, 0, 1, 0);
	glScalef(0.5, 0.65, 0.5);
	glColor3f(1, 1, 1);
	DesenhaObjeto(lousa);
	glPopMatrix();

	// lixeira
	glPushMatrix();
	glTranslatef(-60, 0, 380);
	glRotatef(-180, 0, 1, 0);
	glScalef(120, 120, 120);
	glColor3f(0.15, 0.15, 0.15);
	DesenhaObjeto(lixeira);
	glPopMatrix();
}

// Desenha o chao
void DesenhaChao(void)
{
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(6, 8, 1);
	glColor3f(0.90, 0.90, 0.90);
	DesenhaObjeto(plano);
	glPopMatrix();
}

// Desenha o teto
void DesenhaTeto(void)
{
	glPushMatrix();
	glTranslatef(0, 300, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(6, 8, 1);
	glColor3f(1, 1, 1);
	DesenhaObjeto(plano);
	glPopMatrix();
}

// Desenha toda a cena
void Desenha(void)
{
	// Limpa a janela de visualizacao com a cor
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Inicializa sistema de coordenadas do modelo
	// antes da execucao de qualquer operacao de manipulacao de matrizes
	glLoadIdentity();

	// Posiciona e orienta observador
	glRotatef(rotX, 1, 0, 0);
	glRotatef(rotY, 0, 1, 0);
	glTranslatef(-obsX, -obsY, -obsZ);

	glEnable(GL_TEXTURE_2D);

	// Agora posiciona a fonte de luz do meio da sala
	// Agora posiciona demais fontes de luz
	glLightfv(GL_LIGHT0, GL_POSITION, posLuz1);
	glLightfv(GL_LIGHT1, GL_POSITION, posLuz2);
	glLightfv(GL_LIGHT2, GL_POSITION, posLuz3);

	// Luz spot
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 70.0);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10);

	// Desenha todos os elementos da cena
	DesenhaParedes();
	DesenhaChao();
	DesenhaTeto();
	DesenhaPorta();
	DesenhaMesas();
	DesenhaMonitores();
	DesenhaTeclados();
	DesenhaMouses();
	DesenhaCadeiras();
	DesenhaObjExtra();

	// Faz a troca dos buffers
	glutSwapBuffers();
}

// Funcao usada para especificar o volume de visualizacao
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projecao
	glMatrixMode(GL_PROJECTION);

	// Inicializa sistema de coordenadas de projecao
	glLoadIdentity();

	// Especifica a projecao perspectiva
	// (angulo, aspecto, zMin, zMax)
	gluPerspective(ang_cam, fAspect, 0.1, 1000);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
}

// Funcao callback chamada quando o tamanho da janela e alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisao por zero
	if (h == 0)
		h = 1;

	// Especifica as dimensoes da viewport
	glViewport(0, 0, w, h);

	// Calcula a correcao de aspecto
	fAspect = (GLfloat)w / (GLfloat)h;

	EspecificaParametrosVisualizacao();
}

// Funcao callback para eventos de teclas de 
// navegacao atraves das setas
void Teclado(int key, int x, int y)
{
	float sina, cosa, sina_01, cosa_01;

	// Pre-calcula o seno e cosseno do angulo
	// de direcao atual + 90 graus, ou seja,
	// a direcao para deslocamento lateral
	sina = 5 * sin((rotY + 90) * M_PI / 180.0);
	cosa = 5 * cos((rotY + 90) * M_PI / 180.0);

	// Pre-calcula o seno e cosseno do angulo
	// sem os 90 graus
	sina_01 = 5 * sin(rotY * M_PI / 180.0);
	cosa_01 = 5 * cos(rotY * M_PI / 180.0);

	// Trata as teclas especiais
	switch (key)
	{
	// Controles de deslocamento lateral
	case GLUT_KEY_LEFT:
		obsX = obsX - sina;
		obsZ = obsZ + cosa;
		break;
	case GLUT_KEY_RIGHT:
		obsX = obsX + sina;
		obsZ = obsZ - cosa;
		break;
	case GLUT_KEY_UP:
		obsX = obsX + sina_01;
		obsZ = obsZ - cosa_01;
		break;

	case GLUT_KEY_DOWN:
		obsX = obsX - sina_01;
		obsZ = obsZ + cosa_01;
		break;
	}
	glutPostRedisplay();
}

// Funcao callback para eventos de botoes do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		// Atualiza as variaveis de navegação com os valores atuais
		x_ini = x;
		y_ini = y;
		obsY_ini = obsY;
		rotX_ini = rotX;
		rotY_ini = rotY;
		bot = button;
	}
	else
		bot = -1;
}

// Funcao callback para eventos de movimento do mouse
#define SENS_ROT 5.0
#define SENS_OBS 10.0

void GerenciaMovimentoMouse(int x, int y)
{
	// Botao esquerdo ? 
	if (bot == GLUT_LEFT_BUTTON)
	{
		// Calcula diferencas
		int deltax = x_ini - x;
		int deltay = y_ini - y;
		// E modifica angulos
		rotY = rotY_ini - deltax / SENS_ROT;
		rotX = rotX_ini - deltay / SENS_ROT;
	}
	// Botao direito ?
	else if (bot == GLUT_RIGHT_BUTTON)
	{
		// Calcula diferenca
		int deltay = y_ini - y;
		// E modifica altura do observador
		obsY = obsY_ini + deltay / SENS_OBS;
	}
	glutPostRedisplay();
}

// Funcao responsavel por inicializar parametros e variaveis
void Inicializa(void)
{
	// Define a cor de fundo da janela de visualizacao como preto
	glClearColor(0, 0, 0, 1);

	// Ajusta iluminacao
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmb1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDif1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEsp1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDif2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, luzAmb1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, luzDif3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, luzAmb1);

	// Habilita todas as fontes de luz
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHTING);

	// Define coeficientes ambiente e difuso
	// do material
	GLfloat matAmb[4] = {0.2, 0.2, 0.2, 1};
	GLfloat matDif[4] = {1, 1, 1, 1};

	// Material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif);

	// Seleciona o modo de GL_COLOR_MATERIAL
	// faz com que uma cor de material acompanhe a cor atual
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Habilita normalizacao automatica
	// Vetores normais sao normalizados para valores unitarios
	// apos transformacao e antes da iluminacao
	glEnable(GL_NORMALIZE);

	// Habilita Z-Buffer
	// Realiza comparacoes de profundidade
	// e atualiza o buffer de profundidade
	glEnable(GL_DEPTH_TEST);

	// Carrega objetos
	plano = CarregaObjeto("obj/parede.obj", true);
	mesa = CarregaObjeto("obj/mesa.obj", true);
	porta = CarregaObjeto("obj/porta.obj", false);
	janela = CarregaObjeto("obj/janela.obj", false);
	lamp = CarregaObjeto("obj/lampada.obj", false);
	monitor = CarregaObjeto("obj/monitor.obj", false);
	teclado = CarregaObjeto("obj/teclado.obj", false);
	mouse = CarregaObjeto("obj/mouse.obj", false);
	cadeira = CarregaObjeto("obj/cadeira.obj", false);
	interruptor = CarregaObjeto("obj/interruptor.obj", false);
	lousa = CarregaObjeto("obj/lousa.obj", false);
	lixeira = CarregaObjeto("obj/lixeira.obj", false);
}

// Programa Principal
int main(int argc, char **argv)
{
	// Inicilizar a Glut
	glutInit(&argc, argv);

	// Define o modo de operacao da GLUT
	// GLUT_DOUBLE : dois buffers de cor
	// GLUT_RGB : define que as cores do programa serão especificadas por componentes GLUT_RGB
	// GLUT_DEPTH : buffer de profundidade
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Especifica a posicao e o tamanho inicial em pixels da janela GLUT
	glutInitWindowPosition(100, 70);
	glutInitWindowSize(1700, 900);

	// Cria a janela passando como argumento o titulo da mesma
	glutCreateWindow("IC Laboratory");

	// Registra a funcao callback de redesenho da janela de visualizacao
	glutDisplayFunc(Desenha);

	// Registra a funcao callback de teclas especiais
	glutSpecialFunc(Teclado);

	// Registra a funcao callback para eventos de botoes do mouse
	glutMouseFunc(GerenciaMouse);

	// Registra a funcao callback para eventos de movimento do mouse
	glutMotionFunc(GerenciaMovimentoMouse);

	// Registra a funcao callback de redimensionamento da janela de visualizacao
	glutReshapeFunc(AlteraTamanhoJanela);

	// Chama a funcao responsavel por fazer as inicializacoes
	Inicializa();

	// Inicia o processamento e aguarda interacoes do usuario
	glutMainLoop();

	return 0;
}