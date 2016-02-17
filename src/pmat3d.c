#include "../include/pmat3d.h"

/* des couleurs prédéfinies */
static G3Xcolor rouge  ={1.0,0.0,0.0};
static G3Xcolor jaune  ={1.0,1.0,0.0};
static G3Xcolor vert   ={0.0,1.0,0.0};
static G3Xcolor cyan   ={0.0,1.0,1.0};
static G3Xcolor bleu   ={0.0,0.0,1.0};
static G3Xcolor magenta={1.0,0.0,1.0};
static G3Xcolor truc={1.0,0.4,0.3};
/* paramètres géométriques */
static double angle= 0.00;
static double rayon= 0.66;
/* paramètres de lumière/matière */
static double alpha= 0.5;
static double ambi = 0.2;
static double diff = 0.3;
static double spec = 0.4;
static double shin = 0.5;

static double distance3D(PMat3D* othis, PMat3D* p) {
    return Distance3(&(othis->position), &(p->position));
}

static void drawPMat3D(PMat3D* othis) {
    glPushMatrix();
        glTranslatef(othis->position.x, othis->position.y, othis->position.z);
        g3x_Material(rouge,ambi,diff,spec,shin,1.);
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

    othis->radius = 0.009;
    othis->color[0] = 1.0;
    othis->color[1] = 0.0;
    othis->color[2] = 0.0;
    othis->color[3] = 1.0;
}
