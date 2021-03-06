#include <g3x.h>
#include "../include/motor3d.h"

Motor3D m;
bool* ghostHoles;
#define MAXCOL 25
static G3Xcolor colmap[MAXCOL];
PMat3D* pmats;
int ghostWidth = 30;
int ghostHeight = 30;
PMat3D* geo1;
bool shape = true;

int nbGeo = 2;

typedef struct Geo {
    double x, y, z, theta, sx, sy, sz;
    int type;
} Geo;

Geo* geos;


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


static void animg3x(void)
{
    m.simulate(&m);
}


static void quitg3x()
{
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
    int sizeEye = cols / 22;
    int sizeMouth = sizeEye * 3;
    ghostHoles = calloc(rows * cols, sizeof(bool));
    CoordIdx leftEye;
    leftEye.row = 2 * rows / 3;
    leftEye.col = 1 * cols / 3;
    CoordIdx rightEye;
    rightEye.row = leftEye.row;
    rightEye.col = 2 * cols / 3;
    CoordIdx mouth;
    mouth.row = leftEye.row + 2 * sizeEye;
    mouth.col = cols / 2;


    int nbHoles = 0;

    int nbPointsByLine = 1;
    int nbRows = sizeEye * 2 + 1;

//    for (int i = -sizeEye; i <= sizeEye; ++i) {
//        for (int j = 0; j < nbPointsByLine; ++j) {
//            int x = leftEye.col - nbPointsByLine / 2 + j;
//            int y = leftEye.row + i;
//            int idx = y * cols + x;
//            ghostHoles[idx] = true;
//            nbHoles++;

//            x = rightEye.col - nbPointsByLine / 2 + j;
//            y = rightEye.row + i;
//            idx = y * cols + x;
//            ghostHoles[idx] = true;
//            nbHoles++;
//        }
//        if (i < 0) {
//            nbPointsByLine += 2;
//        }
//        else {
//            nbPointsByLine -= 2;
//        }
//    }

//    nbPointsByLine = sizeMouth;
//    for (int i = 0 ; i <= sizeMouth; ++i) {
//        for (int j = 0; j < nbPointsByLine; ++j) {
//            int x = mouth.col - nbPointsByLine / 2 + j;
//            int y = mouth.row + i;
//            int idx = y * cols + x;
//            ghostHoles[idx] = true;
//            nbHoles++;
//         }
//    }

    return nbHoles;
}

