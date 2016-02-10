#include <g3x.h>
#include "../include/motor3d.h"

Motor3D m;
bool* ghostHoles;
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

typedef struct { int col; int row; } CoordIdx;

static int coordToIdx(int col, int row, int cols) {
    return row * cols + col;
}

static CoordIdx idxToCoord(int idx, int cols) {
    CoordIdx res;
    res.col = idx % cols;
    res.row = idx / cols;
    return res;
}


static int modelateHoles(int cols, int rows) {
    ghostHoles = calloc(rows * cols, sizeof(bool));
    CoordIdx leftEye;
    leftEye.row = 2 * rows / 3;
    leftEye.col = 1 * cols / 3;
    CoordIdx rightEye;
    rightEye.row = leftEye.row;
    rightEye.col = 2 * cols / 3;


    // ______ !!!!!!! ______ Pensez à le mettre à jours pour les autres formes !!!!!
    int nbHoles = 0;

    int sizeEye = cols / 8;
    int nbPointsByLine = 1;
    int nbRows = sizeEye * 2 + 1;

    for (int i = -sizeEye; i <= sizeEye; ++i) {
        for (int j = 0; j < nbPointsByLine; ++j) {
            int x = leftEye.col - nbPointsByLine / 2 + j;
            int y = leftEye.row + i;
            int idx = y * cols + x;
            ghostHoles[idx] = true;
            nbHoles++;

            x = rightEye.col - nbPointsByLine / 2 + j;
            y = rightEye.row + i;
            idx = y * cols + x;
            ghostHoles[idx] = true;
            nbHoles++;
        }
        if (i < 0) {
            nbPointsByLine += 2;
        }
        else {
            nbPointsByLine -= 2;
        }
    }

    return nbHoles;
}

