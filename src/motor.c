#include "../include/motor.h"

static void simulate(Motor* othis) {
    for (int i = 0; i < othis->nbPmats; ++i) {
        othis->pmats[i].algo(&(othis->pmats[i]));
    }
    for (int i = 0; i < othis->nbLinks; ++i) {
        othis->links[i].algo(&(othis->links[i]));
    }
}

static void drawMotor(Motor* othis) {
    for (int i = 0; i < othis->nbPmats; ++i) {
        othis->pmats[i].draw(&(othis->pmats[i]));
    }
    for (int i = 0; i < othis->nbLinks; ++i) {
        othis->links[i].draw(&(othis->links[i]));
    }
}

void MotorInit(Motor* othis, int nbPmats, int nbLinks) {
    othis->pmats = malloc(sizeof(PMat) * nbPmats);
    othis->links = malloc(sizeof(Link) * nbLinks);
    othis->nbPmats = nbPmats;
    othis->nbLinks = nbLinks;

    othis->simulate = &simulate;
    othis->draw = &drawMotor;
}

void MotorDestroy(Motor* othis) {
    free(othis->pmats);
    free(othis->links);
}