static void modelateGhost(int cols, int rows) {
    double k = 0.03 * (Fe * Fe);
    double xi = 0.013 * Fe;

    double start_x = -1.5;
    double end_x = 1.5;
    double start_y = -1.5;
    double end_y = 1.5;
    double z = 1;

    double step_x = (fabs(start_x) + fabs(end_x)) / cols;
    double step_y = (fabs(start_y) + fabs(end_y)) / rows;


    // PMats
    int nbHoles = modelateHoles(cols, rows);
    int nbPmatsWithoutHoles = cols * rows - nbHoles;
    int nbPmatsWithHoles = cols * rows;
    pmats = malloc(sizeof(PMat3D) * nbPmatsWithHoles);
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int idx = coordToIdx(j, i, cols);
            if (!ghostHoles[idx]) {
                float mass = 1.0;
                // Lest the ghost on border
                if (i == 0 || j == 0 || i == rows - 1 || j == cols - 1) {
                    mass = 0.5f;
                }
                PMat3DMovableInit(&(pmats[idx]), (Point3) { start_x + j * step_x, start_y + i * step_y, z }, (Vector3) { 0, 0, 0}, mass);
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
    int gravity = 1;
    Motor3DInit(&m, nbPmatsWithoutHoles + nbGeo, nbLinksWithoutHoles + gravity + nbGeo * nbPmatsWithoutHoles);
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

    if (gravity != 0) {
        Link3D* gravity = malloc(sizeof(Link3D));
        GravityLink3DInit(gravity);
        gravity->connect(gravity, m.pmats[0], m.pmats[nbPmatsWithoutHoles - 1]);
        m.links[nbLinksWithoutHoles] = gravity;
    }

    // Spheres
    for (int i = 0; i < nbGeo; ++i) {
        printf("%d ", i);
        Geo geoData = geos[i];
        PMat3D* geo = malloc(sizeof(PMat3D));

        G3Xhmat ScaleMat;
        G3Xhmat RotMat;
        G3Xhmat TransMat;
        G3Xhmat InvScaleMat;
        G3Xhmat InvRotMat;
        G3Xhmat InvTransMat;
        G3Xhmat transformWorldToCan;
        G3Xhmat transformWorldToCanTemp;
        G3Xhmat transformCanToWorld;
        G3Xhmat transformCanToWorldTemp;

        g3x_MakeTranslationXYZ(TransMat, geoData.x, geoData.y, geoData.z);
        g3x_MakeHomothetieXYZ(ScaleMat, geoData.sx, geoData.sy, geoData.sz);
        g3x_MakeRotationY(RotMat, geoData.theta * 0.0174);
        g3x_MakeTranslationXYZ(InvTransMat, -geoData.x, -geoData.y, -geoData.z);
        g3x_MakeHomothetieXYZ(InvScaleMat, 1.0/geoData.sx, 1.0/geoData.sy, 1.0/geoData.sz);
        g3x_MakeRotationY(InvRotMat, -geoData.theta * 0.0174);
        // S*R*T -> world to can
        g3x_ProdHMat(ScaleMat, RotMat, transformWorldToCanTemp);
        g3x_ProdHMat(transformWorldToCanTemp, TransMat, transformWorldToCan);
        // T * R * S -> can to world
        g3x_ProdHMat(InvTransMat, InvRotMat, transformCanToWorldTemp);
        g3x_ProdHMat(transformCanToWorldTemp, InvScaleMat, transformCanToWorld);

        switch(geoData.type) {
            case 0:
                PMat3DCube(geo, (Point3) { 0, 0, 0 }, transformCanToWorld, transformWorldToCan);
                break;
            default:
                PMat3DSphere(geo, (Point3) { 0, 0, 0 }, transformCanToWorld, transformWorldToCan);
                break;
        }

        geo->radius = geoData.sx;
        m.pmats[nbPmatsWithoutHoles - 1 + gravity + i] = geo;

        for (int j = 0; j < nbPmatsWithoutHoles; ++j) {
            Link3D* geoLink = malloc(sizeof(Link3D));
            GeoLink3DInit(geoLink, k * 21, xi * 125);
            geoLink->connect(geoLink, geo, m.pmats[j]);
            m.links[nbLinksWithoutHoles + gravity + j + i * nbPmatsWithoutHoles] = geoLink;
        }
    }


}

void drawVertexTriangle(PMat3D* p1, PMat3D* p2, PMat3D* p3) {
//    glTexCoord2f(0.0, 0.0);
    glVertex3f(p1->position.x, p1->position.y, p1->position.z);
//    glTexCoord2f(1.0, 0.0);
    glVertex3f(p2->position.x, p2->position.y, p2->position.z);
//    glTexCoord2f(0.0, 1.0);
    glVertex3f(p3->position.x, p3->position.y, p3->position.z);
}

void drawGhost() {
    glBegin(GL_TRIANGLES); //Begin triangle coordinates

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    for(int i = 0; i < ghostWidth - 1; ++i) {
        for(int j = 0; j < ghostHeight; ++j) {
            int idx = coordToIdx(i, j, ghostWidth);
            int idxRight = coordToIdx(i + 1, j, ghostWidth);
            if (ghostHoles[idx] || ghostHoles[idxRight]) {
                continue;
            }

            if (j < ghostHeight - 1) {
                int idxBottom = coordToIdx(i, j + 1, ghostWidth);
                if (ghostHoles[idxBottom]) {
                    idxBottom = coordToIdx(i + 1, j + 1, ghostWidth);
                }
                if (!ghostHoles[idxBottom]) {
                    drawVertexTriangle(&(pmats[idx]), &(pmats[idxRight]), &(pmats[idxBottom]));
                }
            }

            if (j > 0) {
                int idxTopRight = coordToIdx(i + 1, j - 1, ghostWidth);
                if (ghostHoles[idxTopRight]) {
                    idxTopRight = coordToIdx(i , j - 1, ghostWidth);
                }
                if (!ghostHoles[idxTopRight]) {
                    drawVertexTriangle(&(pmats[idx]), &(pmats[idxRight]), &(pmats[idxTopRight]));
                }
            }

        }
    }
    glEnd(); //End triangle coordinates
}


static void drawg3x()
{
    if(shape) {
        g3x_Material(white,ambi,diff,spec,shin,1.);
        drawGhost();
        float percent = 0.90;
        g3x_Material(black,ambi,diff,spec,shin,1.);

        for (int i = 0; i < nbGeo; ++i) {
            Geo geo = geos[i];
            glPushMatrix();
                glTranslatef(-geo.x,-geo.y, -geo.z);
                glRotatef(-geo.theta,0.,1.,0.);
                glScalef(1.0/geo.sx * percent, 1.0/geo.sy * percent, 1.0/geo.sz * percent);
                switch (geo.type) {
                    case 0:
                        glutSolidCube(2.);
                        break;
                    default:
                        glutSolidSphere(1, 16, 16);
                        break;
                }

            glPopMatrix();
        }
    } else {
        m.draw(&m);
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

    Geo sphere1 = { 0, 0, 0, 30, 1, 2, 2, 1 };
    Geo sphere2 = { 1, 0, 1, 0, 2, 2, 2, 1 };
    Geo sphere3 = { 0, 1, 0.3, 0, 2, 2, 2, 1 };
    Geo cube1 = { 0.7, 1, 0.5, 20, 2, 2, 1.5, 0 };
    Geo cube2 = { -0.3, 1, 0.7, 0, 2, 2, 2, 0 };
    nbGeo = 5;
    geos = malloc(nbGeo * sizeof(Geo));
    geos[0] = sphere1;
    geos[1] = cube1;
    geos[2] = cube2;
    geos[3] = sphere2;
    geos[4] = sphere3;


    int width = 1024, height = 512;
    modelateGhost(ghostWidth, ghostHeight);
    glEnable(GL_TEXTURE_2D);
    /* creation de la fenetre - titre et tailles (pixels) */
    g3x_InitWindow(*argv,width,height);
    g3x_SetPerspective(40.,100.,1.);
    /* position, orientation de la caméra */
    //g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});
    g3x_SetCameraCartesian((G3Xpoint){0,0.001, 5}, (G3Xpoint){0.,0.,1.});
    g3x_SetScrollWidth(6);

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
