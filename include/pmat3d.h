#ifndef PMAT3D_H
#define PMAT3D_H

#include <g3x.h>
#include "constants.h"
#include "math3x.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct PMat3D {
    Point3 position;
    Vector3 velocity;
    Vector3 force;
    float masse;
    G3Xhmat worldToCan;
    G3Xhmat canToWorld;

    float radius;
    G3Xcolor color;

    void (*algo)(struct PMat3D*);
    void (*draw)(struct PMat3D*);
    double (*distance)(struct PMat3D*, struct PMat3D*);
    bool (*distanceGeo)(struct PMat3D*, struct PMat3D*, Vector3* N);

}PMat3D;

static double distance3D(PMat3D* othis, PMat3D* p);

static void drawPMat3D(PMat3D* othis);

static void algoFix3D(PMat3D* othis);

void setFix3D(PMat3D* othis);

static void algoMovable3D(PMat3D* othis);

static void noDrawPMat3D(PMat3D* othis);

static bool distanceGeoCube(PMat3D* othis, PMat3D* p, Vector3* N);
static bool uselessDistanceGeo(PMat3D* othis, PMat3D* p, Vector3* N);

// CONSTRUCTORS

void PMat3DFixInit(PMat3D* othis, Point3 pos);

void PMat3DMovableInit(PMat3D* othis, Point3 pos, Vector3 vel, float m);

void PMat3DCube(PMat3D* othis, Point3 pos, G3Xhmat canToWorld, G3Xhmat worldToCan);
#endif
