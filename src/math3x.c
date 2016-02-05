#include "../include/math3x.h"
#include <math.h>

Vector3* ProdVec3(Vector3* v, double s) {
    v->x *= s;
    v->y *= s;
    v->z *= s;
    return v;
}

Vector3* DivVec3(Vector3* v, double s) {
    v->x /= s;
    v->y /= s;
    v->z /= s;
    return v;
}

Vector3* SubVec3(Vector3* v, double s) {
    v->x -= s;
    v->y -= s;
    v->z -= s;
    return v;
}

Vector3* AddVec3(Vector3* v, double s) {
    v->x += s;
    v->y += s;
    v->z += s;
    return v;
}

Vector3* Vec3ProdVec3(Vector3* v, Vector3* v2) {
    v->x *= v2->x;
    v->y *= v2->y;
    v->z *= v2->z;
    return v;
}

Vector3* Vec3DivVec3(Vector3* v, Vector3* v2) {
    v->x /= v2->x;
    v->y /= v2->y;
    v->z /= v2->z;
    return v;
}

Vector3* Vec3SubVec3(Vector3* v, Vector3* v2) {
    v->x -= v2->x;
    v->y -= v2->y;
    v->z -= v2->z;
    return v;
}

Vector3* Vec3AddVec3(Vector3* v, Vector3* v2) {
    v->x += v2->x;
    v->y += v2->y;
    v->z += v2->z;
    return v;
}

Vector3* Vec3FromPoint3(Vector3* v, Point3* p, Point3* p2) {
    v->x = p2->x - p->x;
    v->y = p2->y - p->y;
    v->z = p2->z - p->z;
    return v;
}

double Distance3(Point3* p, Point3* p2) {
    double xSqr = p2->x - p->x;
    xSqr *= xSqr;

    double ySqr = p2->y - p->y;
    ySqr *= ySqr;

    double zSqr = p2->z - p->z;
    zSqr *= zSqr;

    return sqrt(xSqr + ySqr + zSqr);
}
