#ifndef MATHG2X_H
#define MATHG2X_H

#include <g2x.h>

G2Xvector* ProdVec(G2Xvector* v, double s);

G2Xvector* DivVec(G2Xvector* v, double s);
G2Xvector* SubVec(G2Xvector* v, double s);
G2Xvector* AddVec(G2Xvector* v, double s);
G2Xvector* VecProdVec(G2Xvector* v, G2Xvector* v2);
G2Xvector* VecDivVec(G2Xvector* v, G2Xvector* v2);
G2Xvector* VecSubVec(G2Xvector* v, G2Xvector* v2);
G2Xvector* VecAddVec(G2Xvector* v, G2Xvector* v2);

#endif
