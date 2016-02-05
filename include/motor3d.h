#ifndef MOTOR3D_H
#define MOTOR3D_H

#include "pmat3d.h"
#include "link3d.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Motor3D {
    PMat3D* pmats;
    Link3D* links;
    int nbPmats;
    int nbLinks;
    void (*simulate)(struct Motor3D*);
    void (*draw)(struct Motor3D*);
} Motor3D;

static void simulate3D(Motor3D* othis);

static void drawMotor3D(Motor3D* othis);

void Motor3DInit(Motor3D* othis, int nbPmats, int nbLinks);

void Motor3DDestroy(Motor3D* othis);

#endif

