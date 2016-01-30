#include "../include/link.h"

static void connect(Link* othis, PMat* p1, PMat* p2) {
    othis->p1 = p1;
    othis->p2 = p2;
    othis->l0 = p1->distance(p1, p2);
}

static void spring(Link* othis) {
    double d = othis->p1->distance(othis->p1, othis->p2);

    double f = - othis->k * (1 - (othis->l0 / d));
    G2Xvector p1p2 = g2x_Vector(othis->p1->position, othis->p2->position);
    ProdVec(&p1p2, f);

    VecSubVec(&(othis->p1->force), &p1p2);
    VecAddVec(&(othis->p2->force), &p1p2);
}

static void brake(Link* othis) {
    G2Xvector f = othis->p1->velocity;
    ProdVec(VecSubVec(&f, &(othis->p2->velocity)), - othis->z);
    VecAddVec(&(othis->p1->force), &f);
    VecSubVec(&(othis->p2->force), &f);
}

static void springBrake(Link* othis) {
    spring(othis);
    brake(othis);
}

static void drawLink(Link* othis) {
    g2x_DrawLine(othis->p1->position,
                 othis->p2->position,
                 (G2Xcolor) {0.0, 0.0, 0.0, 1.0},
                 1);
}

static void noDraw(Link* othis) { }

static void gravity(Link* othis) {
    G2Xvector gravity = {0, -9.1};
    for (PMat* p = othis->p1; p <= othis->p2; ++p) {
        VecAddVec(&(p->force), &gravity);
    }
}

// Constructors

void LinkInit(Link* othis, double k, double z) {
    othis->k = k;
    othis->z = z;
    othis->l0 = 0;
    othis->p1 = NULL;
    othis->p2 = NULL;

    othis->algo = &springBrake;
    othis->draw = &drawLink;
    othis->connect = &connect;
}

void GravityLinkInit(Link* othis) {
    othis->k = 0;
    othis->z = 0;
    othis->l0 = 0;
    othis->p1 = NULL;
    othis->p2 = NULL;

    othis->algo = &gravity;
    othis->draw = &noDraw;
    othis->connect = &connect;
}
