#include "../include/link3d.h"

static void connect3D(Link3D* othis, PMat3D* p1, PMat3D* p2) {
    othis->p1 = p1;
    othis->p2 = p2;
    othis->l0 = p1->distance(p1, p2);
}

static void spring3D(Link3D* othis) {
    double d = othis->p1->distance(othis->p1, othis->p2);

    double f = - othis->k * (1 - (othis->l0 / d));
    Vector3 p1p2;
    Vec3FromPoint3(&p1p2, &(othis->p1->position), &(othis->p2->position));
    ProdVec3(&p1p2, f);

    Vec3SubVec3(&(othis->p1->force), &p1p2);
    Vec3AddVec3(&(othis->p2->force), &p1p2);
}

static void brake3D(Link3D* othis) {
    Vector3 f = othis->p1->velocity;
    ProdVec3(Vec3SubVec3(&f, &(othis->p2->velocity)), - othis->z);
    Vec3AddVec3(&(othis->p1->force), &f);
    Vec3SubVec3(&(othis->p2->force), &f);
}

static void springBrake3D(Link3D* othis) {
    spring3D(othis);
    brake3D(othis);
}

static void drawLink3D(Link3D* othis) {
   /* g3x_DrawLine(othis->p1->position,
                 othis->p2->position,
                 (G3Xcolor) {0.0, 0.0, 0.0, 1.0},
                 1);*/
}

static void noDraw3D(Link3D* othis) { }

static void gravity3D(Link3D* othis) {
    Vector3 gravity = {0.0f, 0.0f, -0.0981f};
    for (PMat3D* p = othis->p1; p <= othis->p2; ++p) {
        Vec3AddVec3(&(p->force), &gravity);
    }
}

// Constructors

void Link3DInit(Link3D* othis, double k, double z) {
    othis->k = k;
    othis->z = z;
    othis->l0 = 0;
    othis->p1 = NULL;
    othis->p2 = NULL;

    othis->algo = &springBrake3D;
    othis->draw = &drawLink3D;
    othis->connect = &connect3D;
}

void GravityLink3DInit(Link3D* othis) {
    othis->k = 0;
    othis->z = 0;
    othis->l0 = 0;
    othis->p1 = NULL;
    othis->p2 = NULL;

    othis->algo = &gravity3D;
    othis->draw = &noDraw3D;
    othis->connect = &connect3D;
}
