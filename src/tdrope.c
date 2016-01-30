#include <g2x.h>
#include "../include/motor.h"

Motor m;


/* un fonction associee a un bouton 'popup' : */
/* remise aux positions initiales             */
static void reset(void)
{

}

/* la fonction d'initialisation */
static void initg2x(void)
{
    reset();
}

static void drawg2x()
{
    m.draw(&m);
}

static void animg2x(void)
{
    m.simulate(&m);
}


static void quitg2x()
{
  //MotorDestroy(&m);
  fprintf(stderr,"\e[31m->nettoyage et sortie\n\e[0m");
}

static void modelateRope(int nb_pmat, int width, int gravity) {
    double k = 0.7 * (Fe * Fe);
    double y = 200.0;
    double start_x = 50.0;
    double step_x = (width - 2 * start_x) / nb_pmat;
    int nbLinks = nb_pmat - 1 + gravity;
    int nbSpring = nb_pmat - 1;
    PMatFixInit(&(m.pmats[0]), (G2Xpoint) {start_x, y});

    for (int i = 1; i < nb_pmat - 1; ++i) {
        PMatMovableInit(&(m.pmats[i]), (G2Xpoint) { start_x + i * step_x, y }, (G2Xvector) { 0, 0}, 1.0);
    }
    start_x += (nb_pmat - 1) * step_x;
    PMatFixInit(&(m.pmats[nb_pmat - 1]), (G2Xpoint) {start_x, y});

    for (int i = 0; i < nbSpring; ++i) {
        LinkInit(&(m.links[i]), k, 0.1 * Fe);
        m.links[i].connect(&(m.links[i]), &(m.pmats[i]), &(m.pmats[i + 1]));
    }

    for (int i = 0; i < gravity; ++i) {
        GravityLinkInit(&(m.links[nbSpring + i]));
        m.links[nbSpring + i].connect(&(m.links[nbSpring + i]), &(m.pmats[0]), &(m.pmats[nb_pmat - 1]));
    }

}

/*
# Frequence : Fe = 1 / h
# Masse : m = 1
# raideur : k = 1 * Fe^2
# viscosite : z = 0.1 * Fe
*/

int main(int argc, char **argv)
{
    int width = 1024, height = 512;
    int nbPmats = 140;
    int gravity = 1;
    MotorInit(&m, nbPmats, nbPmats - 1 + gravity);
    modelateRope(nbPmats, width, gravity);
    // m.pmats[nbPmats / 2].position.y += 10;

    /* creation de la fenetre - titre et tailles (pixels) */
    g2x_InitWindow(*argv,width,height);
    /* zone graphique reelle associee a la fenetre */
    g2x_SetWindowCoord(0,0,width,height);

    g2x_SetInitFunction(initg2x); /* fonction d'initialisation */
    g2x_SetDrawFunction(drawg2x); /* fonction de dessin        */
    g2x_SetAnimFunction(animg2x); /* fonction d'animation      */
    g2x_SetExitFunction(quitg2x); /* fonction de sorite        */

    /* lancement de la boucle principale */
  return g2x_MainStart();
    /* RIEN APRES CA */
}
