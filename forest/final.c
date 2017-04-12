//Project: Forest
//Rebecca Robb

#include "CSCIx229.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float th = 0;
float ph = -90;
float zh = 0;

int fov = 55;
int dim = 50;
double asp = 1;

GLuint makeTree;
GLuint makeForest;
unsigned int forest[6];
unsigned int skytex[2];
//unsigned int bushtex[2];

//Light Values
int move = 1;
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  70;  // Ambient intensity (%)
int diffuse   = 50;  // Diffuse intensity (%)
int specular  =   20;  // Specular intensity (%)
int shininess =  0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int lh        =  0;  // Light azimuth
float ylight  =   45;  // Elevation of light

//Camera calculations
double lx = 0;
double ly = 0;
double lz = 5;

double angleY = 90.0;
double angleZ = 90.0;
double directionX = 0.0;
double directionY = 1.0;
double directionZ = 0.0;

int godmode = 0;

//Code to create skybox - based some off of the code in ex25
static void Sky(double D)
{
   
   //  Sides
   glPushMatrix();
    glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,skytex[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.25,0.66); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.00,0.66); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.00,0.41); glVertex3f(+D,-D,0);
   glTexCoord2f(0.25,0.41); glVertex3f(-D,-D,0);

   glTexCoord2f(1.00,0.41); glVertex3f(+D,-D,0);
   glTexCoord2f(1.00,0.66); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0.66); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.75,0.41); glVertex3f(+D,+D,0);

   glTexCoord2f(0.75,0.41); glVertex3f(+D,+D,0);
   glTexCoord2f(0.75,0.66); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.50,0.66); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,0.41); glVertex3f(-D,+D,0);

   glTexCoord2f(0.25,0.66); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.25,0.41); glVertex3f(-D,-D,0);
   glTexCoord2f(0.50,0.41); glVertex3f(-D,+D,0);
   glTexCoord2f(0.50,0.66); glVertex3f(-D,+D,+D);
   
    

   //  Top and bottom
    
   
   glTexCoord2f(0.251,0.999); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.251,0.659); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.499,0.659); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.499,0.999); glVertex3f(+D,+D,+D);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skytex[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0); glVertex3f(-D,-D,0);
    glTexCoord2f(7.0,0.0); glVertex3f(+D,-D,0);
    glTexCoord2f(7.0,7.0); glVertex3f(+D,+D,0);
    glTexCoord2f(0.0,7.0); glVertex3f(-D,+D,0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//Code to build trees using randomized recursive fractal calculations
//Code modified from here: http://www.mhzn.net/index.php/8-c-opengl/2-first-post
void tree(double height, double radius)
{
   float angle, zangle, langle;
    int j, a;
    int angle_step = 60;
    double loheight = -(radius*0.65);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, forest[0]);
    glColor3d(0.6, 0.5, 0);
    //Draw cylinders to make up tree limbs
    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <=360; j += angle_step)
    {
        double xv = radius*Cos(j);
        double yv = radius*Sin(j);
        double xvp = (radius*1.35)*Cos(j);
        double yvp = (radius*1.35)*Sin(j);
        glNormal3d(xv, yv, 0);
        glTexCoord2d(Cos(j), 1.0);glVertex3d(xv, yv, height);
        glNormal3d(xvp, yvp, 0);
        glTexCoord2d(Cos(j), 0.0);glVertex3d(xvp, yvp, loheight);
    }
    glEnd();
    glPopMatrix();
    glTranslatef(0.0, 0.0, height+loheight);

    height -= height*0.175;
    radius -= radius*0.325;

    int branches = rand()% 4 + 1;

    for(a = 0; a < branches; a++)
    {
        //Determine randomly what the angle of rotation of the branches will be
        //Increases realism of tree
        angle = rand()%50+10;
        zangle = rand()%90+10;
        if(angle > 43)
        {
            angle = -(rand()%50+10);
        }

        if(height > 1){
            //Recursively call the tree function to continue building the tree.
            glPushMatrix();
            glRotatef(zangle,0.0,0.0,1);
            glRotatef(angle,1,1,0.0);
            ErrCheck("tree");
            tree(height, radius);
            glPopMatrix();
        }   
        else if(height > 0.75){   
                //Generate leaves at the end of the branches             
                langle = rand()%60+10;
                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, forest[1]);
                glColor3d(0.8, 0.8, 0.8);
                glRotatef(langle, 1, 0, -1);
                glBegin(GL_TRIANGLE_FAN);
                glNormal3d(0, 0, 1);

                glTexCoord2d(0.00, 0.00);glVertex3d(0, 0, 0);
                glTexCoord2d(0.50, 0.00);glVertex3d(0.5, 0, 0);
                glTexCoord2d(1.00, 0.75);glVertex3d(0.85, 0.5, 0);
                glTexCoord2d(1.00, 1.00);glVertex3d(1, 1, 0);
                glTexCoord2d(0.75, 1.00);glVertex3d(0.5, 0.85, 0);
                glTexCoord2d(0.50, 1.00);glVertex3d(0, 0.5, 0);
                
                glEnd();
                glPopMatrix();
        }
    }
}

