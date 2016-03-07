#include "../include/pmat3d.h"
#include "../include/constants.h"

static double distance3D(PMat3D* othis, PMat3D* p) {
    return Distance3(&(othis->position), &(p->position));
}

static bool uselessDistanceGeo(PMat3D* othis, PMat3D* p, Vector3* N) {
    return false;
}

static bool distanceGeoCube(PMat3D* othis, PMat3D* p, Vector3* N) {
    G3Xpoint point;
    G3Xpoint pointCano;
    point[0] = p->position.x;
    point[1] = p->position.y;
    point[2] = p->position.z;
    g3x_ProdHMatPoint(othis->worldToCan, point, pointCano);

    double d = MAX(MAX(fabs(pointCano[0]), fabs(pointCano[1])), fabs(pointCano[2]));

    if (d > 1) {
        return false;
    }

    G3Xvector N3X = { 0, 0, 0};
    G3Xvector N3XWorld;
    if (d == abs(pointCano[0])) {
        N3X[0] = pointCano[0] * (1.0 / d - 1.0);
    }
    else if (d == abs(pointCano[1])) {
        N3X[1] = pointCano[1] * (1.0 / d - 1.0);
    } else {
        N3X[2] = pointCano[2] * (1.0 / d - 1.0);
    }

    g3x_ProdHMatVector(othis->canToWorld, N3X, N3XWorld);
    N->x = N3XWorld[0];
    N->y = N3XWorld[1];
    N->z = N3XWorld[2];

    return true;
}

static bool distanceGeoSphere(PMat3D* othis, PMat3D* p, Vector3* N) {
    G3Xpoint point;
    G3Xpoint pointCano;
    point[0] = p->position.x;
    point[1] = p->position.y;
    point[2] = p->position.z;
    g3x_ProdHMatPoint(othis->worldToCan, point, pointCano);

    double d = pointCano[0] * pointCano[0] + pointCano[1] * pointCano[1] + pointCano[2] * pointCano[2];

    if (d > 1) {
        return false;
    }
    d = 1 / sqrt(d) - 1;
    G3Xvector N3X = { 0, 0, 0};
    G3Xvector N3XWorld;
    N3X[0] = pointCano[0] * d;
    N3X[1] = pointCano[1] * d;
    N3X[2] = pointCano[2] * d;

    g3x_ProdHMatVector(othis->canToWorld, N3X, N3XWorld);
    N->x = N3XWorld[0];
    N->y = N3XWorld[1];
    N->z = N3XWorld[2];

    return true;
}

static void drawPMat3D(PMat3D* othis) {
    glPushMatrix();
        glTranslatef(othis->position.x, othis->position.y, othis->position.z);
        g3x_Material(red,ambi,diff,spec,shin,1.);
        glutSolidSphere(othis->radius,40,40);
    glPopMatrix();
}

static void noDrawPMat3D(PMat3D* othis) { };

static void algoFix3D(PMat3D* othis) {
    othis->velocity.x = 0;
    othis->velocity.y = 0;
    othis->velocity.z = 0;
}

static void algoMovable3D(PMat3D* othis) {
    // Leap frog
    othis->velocity.x += H / othis->masse * othis->force.x;
    othis->velocity.y += H / othis->masse * othis->force.y;
    othis->velocity.z += H / othis->masse * othis->force.z;
    othis->position.x += H * othis->velocity.x;
    othis->position.y += H * othis->velocity.y;
    othis->position.z += H * othis->velocity.z;
    othis->force.x = 0;
    othis->force.y = 0;
    othis->force.z = 0;
}

void setFix3D(PMat3D* othis) {
    othis->algo = &algoFix3D;
}

// CONSTRUCTORS

void PMat3DFixInit(PMat3D* othis, Point3 pos) {
    othis->position = pos;

    othis->velocity.x = 0;
    othis->velocity.y = 0;
    othis->velocity.z = 0;

    othis->force.x = 0;
    othis->force.y = 0;
    othis->force.z = 0;

    othis->masse = 1;
    othis->algo = &algoFix3D;
    othis->distance = &distance3D;
    othis->draw = &drawPMat3D;
    othis->distanceGeo = &uselessDistanceGeo;

    othis->radius = 0.01;
    othis->color[0] = 0.0;
    othis->color[1] = 0.0;
    othis->color[2] = 1.0;
    othis->color[3] = 1.0;
}

void PMat3DMovableInit(PMat3D* othis, Point3 pos, Vector3 vel, float m) {
    othis->position = pos;
    othis->velocity = vel;

    othis->force.x = 0;
    othis->force.y = 0;
    othis->force.z = 0;

    othis->masse = m;
    othis->algo = &algoMovable3D;
    othis->distance = &distance3D;
    othis->draw = &drawPMat3D;
    othis->distanceGeo = &uselessDistanceGeo;

    othis->radius = 0.009;
    othis->color[0] = 1.0;
    othis->color[1] = 0.0;
    othis->color[2] = 0.0;
    othis->color[3] = 1.0;
}

void PMat3DCube(PMat3D* othis, Point3 pos, G3Xhmat canToWorld, G3Xhmat worldToCan) {
    othis->position = pos;
    othis->velocity.x = 0;
    othis->velocity.y = 0;
    othis->velocity.z = 0;

    othis->force.x = 0;
    othis->force.y = 0;
    othis->force.z = 0;

    G3Xcopymat(othis->worldToCan, worldToCan);
    G3Xcopymat(othis->canToWorld, canToWorld);

    othis->masse = 0;
    othis->algo = &algoFix3D;
    othis->distance = &distance3D;
    othis->draw = &drawPMat3D;
    othis->distanceGeo = &distanceGeoCube;

    othis->radius = 0.009;
    othis->color[0] = 1.0;
    othis->color[1] = 0.0;
    othis->color[2] = 0.0;
    othis->color[3] = 1.0;
}

void PMat3DSphere(PMat3D* othis, Point3 pos, G3Xhmat canToWorld, G3Xhmat worldToCan) {
    othis->position = pos;
    othis->velocity.x = 0;
    othis->velocity.y = 0;
    othis->velocity.z = 0;

    othis->force.x = 0;
    othis->force.y = 0;
    othis->force.z = 0;

    G3Xcopymat(othis->worldToCan, worldToCan);
    G3Xcopymat(othis->canToWorld, canToWorld);

    othis->masse = 0;
    othis->algo = &algoFix3D;
    othis->distance = &distance3D;
    othis->draw = &drawPMat3D;
    othis->distanceGeo = &distanceGeoSphere;

    othis->radius = 0.009;
    othis->color[0] = 1.0;
    othis->color[1] = 0.0;
    othis->color[2] = 0.0;
    othis->color[3] = 1.0;
}
