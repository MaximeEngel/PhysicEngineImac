#include "../include/pmat.h"

static double distance(PMat* othis, PMat* p) {
    return g2x_Distance(othis->position, p->position);
}

static void drawPMat(PMat* othis) {
    // g2x_DrawPoint(othis->position, othis->color, othis->radius);
}

static void algoFix(PMat* othis) {
    othis->velocity.x = 0;
    othis->velocity.y = 0;
}

static void algoMovable(PMat* othis) {
    // Leap frog
    othis->velocity.x += H / othis->masse * othis->force.x;
    othis->velocity.y += H / othis->masse * othis->force.y;

    othis->position.x += H * othis->velocity.x;
    othis->position.y += H * othis->velocity.y;

    othis->force.x = 0;
    othis->force.y = 0;
}

// CONSTRUCTORS

void PMatFixInit(PMat* othis, G2Xpoint pos) {
    othis->position = pos;

    othis->velocity.x = 0;
    othis->velocity.y = 0;

    othis->force.x = 0;
    othis->force.y = 0;

    othis->masse = 1;
    othis->algo = &algoFix;
    othis->distance = &distance;
    othis->draw = &drawPMat;

    othis->radius = 5;
    othis->color[0] = 0.0;
    othis->color[1] = 0.0;
    othis->color[2] = 1.0;
    othis->color[3] = 1.0;
}

void PMatMovableInit(PMat* othis, G2Xpoint pos, G2Xvector vel, float m) {
    othis->position = pos;
    othis->velocity = vel;

    othis->force.x = 0;
    othis->force.y = 0;

    othis->masse = m;
    othis->algo = &algoMovable;
    othis->distance = &distance;
    othis->draw = &drawPMat;

    othis->radius = 3;
    othis->color[0] = 1.0;
    othis->color[1] = 0.0;
    othis->color[2] = 0.0;
    othis->color[3] = 1.0;
}
