#include <GL/glut.h>
#include "bibutil.h"
#include "bibutilNoTex.h"
#include <stdbool.h>

void desenhaGeral(
    OBJ *object,
    float x,
    float y,
    float z,
    float angle,
    float rotX,
    float rotY,
    float rotZ,
    float sclX,
    float sclY,
    float sclZ)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, rotX, rotY, rotZ);
    glScalef(sclX, sclY, sclZ);
    DesenhaObjeto(object);
    glPopMatrix();
}

void desenhaGeralTextura(
    OBJ *object,
    float x,
    float y,
    float z,
    float angle,
    float rotX,
    float rotY,
    float rotZ,
    float sclX,
    float sclY,
    float sclZ,
    TEX *texture)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, rotX, rotY, rotZ);
    glScalef(sclX, sclY, sclZ);
    object->textura = texture->texid;
    DesenhaObjeto(object);
    glPopMatrix();
}