#ifndef __MYUTIL_H
#define __MYUTIL_H

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
    float sclZ);

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
    TEX *texture);

#endif