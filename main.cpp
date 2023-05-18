#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "bibutil.h"
#include "bibutilNoTex.h"
#include "myutil.h"
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

TEX *parede, *chao, *teto, *tabua, *telalousa, *tecido, *metalico, *grama, *foto, *portaTex;

// Filtros de textura
GLint filtros[] = {
       GL_NEAREST, GL_LINEAR,
       GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR_MIPMAP_NEAREST,
       GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
};

// Define filtro inicial como GL_NEAREST_MIPMAP_LINEAR
int filtro = 4;

// Define modo inicial de aplicacao da textura
GLint modo = GL_MODULATE;

// Define modo de desenho inicial: textura
char modo_des = 't';

void SetaEscalaTextura(float x,float y) {
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(x,y,1);
	glMatrixMode(GL_MODELVIEW);
}

// Variaveis para controle da projecao
GLfloat fAspect;
GLfloat ang_cam = 60;

// Variaveis para controle da porta
#define ANGLE_DOOR_MAX 90
GLfloat x_trans_angle = 0.430,
		angle_door = 0;

// Variaveis para controle da janela
#define ANGLEE_WINDOW_MAX -180
#define ANGLED_WINDOW_MAX 180

GLfloat xE_trans_angle = 0.430,
		xD_trans_angle = -0.430,
		angleE_window = 0,
		angleD_window = 0;

// Variaveis para controle da hélice
GLfloat angulo;
GLfloat sentido = true;

// Variaveis para controle do mouse
#define SENS_ROT 5.0
#define SENS_OBS 10.0

// Luminosidade base de uma lampada
#define LOW 0.3

// Define parametros de iluminacao

// Luz 1: pontual no teto, frente
GLfloat luzDif1[4] = {LOW, LOW, LOW, 1.0}; // luz difusa
GLfloat luzEsp1[4] = {0.0, 0.0, 0.0, 1.0}; // luz especular
GLfloat posLuz1[4] = {0, 200, 250, 1};	   // posicao da fonte de luz

// Luz 2: pontual no teto, meio da sala
GLfloat luzDif2[4] = {LOW, LOW, LOW, 1.0}; // luz difusa
GLfloat posLuz2[4] = {0, 200, 0, 1};	   // posicao da fonte de luz

// Luz 3: pontual no teto, atras
GLfloat luzDif3[4] = {LOW, LOW, LOW, 1.0}; // luz difusa
GLfloat posLuz3[4] = {0, 200, -250, 1};	   // posicao da fonte de luz

// // Luz 4: Sol
GLfloat luzAmb4[4] = {0.3, 0.3, 0.3, 1.0}; // luz difusa
GLfloat luzDif4[4] = {0.4, 0.2, 0.0, 1.0}; // luz difusa
GLfloat posLuz4[4] = {0.4, 0.4, 1, 0};	   // posicao da fonte de luz

GLfloat luzAmb5[4] = {0.2, 0.2, 0.2, 1.0}; // luz ambiente
GLfloat luzDif5[4] = {1, 1, 0.7, 1}; // luz difusa
GLfloat luzEsp5[4] = {0, 0, 0, 1.0}; // luz especular
GLfloat posLuz5[4] = {250, 105, -13, 1.0};	   // posicao da fonte de luz
GLfloat dirLuz5[3] = {0, -1, 1};	   // posicao da fonte de luz

GLfloat luzAmb6[4] = {0.2, 0.2, 0.2, 1.0}; // luz ambiente
GLfloat luzDif6[4] = {1, 1, 0.7, 1}; // luz difusa
GLfloat luzEsp6[4] = {0, 0, 0, 1.0}; // luz especular
GLfloat posLuz6[4] = {-250, 105, -13, 1.0};	   // posicao da fonte de luz
GLfloat dirLuz6[3] = {0, -1, 1};	   // posicao da fonte de luz

GLfloat luzAmb7[4] = {0.2, 0.2, 0.2, 1.0}; // luz ambiente
GLfloat luzDif7[4] = {1, 1, 0.7, 1}; // luz difusa
GLfloat luzEsp7[4] = {0, 0, 0, 1.0}; // luz especular
GLfloat posLuz7[4] = {-20, 85, -380, 1.0};	   // posicao da fonte de luz
GLfloat dirLuz7[3] = {0, -1, 1};	   // posicao da fonte de luz

