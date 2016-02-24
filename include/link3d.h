#ifndef LINK3D_H
#define LINK3D_H

#include "pmat3d.h"
#include "math3x.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Link3D {
    PMat3D* p1;
    PMat3D* p2;
    double k;
    double z;
    double l0;

    void (*connect)(struct Link3D*, PMat3D*, PMat3D*);
    void (*algo)(struct Link3D*);
    void (*draw)(struct Link3D*);
}Link3D;

static void connect3D(Link3D* othis, PMat3D* p1, PMat3D* p2);
static void spring3D(Link3D* othis);
static void drawLink3D(Link3D* othis);
static void brake3D(Link3D* othis);
static void springBrake3D(Link3D* othis);
static void noDraw3D(Link3D* othis);
static void gravity3D(Link3D* othis);
static void connectLevel3D(Link3D* othis, PMat3D* p1, PMat3D* p2);
static void springBrakeLevel3D(Link3D* othis);
static void springBrakeGeo3D(Link3D* othis);
static void connectGeo3D(Link3D* othis, PMat3D* solid, PMat3D* pmat);
// Constructor

void Link3DInit(Link3D* othis, double k, double z);
void GravityLink3DInit(Link3D* othis);
void LevelLink3DInit(Link3D* othis, double k, double z);
void GeoLink3DInit(Link3D* othis, double k, double z);

#endif
