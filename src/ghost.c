#include <g3x.h>
#include "../include/motor3d.h"
#include <SOIL/SOIL.h>

Motor3D m;
bool* ghostHoles;
#define MAXCOL 25
static G3Xcolor colmap[MAXCOL];
PMat3D* pmats;
int ghostWidth = 30;
int ghostHeight = 30;
PMat3D* sphere1;
bool shape = true;


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
double x = 0, y = 0, z = 1, theta = 60, sx = 1.5, sy = 2, sz = 2;


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


    // ______ !!!!!!! ______ Pensez à le mettre à jours pour les autres formes !!!!!
    int nbHoles = 0;

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

    nbPointsByLine = sizeMouth;
    for (int i = 0 ; i <= sizeMouth; ++i) {
        for (int j = 0; j < nbPointsByLine; ++j) {
            int x = mouth.col - nbPointsByLine / 2 + j;
            int y = mouth.row + i;
            int idx = y * cols + x;
            ghostHoles[idx] = true;
            nbHoles++;
         }
    }

    return nbHoles;
}

static void modelateGhost(int cols, int rows) {
    double k = 0.03 * (Fe * Fe);
    double xi = 0.013 * Fe;

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
    int nbSpheres = 1;
    Motor3DInit(&m, nbPmatsWithoutHoles + nbSpheres, nbLinksWithoutHoles + gravity + nbSpheres * nbPmatsWithoutHoles);
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
    if (nbSpheres != 0 ) {
//        sphere1 = malloc(sizeof(PMat3D));
//        PMat3DFixInit(sphere1, (Point3) { 0, 0, 0 });
//        sphere1->radius = 0.5f;
//        m.pmats[nbPmatsWithoutHoles] = sphere1;

//        for (int i = 0; i < nbPmatsWithoutHoles; ++i) {
//            Link3D* levelLink = malloc(sizeof(Link3D));
//            LevelLink3DInit(levelLink, k * 20, xi * 10);
//            levelLink->connect(levelLink, sphere1, m.pmats[i]);
//            m.links[nbLinksWithoutHoles + 1 + i] = levelLink;
//        }
        sphere1 = malloc(sizeof(PMat3D));
        PMat3DCube(sphere1, (Point3) { 0, 0, 0 }, transformCanToWorld, transformWorldToCan);
        sphere1->radius = 0.5f;
        m.pmats[nbPmatsWithoutHoles] = sphere1;

        for (int i = 0; i < nbPmatsWithoutHoles; ++i) {
            Link3D* geoLink = malloc(sizeof(Link3D));
            GeoLink3DInit(geoLink, k * 20, xi * 120);
            geoLink->connect(geoLink, sphere1, m.pmats[i]);
            m.links[nbLinksWithoutHoles + 1 + i] = geoLink;
        }
    }


}

void drawVertexTriangle(PMat3D* p1, PMat3D* p2, PMat3D* p3) {
    glTexCoord2f(0.0, 0.0);
    glVertex3f(p1->position.x, p1->position.y, p1->position.z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(p2->position.x, p2->position.y, p2->position.z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(p3->position.x, p3->position.y, p3->position.z);
}

void drawGhost() {
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
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
        g3x_Material(jaune,ambi,diff,spec,shin,1.);
        drawGhost();
        g3x_Material(rouge,ambi,diff,spec,shin,1.);
        glPushMatrix();
            glTranslatef(-x,-y, -z);
            glRotatef(-theta,0.,1.,0.);
            glScalef(1.0/sx * 0.96, 1.0/sy * 0.96, 1.0/sz * 0.96);
            glutSolidCube(2.);
        glPopMatrix();
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
    g3x_MakeTranslationXYZ(TransMat, x, y, z);
    g3x_MakeHomothetieXYZ(ScaleMat, sx, sy, sz);
    g3x_MakeRotationY(RotMat, theta);
    g3x_MakeTranslationXYZ(InvTransMat, -x, -y, -z);
    g3x_MakeHomothetieXYZ(InvScaleMat, 1.0/sx, 1.0/sy, 1.0/sz);
    g3x_MakeRotationY(InvRotMat, -theta);
    // S*R*T -> world to can
    g3x_ProdHMat(ScaleMat, RotMat, transformWorldToCanTemp);
    g3x_ProdHMat(transformWorldToCanTemp, TransMat, transformWorldToCan);
    // T * R * S -> can to world
    g3x_ProdHMat(InvTransMat, InvRotMat, transformCanToWorldTemp);
    g3x_ProdHMat(transformCanToWorldTemp, InvScaleMat, transformCanToWorld);

    int width = 1024, height = 512;
    modelateGhost(ghostWidth, ghostHeight);
    m.pmats[50]->position.z += 2;
    glEnable(GL_TEXTURE_2D);
    /* creation de la fenetre - titre et tailles (pixels) */
    g3x_InitWindow(*argv,width,height);
    g3x_SetPerspective(40.,100.,1.);
    /* position, orientation de la caméra */
    //g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});
    g3x_SetCameraCartesian((G3Xpoint){0,0.001, 5}, (G3Xpoint){0.,0.,1.});
    g3x_SetScrollWidth(6);
    g3x_CreateScrollv_d("ghostposition",&(sphere1->position.z),-0.1,  0.1,1.0,"position sphere fantome ");

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

     GLuint tex_2d = SOIL_load_OGL_texture
         (
             "GhostMatter.jpg",
             SOIL_LOAD_AUTO,
             SOIL_CREATE_NEW_ID,
             SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
         );
     if( 0 == tex_2d )
     {
         printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
     }
     glBindTexture(GL_TEXTURE_2D, tex_2d);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


     g3x_SetExitFunction(quitg3x  );     /* la fonction de sortie */
     g3x_SetDrawFunction(drawg3x);     /* la fonction de Dessin */
       g3x_SetAnimFunction(animg3x);


    /* lancement de la boucle principale */
    return g3x_MainStart();
    /* RIEN APRES CA */
}