int luz = 0;

GLfloat luzAmbiente[4] = {0.1, 0.1, 0.1, 1};

bool luzes[7] = {true, true, true, false, true, true, true};

// Objetos
OBJ *plano,
	*mesa,
	*porta,
	*janela,
	*lamp,
	*monitor,
	*teclado,
	*mouse,
	*cadeira,
	*interruptor,
	*lousa,
	*molduralousa,
	*lixeira,
	*base,
	*helice,
	*luminaria,
	*quadro;

// Define variaveis para navegacao
GLfloat rotX = 0,
		rotY = 0,
		rotX_ini,
		rotY_ini;

GLfloat obsX = 0,
		obsY = 150,
		obsZ = 100,
		obsY_ini;

int x_ini,
	y_ini,
	bot;

void DesenhaSala() {

	SetaEscalaTextura(8, 3);

	if (modo_des == 't')
		glColor3f(1, 1, 1);
	else
		glColor3ub(196, 210, 184);

	// paredes
	glColor3f(0.92, 0.92, 0.92);

	desenhaGeralTextura(plano, 300, 150, 0, -90, 0, 1, 0, 8, 3, 1, parede);        // esquerda
	SetaEscalaTextura(8, 3);
	desenhaGeralTextura(plano, -300, 150, 0, 90, 0, 1, 0, 8, 3, 1, parede);        // direita

	// fundos
	SetaEscalaTextura(6, 0.52);
	desenhaGeralTextura(plano, 0, 274.5, -400, 0, 0, 1, 0, 6, 0.52, 1, parede);    // superior
	SetaEscalaTextura(6, 1.315);
	desenhaGeralTextura(plano, 0, 65.5, -400, 0, 0, 0, 0, 6, 1.315, 1, parede);    // inferior
	SetaEscalaTextura(1.69, 1.2);
	desenhaGeralTextura(plano, 0, 190, -400, 0, 0, 0, 0, 1.69, 1.2, 1, parede);    // meio
	SetaEscalaTextura(1.14, 1.2);
	desenhaGeralTextura(plano, 243, 190, -400, 0, 0, 0, 0, 1.14, 1.2, 1, parede);  // direita
	SetaEscalaTextura(1.14, 1.2);
	desenhaGeralTextura(plano, -243, 190, -400, 0, 0, 0, 0, 1.14, 1.2, 1, parede); // esquerda

	// frente
	SetaEscalaTextura(6, 1);
	desenhaGeralTextura(plano, 0, 250, 400, 180, 0, 1, 0, 6, 1, 1, parede);        // superior
	SetaEscalaTextura(4.25, 2);
	desenhaGeralTextura(plano, 87.5, 100, 400, 180, 0, 1, 0, 4.25, 2, 1, parede);  // esquerda
	SetaEscalaTextura(0.86, 2);
	desenhaGeralTextura(plano, -257, 100, 400, 180, 0, 1, 0, 0.86, 2, 1, parede);  // direita

	// chão
	SetaEscalaTextura(6,8);
	glColor3f(0.80, 0.80, 0.80);
	desenhaGeralTextura(plano, 0, 0, 0, -90, 1, 0, 0, 6, 8, 1, chao);

	// teto
	SetaEscalaTextura(6, 8);
	glColor3f(0.90, 0.90, 0.90);
	desenhaGeralTextura(plano, 0, 300, 0, 90, 1, 0, 0, 6, 8, 1, teto);

	// Grama
	SetaEscalaTextura(5, 5);
	glColor3f(0.2, 1.0, 0.2);
	desenhaGeralTextura(plano, 0, -0.1, 0, -90, 1, 0, 0, 20, 20, 1, grama);
}