//Code to build vines - modified from the tree function to make a linear fractal structure
void vines(double height, double radius)
{
   float angle, zangle, langle;
    int j, a;
    int angle_step = 90;
    double loheight = -radius;

    glBindTexture(GL_TEXTURE_2D, forest[3]);
    glColor3d(0.6, 0.6, 0.6);
    //Draw cylinders to make up vine stems
    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <=360; j += angle_step)
    {
        double xv = radius*Cos(j);
        double yv = radius*Sin(j);
        double xvp = (radius*1.35)*Cos(j);
        double yvp = (radius*1.35)*Sin(j);
        glNormal3d(xv, yv, 0);
        glTexCoord2d(Cos(j), 1.0);glVertex3d(xv, yv, height);
        glNormal3d(xvp, yvp, 0);
        glTexCoord2d(Cos(j), 0.0);glVertex3d(xvp, yvp, loheight);
    }
    glEnd();
    
    glTranslatef(0.0, 0.0, height);

    height -= height*0.125;

    int branches = rand()% 4 + 1;

    for(a = 0; a < branches; a++)
    {
        //Determine randomly what the angle of rotation of the branches will be
        angle = rand()%50+10;
        zangle = rand()%90+10;
        if(angle > 43)
        {
            angle = -(rand()%50+10);
        }

        if(height >2){
            //Recursively call the vine function to continue building the vines.
            glRotatef(zangle,0.0,0.0,1);
            glRotatef(angle,1,1,0.0);
            ErrCheck("bushes");
            vines(height, radius);
        }   
        else {   
                //Generate leaves at the end of the branches             
                langle = rand()%60+10;
                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, forest[2]);
                glColor3d(0.8, 0.8, 0.8);
                glRotatef(langle, 1, 0, -1);
                glBegin(GL_TRIANGLE_FAN);
                glNormal3d(0, 0, 1);

                glTexCoord2d(0.00, 0.00);glVertex3d(0, 0, 0);
                glTexCoord2d(0.50, 0.00);glVertex3d(0.5, 0, 0);
                glTexCoord2d(1.00, 0.75);glVertex3d(0.85, 0.5, 0);
                glTexCoord2d(1.00, 1.00);glVertex3d(1, 1, 0);
                glTexCoord2d(0.75, 1.00);glVertex3d(0.5, 0.85, 0);
                glTexCoord2d(0.50, 1.00);glVertex3d(0, 0.5, 0);
                
                glEnd();
                glPopMatrix();
        }
    }
}

