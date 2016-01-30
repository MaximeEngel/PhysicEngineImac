#ifndef PMAT_H
#define PMAT_H

#include <g2x.h>
#include "constants.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct PMat {
    G2Xpoint position;
    G2Xvector velocity;
    G2Xvector force;
    float masse;

    float radius;
    G2Xcolor color;

    void (*algo)(struct PMat*);
    void (*draw)(struct PMat*);
    double (*distance)(struct PMat*, struct PMat*);

}PMat;

static double distance(PMat* othis, PMat* p);

static void drawPMat(PMat* othis);

static void algoFix(PMat* othis);

static void algoMovable(PMat* othis);

// CONSTRUCTORS

void PMatFixInit(PMat* othis, G2Xpoint pos);

void PMatMovableInit(PMat* othis, G2Xpoint pos, G2Xvector vel, float m);
#endif
