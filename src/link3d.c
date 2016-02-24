#include "../include/link3d.h"

static void connect3D(Link3D* othis, PMat3D* p1, PMat3D* p2) {
    othis->p1 = p1;
    othis->p2 = p2;
    othis->l0 = p1->distance(p1, p2);
}

static void connectLevel3D(Link3D* othis, PMat3D* solid, PMat3D* pmat) {
    othis->p1 = pmat;
    othis->p2 = solid;
    othis->l0 = solid->radius;
}

static void connectGeo3D(Link3D* othis, PMat3D* solid, PMat3D* pmat) {
    othis->p1 = solid;
    othis->p2 = pmat;
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

static void springBrakeLevel3D(Link3D* othis) {
    double d = othis->p1->distance(othis->p1, othis->p2);
    if (d <= othis->l0) {
        spring3D(othis);
        brake3D(othis);
    }
}

static void springBrakeGeo3D(Link3D* othis) {
    Vector3 N;
    if (othis->p1->distanceGeo(othis->p1, othis->p2, &N)) {
        ProdVec3(&N, othis->k);
        Vec3AddVec3(&(othis->p2->force), &N);
        Vector3 f = othis->p1->velocity;
        ProdVec3(Vec3SubVec3(&f, &(othis->p2->velocity)), - othis->z);
        Vec3SubVec3(&(othis->p2->force), &f);
        //printf("%f %f %f \n", N.x, N.y, N.z);
    }
}

static void drawLink3D(Link3D* othis) {
    glLineWidth(2.5);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(othis->p1->position.x, othis->p1->position.y, othis->p1->position.z);
    glVertex3f(othis->p2->position.x, othis->p2->position.y, othis->p2->position.z);
    glEnd();
}

static void noDraw3D(Link3D* othis) { }

static void gravity3D(Link3D* othis) {
    float factor = 5;
    Vector3 gravity = {0.0f, 0.0f, -0.0981f * factor};
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

void LevelLink3DInit(Link3D* othis, double k, double z) {
    othis->k = k;
    othis->z = z;
    othis->l0 = 0;
    othis->p1 = NULL;
    othis->p2 = NULL;

    othis->algo = &springBrakeLevel3D;
    othis->draw = &noDraw3D;
    othis->connect = &connectLevel3D;
}

void GeoLink3DInit(Link3D* othis, double k, double z) {
    othis->k = k;
    othis->z = z;
    othis->l0 = 0;
    // P1 is the geo point
    othis->p1 = NULL;
    // p2 is the move point
    othis->p2 = NULL;

    othis->algo = &springBrakeGeo3D;
    othis->draw = &noDraw3D;
    othis->connect = &connectGeo3D;
}