//Code to build bushes using randomized recursive fractal calculations
void bushes(double height, double radius)
{
   float angle, zangle, langle;
    int j, a;
    int angle_step = 90;
    double loheight = -(radius*0.65);

    glTranslatef(0.0, 0.0, height+loheight);
    int branches = rand()% 4 + 1;

    for(a = 0; a < branches; a++)
    {


        //Determine randomly what the angle of rotation of the branches will be
        //Increases realism
        angle = rand()%50+10;
        zangle = rand()%90+10;
        if(angle > 43)
        {
            angle = -(rand()%50+10);
        }

        glPushMatrix();
        glRotatef(zangle,0.0,0.0,1);
        glRotatef(angle,1,1,0.0);
        

        glBindTexture(GL_TEXTURE_2D, forest[0]);
        glColor3d(0.6, 0.5, 0);
        //Draw cylinders to make up bush limbs
        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <=360; j += angle_step)
        {
            double xv = radius*Cos(j);
            double yv = radius*Sin(j);
            double xvp = (radius*1.35)*Cos(j);
            double yvp = (radius*1.35)*Sin(j);
            glNormal3d(xv, yv, 0);
            glTexCoord2d(Cos(j), 1.0);glVertex3d(xv, yv, height);
            glNormal3d(xvp, yvp, 0);
            glTexCoord2d(Cos(j), 0.0);glVertex3d(xvp, yvp, loheight);
        }
        glEnd();
        glPopMatrix();
        
        height -= height*0.175;
        radius -= radius*0.325;

        if(height > 0.15){
            //Recursively call the bushes function to continue building the bush.
            glPushMatrix();
            
            glRotatef(zangle,0.0,0.0,1);
            glRotatef(angle,1,1,0.0);
            
            ErrCheck("tree");
            bushes(height, radius);
            glPopMatrix();
           
            //Generate leaves along the branches             
            langle = rand()%60+10;
            glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, forest[2]);
            glColor3d(0.8, 0.8, 0.8);
            glRotatef(langle, 1, 0, -1);
            glScaled(0.3,0.3,0.3);
            glBegin(GL_TRIANGLE_FAN);
            glNormal3d(0, 0, 1);

            glTexCoord2d(0.00, 0.00);glVertex3d(0, 0, 0);
            glTexCoord2d(0.50, 0.00);glVertex3d(0.5, 0, 0);
            glTexCoord2d(1.00, 0.75);glVertex3d(0.85, 0.5, 0);
            glTexCoord2d(1.00, 1.00);glVertex3d(1, 1, 0);
            glTexCoord2d(0.75, 1.00);glVertex3d(0.5, 0.85, 0);
            glTexCoord2d(0.50, 1.00);glVertex3d(0, 0.5, 0);
            
            glEnd();
            glPopMatrix();
        }
    }
}