void DesenhaPorta() {
	// Desenha a porta
	SetaEscalaTextura(1, 1);
	glPushMatrix();
	glTranslatef(-170, 0, 397.5);
	glScalef(100, 100, 100);
	glRotatef(-180, 0, 1, 0);
	glColor3f(0.75, 0.75, 0.75);
	glTranslatef(x_trans_angle, 0, 0);
	glRotated(angle_door, 0, 1, 0);
	glTranslatef(-x_trans_angle, 0, 0);
	porta->textura = portaTex->texid;
	DesenhaObjeto(porta);
	glPopMatrix();
}

void DesenhaJanela() {
	glColor3f(0.65, 0.5, 0.35);
	SetaEscalaTextura(1, 1);
	// Janela Direita
	// JanelaE
	glPushMatrix();
	glTranslatef(109.5, 130, -399);
	glScalef(60, 60, 60);
	glRotatef(-180, 0, 1, 0);
	glTranslatef(xE_trans_angle, 0, 0);
	glRotated(angleE_window, 0, 1, 0);
	glTranslatef(-xE_trans_angle, 0, 0);
	janela->textura = tabua->texid;
	DesenhaObjeto(janela);
	glPopMatrix();

	// JanelaD
	glPushMatrix();
	glTranslatef(160, 130, -399);
	glScalef(60, 60, 60);
	glRotatef(-180, 0, 1, 0);
	glTranslatef(xD_trans_angle, 0, 0);
	glRotated(angleD_window, 0, 1, 0);
	glTranslatef(-xD_trans_angle, 0, 0);
	janela->textura = tabua->texid;
	DesenhaObjeto(janela);
	glPopMatrix();

	// Janela Esquerda
	// JanelaE
	glPushMatrix();
	glTranslatef(-160.5, 130, -399);
	glScalef(60, 60, 60);
	glRotatef(-180, 0, 1, 0);
	glTranslatef(xE_trans_angle, 0, 0);
	glRotated(angleE_window, 0, 1, 0);
	glTranslatef(-xE_trans_angle, 0, 0);
	janela->textura = tabua->texid;
	DesenhaObjeto(janela);
	glPopMatrix();

	// JanelaD
	glPushMatrix();
	glTranslatef(-110, 130, -399);
	glScalef(60, 60, 60);
	glRotatef(-180, 0, 1, 0);
	glTranslatef(xD_trans_angle, 0, 0);
	glRotated(angleD_window, 0, 1, 0);
	glTranslatef(-xD_trans_angle, 0, 0);
	janela->textura = tabua->texid;
	DesenhaObjeto(janela);
	glPopMatrix();
}

void DesenhaVentiladores() {
	// Base
	glColor3f(0.75, 0.75, 0.75);
	desenhaGeral(base, 0, 252, 180, -180, 0, 1, 0, 40, 40, 40);

	// Hélice
	SetaEscalaTextura(1, 1);
	glPushMatrix();
	glTranslatef(0, 255, 180);
	glScalef(370, 100, 370);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	glColor3f(1, 1, 1);
	glRotatef(angulo, 0, 0, 1);
	helice->textura = metalico->texid;
	DesenhaObjeto(helice);
	glPopMatrix();

	// Base
	glColor3f(0.75, 0.75, 0.75);
	desenhaGeral(base, 0, 252, -180, -180, 0, 1, 0, 40, 40, 40);

	// Hélice
	SetaEscalaTextura(1, 1);
	glPushMatrix();
	glTranslatef(0, 255, -180);
	glScalef(370, 100, 370);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	glColor3f(1, 1, 1);
	glRotatef(angulo, 0, 0, 1);
	helice->textura = metalico->texid;
	DesenhaObjeto(helice);
	glPopMatrix();
}

void DesenhaMesas() {

	glColor3f(1, 1, 1);
	SetaEscalaTextura(1, 1);
	desenhaGeralTextura(mesa, 263, 0, 33, 90, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, 263, 0, -90, 90, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, 263, 0, -213, 90, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, 263, 0, -336, 90, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, 165, 0, -363, -180, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, 42, 0, -363, -180, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, -81, 0, -363, -180, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, -204, 0, -363, -180, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, -263, 0, 33, -90, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, -263, 0, -90, -90, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, -263, 0, -213, -90, 0, 1, 0, 90, 90, 90, tabua);
	desenhaGeralTextura(mesa, -263, 0, -336, -90, 0, 1, 0, 90, 90, 90, tabua);
}