static void modelateGhost(int cols, int rows) {
    double k = 0.7 * (Fe * Fe);
    double xi = 0.1 * Fe;

    double start_x = -1.0;
    double end_x = 1.0;
    double start_y = -1.0;
    double end_y = 1.0;
    double z = 1;

    double step_x = (fabs(start_x) + fabs(end_x)) / cols;
    double step_y = (fabs(start_y) + fabs(end_y)) / rows;


    // PMats
    int nbHoles = modelateHoles(cols, rows);
    int nbPmatsWithoutHoles = cols * rows - nbHoles;
    int nbPmatsWithHoles = cols * rows;
    PMat3D* pmats = malloc(sizeof(PMat3D) * nbPmatsWithHoles);
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int idx = coordToIdx(j, i, cols);
            if (!ghostHoles[idx]) {
                PMat3DMovableInit(&(pmats[idx]), (Point3) { start_x + j * step_x, start_y + i * step_y, z }, (Vector3) { 0, 0, 0}, 1.0);
            }
        }
    }

    // Links
    int nbLinksWithHoles = 6 * cols * rows - 5 * cols - 5 * rows + 2;
    int nbLinksWithoutHoles = 0;
    Link3D* links = malloc(sizeof(Link3D) * nbLinksWithHoles);
    for(int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int idx = coordToIdx(col, row, cols);
            // If main point is not a hole
            if (!ghostHoles[idx]) {
                PMat3D* mainPMat = &(pmats[idx]);
                PMat3D* secondPMat = NULL;
                Link3D* link = NULL;
                int x, y;
                int idx2;

                //right link
                x = col + 1;
                idx2 = coordToIdx(x, row, cols);
                if (x < cols && !ghostHoles[idx2]) {
                    secondPMat = &(pmats[idx2]);
                    link = &(links[nbLinksWithoutHoles]);
                    Link3DInit(link, k, xi);
                    link->connect(link, mainPMat, secondPMat);
                    nbLinksWithoutHoles++;
                }

                // sur right link
                x = col + 2;
                idx2 = coordToIdx(x, row, cols);
                if (x < cols && !ghostHoles[idx2] ) {
                    secondPMat = &(pmats[idx2]);
                    link = &(links[nbLinksWithoutHoles]);
                    Link3DInit(link, k, xi);
                    link->connect(link, mainPMat, secondPMat);
                    nbLinksWithoutHoles++;
                }

                // bottom link
                y = row + 1;
                idx2 = coordToIdx(col, y, cols);
                if (y < rows && !ghostHoles[idx2] ) {
                    secondPMat = &(pmats[idx2]);
                    link = &(links[nbLinksWithoutHoles]);
                    Link3DInit(link, k, xi);
                    link->connect(link, mainPMat, secondPMat);
                    nbLinksWithoutHoles++;
                }

                // sur bottom link
                y = row + 2;
                idx2 = coordToIdx(col, y, cols);
                if (y < rows && !ghostHoles[idx2] ) {
                    secondPMat = &(pmats[idx2]);
                    link = &(links[nbLinksWithoutHoles]);
                    Link3DInit(link, k, xi);
                    link->connect(link, mainPMat, secondPMat);
                    nbLinksWithoutHoles++;
                }

                // bottom right link
                y = row + 1;
                x = col + 1;
                idx2 = coordToIdx(x, y, cols);
                if (y < rows && x < cols && !ghostHoles[idx2] ) {
                    secondPMat = &(pmats[idx2]);
                    link = &(links[nbLinksWithoutHoles]);
                    Link3DInit(link, k, xi);
                    link->connect(link, mainPMat, secondPMat);
                    nbLinksWithoutHoles++;
                }

                // up right link
                y = row - 1;
                x = col + 1;
                idx2 = coordToIdx(x, y, cols);
                if (y >= 0 && x < cols && !ghostHoles[idx2] ) {
                    secondPMat = &(pmats[idx2]);
                    link = &(links[nbLinksWithoutHoles]);
                    Link3DInit(link, k, xi);
                    link->connect(link, mainPMat, secondPMat);
                    nbLinksWithoutHoles++;
                }

            }
        }
    }


    // Pass to physic motor only what is necessary
    Motor3DInit(&m, nbPmatsWithoutHoles, nbLinksWithoutHoles);
    int cpt = 0;
    for (int i = 0; i < nbPmatsWithHoles; ++i) {
        if (!ghostHoles[i]) {
            m.pmats[cpt] = &(pmats[i]);
            cpt++;
        }
    }
    for (int i = 0; i < nbLinksWithoutHoles; ++i) {
        m.links[i] = &(links[i]);
    }

    // Very ugly just for test, Add 1 to motor init before uncomment !!!
//    GravityLink3DInit(&(m.links[nbLinksWithoutHoles]));
//    m.links[nbLinksWithoutHoles].connect(&(m.links[nbLinksWithoutHoles]), &(m.pmats[0]), &(m.pmats[nbPmatsWithoutHoles - 1]));

}

/*
# Frequence : Fe = 1 / h
# Masse : m = 1
# raideur : k = 1 * Fe^2
# viscosite : z = 0.1 * Fe
*/

int main(int argc, char **argv)
{
    printf("_____________________________________ \n");
    int width = 1024, height = 512;
    modelateGhost(20, 20);
    m.pmats[30]->force.z += 0.00001;
//    printf("distance %f %p\n", m.pmats[0].distance(&(m.pmats[0]), &(m.pmats[1])), &(m.pmats[0]));
//    for (int i = 0 ; i < 80 ; ++i) {
//        printf("%f %f %f %p %p\n", m.links[i].l0, m.links[i].p1->position.z, m.links[i].p1->distance(m.links[i].p1, m.links[i].p2),m.links[i].p1, m.links[i].p2);
//    }

    /* creation de la fenetre - titre et tailles (pixels) */
    g3x_InitWindow(*argv,width,height);
    g3x_SetPerspective(40.,100.,1.);
    /* position, orientation de la caméra */
    //g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});
    g3x_SetCameraCartesian((G3Xpoint){0,0.001, 5}, (G3Xpoint){0.,0.,1.});

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