void flowers(double height, double radius)
{
   float angle, zangle;
    int j, a;
    int angle_step = 90;
    double loheight = -(radius*0.65);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, forest[0]);
    glColor3d(0, 1.0, 0);
    //Draw cylinders to make up flower stem
    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <=360; j += angle_step)
    {
        double xv = radius*Cos(j);
        double yv = radius*Sin(j);
        double xvp = (radius*1.35)*Cos(j);
        double yvp = (radius*1.35)*Sin(j);
        glNormal3d(xv, yv, 0);
        glTexCoord2d(Cos(j), 1.0);glVertex3d(xv, yv, height);
        glNormal3d(xvp, yvp, 0);
        glTexCoord2d(Cos(j), 0.0);glVertex3d(xvp, yvp, loheight);
    }
    glEnd();
    glPopMatrix();

    //Add leaves to the stem
    if(height >= 0.5){             
        glPushMatrix();
        glColor3f(0.8, 0.8, 0.8);
        glBindTexture(GL_TEXTURE_2D, forest[4]);
        glRotatef(45, 1, 1, 0);
        glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0,1,0);
        glTexCoord2d(0.5,0.0);glVertex3d(0.0, 0.0, 0.0);
        glTexCoord2d(0.0, 0.5);glVertex3d(-0.2, 0.4, -0.2);
        glTexCoord2d(0.5,1.0);glVertex3d(0.0, 3.0, 0.0);
        glTexCoord2d(1.0,0.5);glVertex3d(0.2, 0.4, 0.2);
        glEnd();
        glPopMatrix();
    }

    glTranslatef(0.0, 0.0, height+loheight);

    height -= height*0.175;
    radius -= radius*0.175;

    for(a = 0; a < 2; a++)
    {
        //Determine randomly what the angle of rotation of the stem
        //Increases realism of stem
        angle = rand()%40+10;
        zangle = rand()%90+10;
        if(angle > 43)
        {
            angle = -(rand()%40+10);
        }

        if(height > 0.5){
            //Recursively call the tree function to continue building the tree.
            glPushMatrix();
            glRotatef(zangle,0.0,0.0,1);
            glRotatef(angle,1,1,0.0);
            ErrCheck("tree");
            flowers(height, radius);
            glPopMatrix();
        }   
        else{   
                
                //Add the flower to the top of the stem
                glBindTexture(GL_TEXTURE_2D, forest[5]);
                glColor3d(0.8, 0.8, 0.8);
                //Rotate around axis to fan out the petals.
                for(j = 0; j <= 360; j += (360/6.3))//Divide 360 by number of petals you want
                {
                    glRotatef(j, 0, 0, 1);
                    glBegin(GL_TRIANGLE_FAN);
                    glNormal3d(0, 0, 1);

                    glTexCoord2d(0.50, 0.00);glVertex3d(0, 0, 0);
                    glTexCoord2d(0.00, 0.50);glVertex3d(0.7, 0, 0);                   
                    glTexCoord2d(0.50, 1.00);glVertex3d(1, 1, 0);                   
                    glTexCoord2d(1.00, 0.50);glVertex3d(0, 0.7, 0);
                    
                    glEnd();
                    
                }
               
        }
    }
}



void display()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //First person camera
    gluLookAt(lx,ly,lz , lx+directionX,ly+directionY,lz+directionZ , 0,0,1);

    
    //Code based off of ex13, with modifications.
    float Ambient[] = {0.01*ambient, 0.01*ambient, 0.01*ambient, 1.0};
    float Diffuse[] = {0.01*diffuse, 0.01*diffuse, 0.01*diffuse, 1.0};
    float Specular[] = {0.01*specular, 0.01*specular, 0.01*specular, 1.0};

    float Position[] = {25, 35, ylight, 1.0};

    glColor3f(1,1,1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
    glLightfv(GL_LIGHT0, GL_POSITION, Position);
    
    //Call the display lists to create the environment.
    glCallList(makeForest);
    glCallList(makeTree);

    Sky(dim); 
    
    glDisable(GL_LIGHTING);

    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
    
}