void DesenhaMonitores() {

	glColor3f(0.15, 0.15, 0.15);

	desenhaGeral(monitor, 273, 65, 33, -90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(monitor, 273, 65, -90, -90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(monitor, 273, 65, -213, -90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(monitor, 165, 65, -373, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(monitor, 42, 65, -373, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(monitor, -81, 65, -373, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(monitor, -273, 65, 33, 90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(monitor, -273, 65, -90, 90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(monitor, -273, 65, -213, 90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(monitor, -204, 65, -373, 0, 0, 1, 0, 90, 90, 90);
}

void DesenhaTeclados() {

	glColor3f(0.15, 0.15, 0.15);

	desenhaGeral(teclado, 251, 65, 33, -90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(teclado, 251, 65, -90, -90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(teclado, 251, 65, -213, -90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(teclado, 165, 65, -351, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(teclado, 42, 65, -351, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(teclado, -81, 65, -351, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(teclado, -204, 65, -351, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(teclado, -251, 65, 33, 90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(teclado, -251, 65, -90, 90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(teclado, -251, 65, -213, 90, 0, 1, 0, 90, 90, 90);
}

void DesenhaMouses() {
	
	glColor3f(0.15, 0.15, 0.15);

	desenhaGeral(mouse, 251, 65, 63, -90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(mouse, 251, 65, -60, -90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(mouse, 251, 65, -183, -90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(mouse, 195, 65, -351, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(mouse, 72, 65, -351, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(mouse, -51, 65, -351, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(mouse, -174, 65, -351, 0, 0, 1, 0, 90, 90, 90);
	desenhaGeral(mouse, -251, 65, 3, 90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(mouse, -251, 65, -120, 90, 0, 1, 0, 90, 90, 90);
	desenhaGeral(mouse, -251, 65, -243, 90, 0, 1, 0, 90, 90, 90);
}

void DesenhaCadeiras() {

	glColor3f(1, 1, 1);
	SetaEscalaTextura(5, 5);
	desenhaGeralTextura(cadeira, 221, 0, 33, 90, 0, 1, 0, 90, 90, 90, tecido);
	desenhaGeralTextura(cadeira, 221, 0, -90, 90, 0, 1, 0, 90, 90, 90, tecido);
	desenhaGeralTextura(cadeira, 221, 0, -213, 90, 0, 1, 0, 90, 90, 90, tecido);
	desenhaGeralTextura(cadeira, 165, 0, -321, 180, 0, 1, 0, 90, 90, 90, tecido);
	desenhaGeralTextura(cadeira, 42, 0, -321, 180, 0, 1, 0, 90, 90, 90, tecido);
	desenhaGeralTextura(cadeira, -81, 0, -321, 180, 0, 1, 0, 90, 90, 90, tecido);
	desenhaGeralTextura(cadeira, -204, 0, -321, 180, 0, 1, 0, 90, 90, 90, tecido);
	desenhaGeralTextura(cadeira, -221, 0, 33, -90, 0, 1, 0, 90, 90, 90, tecido);
	desenhaGeralTextura(cadeira, -221, 0, -90, -90, 0, 1, 0, 90, 90, 90, tecido);
	desenhaGeralTextura(cadeira, -221, 0, -213, -90, 0, 1, 0, 90, 90, 90, tecido);
}

void DesenhaLamps() {
	glColor3f(0.9, 0.9, 0.5);
	desenhaGeral(luminaria, 250, 65, -13, 0, 0, 0, 0, 90, 90, 90);
	desenhaGeral(luminaria, -250, 65, -13, 0, 0, 0, 0, 90, 90, 90);
	desenhaGeral(luminaria, -20, 65, -380, 0, 0, 0, 0, 90, 90, 90);
}

void DesenhaObjExtra() {
	
	glPushMatrix();
	glTranslatef(0, 297, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	DesenhaObjeto(lamp);
	glPopMatrix();

	glColor3f(0.90, 0.90, 0.90);
	SetaEscalaTextura(1, 1);
	desenhaGeralTextura(interruptor, -80, 105, 397.5, -180, 0, 1, 0, 120, 120, 120, metalico);

	// tela da lousa e lousa
	SetaEscalaTextura(1, 1);
	desenhaGeralTextura(lousa, 90, 165, 397, 180, 1, 0, 0, 2.5, 1.30, 0.5, telalousa);
	desenhaGeral(molduralousa, 90, 165, 397.5, -180, 0, 1, 0, 0.5, 0.65, 0.5);

	// retrato
	SetaEscalaTextura(1.5, 1.5);
	desenhaGeralTextura(quadro, 0, 185, -399.5, 180, 0, 0, 1, 90, 90, 90, foto);

	glColor3f(0.15, 0.15, 0.15);
	desenhaGeral(lixeira, -60, 0, 380, -180, 0, 1, 0, 120, 120, 120);
}

// Desenha toda a cena
void Desenha(void) {
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
	glLightfv(GL_LIGHT3, GL_POSITION, posLuz4);

	glLightfv(GL_LIGHT4, GL_POSITION, posLuz5);
	glLightfv(GL_LIGHT4, GL_AMBIENT, luzAmb5);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, luzDif5);
	glLightfv(GL_LIGHT4, GL_SPECULAR, luzEsp5);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dirLuz5);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT5, GL_POSITION, posLuz6);
	glLightfv(GL_LIGHT5, GL_AMBIENT, luzAmb6);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, luzDif6);
	glLightfv(GL_LIGHT5, GL_SPECULAR, luzEsp6);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, dirLuz6);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT6, GL_POSITION, posLuz7);
	glLightfv(GL_LIGHT6, GL_AMBIENT, luzAmb7);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, luzDif7);
	glLightfv(GL_LIGHT6, GL_SPECULAR, luzEsp7);
	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, dirLuz7);
	glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10.0);

	// Desenha todos os elementos da cena
	DesenhaSala();
	DesenhaPorta();
	DesenhaJanela();
	DesenhaVentiladores();
	DesenhaMesas();
	DesenhaMonitores();
	DesenhaTeclados();
	DesenhaMouses();
	DesenhaCadeiras();
	DesenhaLamps();
	DesenhaObjExtra();

	// Faz a troca dos buffers
	glutSwapBuffers();
}

// Gira hélice do ventilador
void AnimacaoHelice(int value)
{
	// Muda o angulo de rotacao, e se chegar a 360, passa para zero
	if (sentido)
	{
		if (--angulo > 360.0f)
			angulo = 0.0f;
	}
	else
	{
		if (++angulo > 360.0f)
			angulo = 0.0f;
	}

	glutPostRedisplay();
	glutTimerFunc(5, AnimacaoHelice, 1);
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
	gluPerspective(ang_cam, fAspect, 0.1, 3000);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
}

// Liga / desliga luzes de acordo com o estado
void SetaLuzes() {
	for (int luz = 0 ; luz < 7 ; luz++) {
		if (luzes[luz]) glEnable(GL_LIGHT0 + luz);
		else glDisable(GL_LIGHT0 + luz);
	}
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

// Funcao callback para eventos de teclas especiais
// de navegacao atraves das setas
void TecladoEspecial(int key, int x, int y)
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

// Funcao callback para eventos de teclado
void Teclado(unsigned char key, int x, int y)
{
	// Trata as diversas teclas
	switch (key) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			luzes[key - '1'] = !luzes[key - '1'];
			SetaLuzes();
			break;
	case 'n': // abre a porta
		if (angle_door < ANGLE_DOOR_MAX)
		{
			angle_door = angle_door + 1;
		}
		break;
	case 'm': // fecha a porta
		if (angle_door > 0)
		{
			angle_door = angle_door - 1;
		}
		break;
	case 'h': // abre a janela (0 > -180) && (0 < 180)
		if (angleE_window > ANGLEE_WINDOW_MAX && angleD_window < ANGLED_WINDOW_MAX)
		{
			angleE_window = angleE_window - 1;
			angleD_window = angleD_window + 1;
		}
		break;
	case 'j': // fecha a janela (-179 > -360) && (179 > 0)
		if (angleE_window > -360 && angleD_window > 0)
		{
			angleE_window = angleE_window + 1;
			angleD_window = angleD_window - 1;
		}
		break;
	case 'g':
		if (sentido)
		{
			sentido = false;
		}
		else
		{
			sentido = true;
		}
		break;
	// Sai do programa
	case 27: // Libera todos os objetos carregados...
		LiberaObjeto(NULL);
		// e materiais
		LiberaMateriais();
		exit(1);
		break;
	}
	// Na próxima iteração por meio de glutMainLoop essa janela será exibida novamente
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

	// Carrega as texturas
	parede = CarregaTextura("texturas/parede.jpg", true);
	chao = CarregaTextura("texturas/chao.jpg", true);
	teto = CarregaTextura("texturas/teto.jpg", true);
	tabua = CarregaTextura("texturas/tabua.jpg", true);
	telalousa = CarregaTextura("texturas/calculo.jpg", true);
	tecido = CarregaTextura("texturas/tecido.jpg", true);
	metalico = CarregaTextura("texturas/metalico.jpg", true);
	grama = CarregaTextura("texturas/grama.jpg", true);
	foto = CarregaTextura("texturas/prof.jpg", true);
	portaTex = CarregaTextura("texturas/portaTex.jpg", true);

	// Seleciona o modo de aplicacao da textura
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, modo);

	// Ajusta iluminacao
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDif1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEsp1);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDif2);

	glLightfv(GL_LIGHT2, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, luzDif3);

	glLightfv(GL_LIGHT3, GL_AMBIENT, luzAmb4);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, luzDif4);

	// Habilita todas as fontes de luz
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_LIGHT6);

	glEnable(GL_LIGHTING);

	// Define coeficientes ambiente e difuso
	// do material
	GLfloat matAmb[4] = {0.2, 0.2, 0.2, 1};
	GLfloat matDif[4] = {1, 1, 1, 1};
	GLfloat matSpec[4] = {1, 1, 1, 1};

	// Material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);

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
	plano = CarregaObjeto("obj/parede.obj", false);
	mesa = CarregaObjeto("obj/mesa.obj", false);
	porta = CarregaObjeto("obj/porta.obj", false);
	janela = CarregaObjeto("obj/janela.obj", false);
	base = CarregaObjeto("obj/base.obj", false);
	helice = CarregaObjeto("obj/helice.obj", false);
	lamp = CarregaObjeto("obj/lampada.obj", false);
	monitor = CarregaObjeto("obj/monitor.obj", false);
	teclado = CarregaObjeto("obj/teclado.obj", false);
	mouse = CarregaObjeto("obj/mouse.obj", false);
	cadeira = CarregaObjeto("obj/cadeira.obj", false);
	interruptor = CarregaObjeto("obj/interruptor.obj", false);
	molduralousa = CarregaObjeto("obj/lousa.obj", false);
	lousa = CarregaObjeto("obj/parede.obj", false);
	lixeira = CarregaObjeto("obj/lixeira.obj", false);
	luminaria = CarregaObjeto("obj/lamp.obj", false);
	quadro = CarregaObjeto("obj/quadro.obj", false);

	SetaLuzes();
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

	// Registra a funcao callback das teclas de movimentação (especiais)
	glutSpecialFunc(TecladoEspecial);

	// Registra a funcao callback de teclas selecionadas
	glutKeyboardFunc(Teclado);

	// Registra a funcao callback para eventos de botoes do mouse
	glutMouseFunc(GerenciaMouse);

	// Registra a funcao callback para eventos de movimento do mouse
	glutMotionFunc(GerenciaMovimentoMouse);

	// Registra a funcao callback para a hélice do ventilador
	glutTimerFunc(60, AnimacaoHelice, 1);

	// Registra a funcao callback de redimensionamento da janela de visualizacao
	glutReshapeFunc(AlteraTamanhoJanela);

	// Chama a funcao responsavel por fazer as inicializacoes
	Inicializa();

	// Inicia o processamento e aguarda interacoes do usuario
	glutMainLoop();

	return 0;
}