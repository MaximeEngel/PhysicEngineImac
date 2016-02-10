#include "../include/motor3d.h"

static void simulate3D(Motor3D* othis) {
    for (int i = 0; i < othis->nbPmats; ++i) {
        othis->pmats[i]->algo(othis->pmats[i]);
    }
    for (int i = 0; i < othis->nbLinks; ++i) {
        othis->links[i]->algo(othis->links[i]);
    }
}

static void drawMotor3D(Motor3D* othis) {
    for (int i = 0; i < othis->nbPmats; ++i) {
        othis->pmats[i]->draw(othis->pmats[i]);
    }
    for (int i = 0; i < othis->nbLinks; ++i) {
        othis->links[i]->draw(othis->links[i]);
    }
}

void Motor3DInit(Motor3D* othis, int nbPmats, int nbLinks) {
    othis->pmats = malloc(sizeof(PMat3D*) * nbPmats);
    othis->links = malloc(sizeof(Link3D*) * nbLinks);
    othis->nbPmats = nbPmats;
    othis->nbLinks = nbLinks;

    othis->simulate = &simulate3D;
    othis->draw = &drawMotor3D;
}

void Motor3DDestroy(Motor3D* othis) {
    free(othis->pmats);
    free(othis->links);
}