void init(void)
{
    //Function to initialize display lists and create trees
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    makeTree = glGenLists(1);
    glNewList(makeTree, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();tree(5, 0.7);glPopMatrix();
    glPushMatrix();glTranslatef(-35, 35, 0);tree(4, 0.4);glPopMatrix();
    glPushMatrix();glTranslatef(15, -25, 0);tree(5, 1);glPopMatrix();
    glPushMatrix();glTranslatef(35, 40, 0);tree(4, 0.6);glPopMatrix();
    glPushMatrix();glTranslatef(-20, -20, 0);tree(8, 2);glPopMatrix();
    glPushMatrix();glTranslatef(10, 0, 0);tree(5, 0.3);glPopMatrix();
    glPushMatrix();glTranslatef(-5, 30, 0);tree(4, 0.5);glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEndList();
}  

//Function to initialize the display list for the other objects(bushes, vines, flowers)
void init2(void)
{
    makeForest = glGenLists(1);
    glNewList(makeForest, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();glTranslatef(35, 35, 0);vines(3, 0.1);glPopMatrix();
    glPushMatrix();glTranslatef(-30, -30, 0);vines(3, 0.1);glPopMatrix();
    glPushMatrix();glTranslatef(0, 20, 0);bushes(1, 0.3);glPopMatrix();
    glPushMatrix();glTranslatef(27, -17, 0);bushes(1, 0.3);glPopMatrix();
    glPushMatrix();glTranslatef(30, 10, 0);glRotatef(-30, 1,0,0);bushes(1, 0.3);glPopMatrix();
    glPushMatrix();glTranslatef(5, 5, 0);glRotatef(-45, 0,1,0);bushes(1, 0.3);glPopMatrix();
    glPushMatrix();glTranslatef(-10, -20, 0);glRotatef(45, 0,1,0);bushes(1, 0.3);glPopMatrix();

    glPushMatrix();glTranslatef(0, -15, 0); flowers(1,0.1);glPopMatrix();
    glPushMatrix();glTranslatef(15, -15, 0); flowers(1,0.1);glPopMatrix();
    glPushMatrix();glTranslatef(45, -45, 0); flowers(1,0.08);glPopMatrix();
    glPushMatrix();glTranslatef(-23, 23, 0); flowers(1,0.08);glPopMatrix();
    glPushMatrix();glTranslatef(-23, 0, 0); flowers(1,0.08);glPopMatrix();
    glPushMatrix();glTranslatef(15, 40, 0); flowers(1,0.08);glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEndList();
}

void special(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
    {
        //Angle first person camera up.
        angleY += 0.05;
        directionZ = -cos(angleY);
        directionY = sin(angleY);
        directionX = sin(angleY);
    }
    else if(key == GLUT_KEY_DOWN)
    {
        //Angle first camera down.
        angleY -= 0.05;
        directionZ = -cos(angleY);
        directionY = sin(angleY);
        directionX = sin(angleY);
    }

    else if(key == GLUT_KEY_RIGHT)
        zh += 5;
    else if(key == GLUT_KEY_LEFT)
        zh -= 5;

    else if(key == GLUT_KEY_PAGE_UP)
        dim += 5;
    else if(key == GLUT_KEY_PAGE_DOWN)
        dim -= 5;
    Project(fov, asp, dim);
    glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
    float fraction = 2;
    if(key == 'w')
    {
        //Move camera forward - if in god mode, allow movement in the z plane.
        lx += directionX * fraction;
        ly += directionY * fraction;
        if(godmode) lz += directionZ * fraction;
    }
    else if(key == 's')
    {
        //Move camera backward - if in god mode, allow movement in the z plane.
        lx -= directionX * fraction;
        ly -= directionY * fraction;
        if(godmode) lz -= directionZ * fraction;
    }
    else if(key == 'd')
    {
        angleZ -= 0.05;
        directionX = sin(angleZ);
        directionY = -cos(angleZ);
    }
    else if(key == 'a')
    {
        angleZ += 0.05;
        directionX = sin(angleZ);
        directionY = -cos(angleZ);
    }

    else if(key == 27)
        exit(0);

    else if(key == 'g')
    {
        lz = 20;
        godmode = 1;
    }
    else if(key == 'f')
    {
        lz = 5;
        godmode = 0;
    }
    
    Project(fov, asp, dim);
    glutPostRedisplay();
}

void reshape(int width, int height)
{
    asp = (height>0) ? (double)width/height : 1;
    glViewport(0,0, width, height);
    Project(fov, asp, dim);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Forest by Rebecca Robb");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    skytex[0] = LoadTexBMP("skybox1.bmp");
    skytex[1] = LoadTexBMP("forestfloor.bmp");
    forest[0] = LoadTexBMP("treebark.bmp");
    forest[1] = LoadTexBMP("leavestexture.bmp");
    forest[2] = LoadTexBMP("redleaftex.bmp");
    forest[3] = LoadTexBMP("birch.bmp");
    forest[4] = LoadTexBMP("flowerleaf.bmp");
    forest[5] = LoadTexBMP("flowertex.bmp");
    init2();
    init();
    
    ErrCheck("init");
    glutMainLoop();
    return 0;
}
