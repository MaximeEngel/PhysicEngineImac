#ifndef LINK_H
#define LINK_H

#include "pmat.h"
#include "mathg2x.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Link {
    PMat* p1;
    PMat* p2;
    double k;
    double z;
    double l0;

    void (*connect)(struct Link*, PMat*, PMat*);
    void (*algo)(struct Link*);
    void (*draw)(struct Link*);
}Link;

static void connect(Link* othis, PMat* p1, PMat* p2);
static void spring(Link* othis);
static void drawLink(Link* othis);
static void brake(Link* othis);
static void springBrake(Link* othis);
static void noDraw(Link* othis);
static void gravity(Link* othis);
// Constructor

void LinkInit(Link* othis, double k, double z);
void GravityLinkInit(Link* othis);

#endif
