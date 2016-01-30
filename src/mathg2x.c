#include "../include/mathg2x.h"

G2Xvector* ProdVec(G2Xvector* v, double s) {
    v->x *= s;
    v->y *= s;
    return v;
}

G2Xvector* DivVec(G2Xvector* v, double s) {
    v->x /= s;
    v->y /= s;
    return v;
}

G2Xvector* SubVec(G2Xvector* v, double s) {
    v->x -= s;
    v->y -= s;
    return v;
}

G2Xvector* AddVec(G2Xvector* v, double s) {
    v->x += s;
    v->y += s;
    return v;
}

G2Xvector* VecProdVec(G2Xvector* v, G2Xvector* v2) {
    v->x *= v2->x;
    v->y *= v2->y;
    return v;
}

G2Xvector* VecDivVec(G2Xvector* v, G2Xvector* v2) {
    v->x /= v2->x;
    v->y /= v2->y;
    return v;
}

G2Xvector* VecSubVec(G2Xvector* v, G2Xvector* v2) {
    v->x -= v2->x;
    v->y -= v2->y;
    return v;
}

G2Xvector* VecAddVec(G2Xvector* v, G2Xvector* v2) {
    v->x += v2->x;
    v->y += v2->y;
    return v;
}
