#include <g3x.h>
#include "../include/motor3d.h"

Motor3D m;
#define MAXCOL 25
static G3Xcolor colmap[MAXCOL];


/* un fonction associee a un bouton 'popup' : */
/* remise aux positions initiales             */
static void reset(void)
{

}

/* la fonction d'initialisation */
static void initg3x(void)
{
    reset();
}

static void drawg3x()
{
    m.draw(&m);
}

static void animg3x(void)
{
    m.simulate(&m);
}


static void quitg3x()
{
  //MotorDestroy(&m);
  fprintf(stderr,"\e[31m->nettoyage et sortie\n\e[0m");
}

static void modelateFlag(int nb_pmat, int width, int gravity) {
    double k = 0.7 * (Fe * Fe);
    double y = 0.0;
    double z = 0;
    double start_x = -1.0f;
    double step_x = 2.0f / nb_pmat;
    int nbLinks = nb_pmat - 1 + gravity;
    int nbSpring = nb_pmat - 1;
    PMat3DFixInit(&(m.pmats[0]), (Point3) {start_x, y, z});

    for (int i = 1; i < nb_pmat - 1; ++i) {
        PMat3DMovableInit(&(m.pmats[i]), (Point3) { start_x + i * step_x, y, z }, (Vector3) { 0, 0, 0}, 1.0);
    }
    start_x += (nb_pmat - 1) * step_x;
    PMat3DFixInit(&(m.pmats[nb_pmat - 1]), (Point3) {start_x, y, z});

    for (int i = 0; i < nbSpring; ++i) {
        Link3DInit(&(m.links[i]), k, 0.1 * Fe);
        m.links[i].connect(&(m.links[i]), &(m.pmats[i]), &(m.pmats[i + 1]));
    }

    for (int i = 0; i < gravity; ++i) {
        GravityLink3DInit(&(m.links[nbSpring + i]));
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
    int nbPmats = 32;
    int gravity = 1;
    Motor3DInit(&m, nbPmats, nbPmats - 1 + gravity);
    modelateFlag(nbPmats, width, gravity);

    /* creation de la fenetre - titre et tailles (pixels) */
    g3x_InitWindow(*argv,width,height);
    g3x_SetPerspective(40.,100.,1.);
    /* position, orientation de la caméra */
    //g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});
    g3x_SetCameraCartesian((G3Xpoint){0.,-1.5,0.}, (G3Xpoint){0.,0.,0.});

    /* fixe les param. colorimétriques du spot lumineux */
   /* lumiere blanche (c'est les valeurs par defaut)   */
     g3x_SetLightAmbient (1.,1.,1.);
     g3x_SetLightDiffuse (1.,1.,1.);
     g3x_SetLightSpecular(1.,1.,1.);

     /* fixe la position et la direction du spot lumineux */
       /* (c'est les valeurs par defaut)                    */
     g3x_SetLightPosition (10.,10.,10.);
     g3x_SetLightDirection( 0., 0., 0.);

     /* initialisation d'une carte de couleurs */
     g3x_FillColorMap(colmap,MAXCOL);


     g3x_SetExitFunction(quitg3x  );     /* la fonction de sortie */
     g3x_SetDrawFunction(drawg3x);     /* la fonction de Dessin */
       g3x_SetAnimFunction(animg3x);


    /* lancement de la boucle principale */
    return g3x_MainStart();
    /* RIEN APRES CA */
}
