#ifndef MOTOR_H
#define MOTOR_H

#include "pmat.h"
#include "link.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Motor {
    PMat* pmats;
    Link* links;
    int nbPmats;
    int nbLinks;
    void (*simulate)(struct Motor*);
    void (*draw)(struct Motor*);
} Motor;

static void simulate(Motor* othis);

static void drawMotor(Motor* othis);

void MotorInit(Motor* othis, int nbPmats, int nbLinks);

void MotorDestroy(Motor* othis);

#endif

