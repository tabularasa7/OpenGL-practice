//Assignment 3: Lighting and Textures
//Rebecca Robb
//code for first person camera modified from here: http://stackoverflow.com/questions/13862246/opengl-fps-camera-using-glulookat

//#include <stdio.h>
//#include <stdlib.h>
//#include <stdarg.h>
//#include <string.h>
//#include <math.h>
#include "CSCIx229.h"

//#define PI 3.14159265359
//#define Cos(th) cos(PI/180*(th))
//#define Sin(th) sin(PI/180*(th))

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int th = 0;
int ph = -85;
int zh = 0;

//Camera calculations
double lx = -6;
double ly = -6;
double lz = 2;

double angleY = 0.0;
double angleZ = 90.0;
double directionX = 0.0;
double directionY = -1.0;
double directionZ = 0.0;

//Perspectives
double dim = 12.0;
int mode = 1;
int fov = 55;
double asp = 1;

//Light Values
int move = 1;
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  50;  // Ambient intensity (%)
int diffuse   = 50;  // Diffuse intensity (%)
int specular  =   10;  // Specular intensity (%)
int shininess =  0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int lh        =  0;  // Light azimuth
float ylight  =   5;  // Elevation of light

//Textures
unsigned int texture[3];

void lego(int red, int green, int blue, float dx, float dy, float dz,
    double sx, double sy, double sz,
    float th, float zh)
{  
    

    glRotatef(th, 0, 1, 0); 
    glRotatef(zh, 0, 0, 1);
   
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    
    glTranslatef(dx, dy, dz);
    glScaled(sx, sy, sz);
    glBegin(GL_QUADS);
    glColor3f(red, green, blue);

     //Left
    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0,0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0,0.25); glVertex3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.5,0.25); glVertex3f(2.0, 0.0, 1.0);
    glTexCoord2f(0.5,0.0); glVertex3f(2.0, 0.0, 0.0);

    //Right
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 4.0, 0.0);
    glTexCoord2f(0.5, 0.0); glVertex3f(2.0, 4.0, 0.0);
    glTexCoord2f(0.5, 0.25); glVertex3f(2.0, 4.0, 1.0);
    glTexCoord2f(0.0, 0.25); glVertex3f(0.0, 4.0, 1.0);

    //Front
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 4.0, 0.0);
    glTexCoord2f(1.0, 0.25); glVertex3f(0.0, 4.0, 1.0);
    glTexCoord2f(0.0, 0.25); glVertex3f(0.0, 0.0, 1.0);
    
    //Back
    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(2.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.25); glVertex3f(2.0, 0.0, 1.0);
    glTexCoord2f(1.0, 0.25); glVertex3f(2.0, 4.0, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.0, 4.0, 0.0);

    //Top
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0, 0.5); glVertex3f(2.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 1.0); 
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 4.0, 1.0);
    glTexCoord2f(1.0, 0.5); glVertex3f(2.0, 4.0, 1.0);

    //Bottom
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.5); glVertex3f(2.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.5); glVertex3f(2.0, 4.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 4.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    //Cylinders to make lego pegs
    //code for cylinders modified from this tutorial: https://www.youtube.com/watch?v=Kujd0RTsaAQ
    
    float height = 1.25;
    float loheight = 0.5;
    float angle_step = 5;
    int j = 0;
    int i = 0;
    int k = 0;

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTranslatef((0.5+dx/sx)*sx, (0.5+dy/sy)*sy, (0.0+dz/sz)*sz);
    glScaled(0.35*sx, 0.35*sy, 1*sz);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+=angle_step)
    {
        glNormal3f(Cos(j), Sin(j), 0);
        glTexCoord2f(Sin(j), 0.5); glVertex3f(Cos(j), Sin(j), height);
        glTexCoord2f(Sin(j), 0.0); glVertex3f(Cos(j), Sin(j), loheight);
    }
    glEnd();
    
    for(i=1.25; i>=.25; i-=1)
    {
        glBegin(GL_TRIANGLE_FAN);
        
        glNormal3f(0,0,i);
        glTexCoord2f(0.5, 0.5); glVertex3f(0,0,1.25);
        for(k = 0; k <=360; k+=angle_step)
        {
            glTexCoord2f(2*Cos(k)+0.5, 0.25*Sin(k)+0.5); glVertex3f(i*Cos(k), Sin(k), height);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTranslatef((0.5+dx/sx)*sx, (1.5+dy/sy)*sy, (0.0+dz/sz)*sz);
    glScaled(0.35*sx, 0.35*sy, 1*sz);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+=angle_step)
    {
        glNormal3f(Cos(j), Sin(j), 0);
        glTexCoord2f(Sin(j), 0.5); glVertex3f(Cos(j), Sin(j), height);
        glTexCoord2f(Sin(j), 0.0); glVertex3f(Cos(j), Sin(j), loheight);
    }
    glEnd();
    
    for(i=1.25; i>=.25; i-=1)
    {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,0,i);
        glTexCoord2f(0.5, 0.5); glVertex3f(0,0,1.25);
        for(k = 0; k <=360; k+=angle_step)
        {
            glTexCoord2f(2*Cos(k)+0.5, 0.25*Sin(k)+0.5); glVertex3f(i*Cos(k), Sin(k), height);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTranslatef((0.5+dx/sx)*sx, (2.5+dy/sy)*sy, (0.0+dz/sz)*sz);
    glScaled(0.35*sx, 0.35*sy, 1*sz);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+=angle_step)
    {
        glNormal3f(Cos(j), Sin(j), 0);
        glTexCoord2f(Sin(j), 0.5); glVertex3f(Cos(j), Sin(j), height);
        glTexCoord2f(Sin(j), 0.0); glVertex3f(Cos(j), Sin(j), loheight);
    }
    glEnd();
    
    for(i=1.25; i>=.25; i-=1)
    {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,0,i);
        glTexCoord2f(0.5, 0.5); glVertex3f(0,0,1.25);
        for(k = 0; k <=360; k+=angle_step)
        {
            glTexCoord2f(2*Cos(k)+0.5, 0.25*Sin(k)+0.5); glVertex3f(i*Cos(k), Sin(k), height);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTranslatef((0.5+dx/sx)*sx, (3.5+dy/sy)*sy, (0.0+dz/sz)*sz);
    glScaled(0.35*sx, 0.35*sy, 1*sz);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+=angle_step)
    {
        glNormal3f(Cos(j), Sin(j), 0);
        glTexCoord2f(Sin(j), 0.5); glVertex3f(Cos(j), Sin(j), height);
        glTexCoord2f(Sin(j), 0.0); glVertex3f(Cos(j), Sin(j), loheight);
    }
    glEnd();
    
    for(i=1.25; i>=.25; i-=1)
    {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,0,i);
        glTexCoord2f(0.5, 0.5); glVertex3f(0,0,1.25);
        for(k = 0; k <=360; k+=angle_step)
        {
            glTexCoord2f(2*Cos(k)+0.5, 0.25*Sin(k)+0.5); glVertex3f(i*Cos(k), Sin(k), height);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTranslatef((1.5+dx/sx)*sx, (0.5+dy/sy)*sy, (0.0+dz/sz)*sz);
    glScaled(0.35*sx, 0.35*sy, 1*sz);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+=angle_step)
    {
        glNormal3f(Cos(j), Sin(j), 0);
        glTexCoord2f(Sin(j), 0.5); glVertex3f(Cos(j), Sin(j), height);
        glTexCoord2f(Sin(j), 0.0); glVertex3f(Cos(j), Sin(j), loheight);
    }
    glEnd();
    
    for(i=1.25; i>=.25; i-=1)
    {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,0,i);
        glTexCoord2f(0.5, 0.5); glVertex3f(0,0,1.25);
        for(k = 0; k <=360; k+=angle_step)
        {
            glTexCoord2f(2*Cos(k)+0.5, 0.25*Sin(k)+0.5); glVertex3f(i*Cos(k), Sin(k), height);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTranslatef((1.5+dx/sx)*sx, (1.5+dy/sy)*sy, (0.0+dz/sz)*sz);
    glScaled(0.35*sx, 0.35*sy, 1*sz);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+=angle_step)
    {
        glNormal3f(Cos(j), Sin(j), 0);
        glTexCoord2f(Sin(j), 0.5); glVertex3f(Cos(j), Sin(j), height);
        glTexCoord2f(Sin(j), 0.0); glVertex3f(Cos(j), Sin(j), loheight);
    }
    glEnd();
    
    for(i=1.25; i>=.25; i-=1)
    {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,0,i);
        glTexCoord2f(0.5, 0.5); glVertex3f(0,0,1.25);
        for(k = 0; k <=360; k+=angle_step)
        {
            glTexCoord2f(2*Cos(k)+0.5, 0.25*Sin(k)+0.5); glVertex3f(i*Cos(k), Sin(k), height);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTranslatef((1.5+dx/sx)*sx, (2.5+dy/sy)*sy, (0.0+dz/sz)*sz);
    glScaled(0.35*sx, 0.35*sy, 1*sz);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+=angle_step)
    {
        glNormal3f(Cos(j), Sin(j), 0);
        glTexCoord2f(Sin(j), 0.5); glVertex3f(Cos(j), Sin(j), height);
        glTexCoord2f(Sin(j), 0.0); glVertex3f(Cos(j), Sin(j), loheight);
    }
    glEnd();
    
    for(i=1.25; i>=.25; i-=1)
    {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,0,i);
        glTexCoord2f(0.5, 0.5); glVertex3f(0,0,1.25);
        for(k = 0; k <=360; k+=angle_step)
        {
            glTexCoord2f(2*Cos(k)+0.5, 0.25*Sin(k)+0.5); glVertex3f(i*Cos(k), Sin(k), height);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTranslatef((1.5+dx/sx)*sx, (3.5+dy/sy)*sy, (0.0+dz/sz)*sz);
    glScaled(0.35*sx, 0.35*sy, 1*sz);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+=angle_step)
    {
        glNormal3f(Cos(j), Sin(j), 0);
        glTexCoord2f(Sin(j), 0.5); glVertex3f(Cos(j), Sin(j), height);
        glTexCoord2f(Sin(j), 0.0); glVertex3f(Cos(j), Sin(j), loheight);
    }
    glEnd();
    
    for(i=1.25; i>=.25; i-=1)
    {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,0,i);
        glTexCoord2f(0.5, 0.5); glVertex3f(0,0,1.25);
        for(k = 0; k <=360; k+=angle_step)
        {
            glTexCoord2f(2*Cos(k)+0.5, 0.25*Sin(k)+0.5); glVertex3f(i*Cos(k), Sin(k), height);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

   
}

//some code for cone found here and modified: http://www.davidwparker.com/2011/09/05/opengl-screencast-8-drawing-in-3d-part-3-spheres/
void cone()
{
    float height = 5;
    float loheight = 0;
    float angle_step = 5;
    int j = 0;
    int i = 0;
    int k = 0;
    
    glTranslatef(-7, 0,1);
    glScalef(2, 1, 2);
    glColor3f(1,1,0);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_TRIANGLES);
    for(j = 0; j <= 360; j+=angle_step)
    {
        glNormal3f(Cos(j), 0, Sin(j));
        glTexCoord2f(0.5, 0.5); glVertex3f(0, height, 0);
        glTexCoord2f(2*Cos(j)+0.5, 2*Sin(j)+0.5); glVertex3f(Cos(j), loheight, Sin(j));
        glTexCoord2f(2*Cos(j+angle_step)+0.5, 2*Sin(j+angle_step)+0.5); glVertex3f(Cos(j+angle_step), loheight, Sin(j+angle_step));
    }
    glEnd();

    glPushMatrix();
    //glRotatef(90, 1, 0, 0); 
    for(i=0; i>= 0; i-=1)
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,0,i);
        glTexCoord2f(0.5,0.5); glVertex3f(0,0,i);
        for(k = 0; k <=360; k+=angle_step)
        {
            glTexCoord2f(2*Cos(k)+0.5, 0.5*Sin(k)+0.5); glVertex3f(i*Cos(k), loheight, Sin(k));
        }
        glEnd();
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}


//Ball and vertex code from ex14
/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

void ground()
{   
    int i = 0;
    int j = 0;
    int num = 100;

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glPushMatrix();
    glTranslatef(-10, -10, 0);
    glScalef(2, 2, 1);
    glBegin(GL_QUADS);
    glColor3f(0, 0.5, 0);
    glNormal3f(0,0,1); 
    for(i = 0; i < num; i++)
        for(j = 0; j < num; j++)
        {
            glTexCoord2d((1.0/num)*(i+0), (1.0/num)*(j+0)); glVertex2d(5*(2.0/num)*(i+0),5*(2.0/num)*(j+0));
            glTexCoord2d((1.0/num)*(i+1), (1.0/num)*(j+0)); glVertex2d(5*(2.0/num)*(i+1),5*(2.0/num)*(j+0));
            glTexCoord2d((1.0/num)*(i+1), (1.0/num)*(j+1)); glVertex2d(5*(2.0/num)*(i+1),5*(2.0/num)*(j+1));
            glTexCoord2d((1.0/num)*(i+0), (1.0/num)*(j+1)); glVertex2d(5*(2.0/num)*(i+0),5*(2.0/num)*(j+1));
        }
    
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void errcheck(char * where)
{
    int err = glGetError();
    if(err) fprintf(stderr, "ERROR: %s [%s]\n", gluErrorString(err), where);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //Set viewing angle.
    //Perspective code modified from ex9.
    glEnable(GL_DEPTH_TEST);
    if (mode == 1)
   {  
        
      double Ex = -2*dim*Sin(ph)*Cos(zh);
      double Ey = +2*dim*Cos(ph)*Cos(zh);
      double Ez = +2*dim*Sin(zh);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,0,Cos(zh));
   }
    else if(mode == 2)
    {
      gluLookAt(lx,ly,lz , lx+directionX,ly+directionY,lz+directionZ , 0,0,1);
    }   
    
    glShadeModel(GL_SMOOTH);

    //Code based off of ex13, with modifications.
    float Ambient[] = {0.01*ambient, 0.01*ambient, 0.01*ambient, 1.0};
    float Diffuse[] = {0.01*diffuse, 0.01*diffuse, 0.01*diffuse, 1.0};
    float Specular[] = {0.01*specular, 0.01*specular, 0.01*specular, 1.0};

    float Position[] = {distance*Cos(lh), distance*Sin(lh), ylight, 1.0};
    glColor3f(1,1,1);
    ball(Position[0], Position[1], Position[2], 0.1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, local);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
    glLightfv(GL_LIGHT0, GL_POSITION, Position);
    
    
    ground();
    //Build pyramid out of legos
    //Bottom layer
    lego(1, 0, 0, 4, 0, 0, 1, 1, 1, 0, 0);
    lego(0, 0, 1, 4, -4, 0, 1, 1, 1, 0, 0);
    lego(1, 1, 0, -4, -4, 0, 1, 1, 1, 0, 90);
    lego(1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 90);
    lego(0, 0, 1, 0, -4, 0, 1, 1, 1, 0, 0);
    lego(1, 1, 0, -4, 0, 0, 1, 1, 1, 0, 90);
    //Middle layer
    lego(1, 0, 0, -1, -1, 1, 1, 1, 1, 0, 90);
    lego(0, 0, 1, -3, -3, 1, 1, 1, 1, 0, 90);
    lego(1, 1, 0, -5, -1, 1, 1, 1, 1, 0, 90);
    lego(0, 0, 1, -3, 1, 1, 1, 1, 1, 0, 90);
    //Top layer
    lego(1, 0, 0, 1, -2, 2, 1, 1, 1, 0, 90);
    
    //Big Lego
    lego(0, 1, 1, 4, 0, -8, 3, 3, 3, 90, 90);
    
    //Cone for funsies
    cone();
    //ball(-1,0,0,0.5);
    glDisable(GL_LIGHTING);
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}

//Code for idle modified from ex13
/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   lh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


void key(unsigned char ch,int x,int y)
{
    if(ch == 27)
        exit(0);
    else if(ch == 'u' || ch == 'U'){
        mode = 1;
    }
    else if(ch == 'f' || ch == 'F')
    {
        mode = 2;
    }    
    else if(ch == 'p' || ch == 'P')
        th += 5;
    else if(ch == 'o' || ch == 'O')
        th -= 5;
    else if(ch == 'w' || ch == 'W'){
        zh += 5; 
    }     
    else if(ch == 's' || ch == 'S'){
        zh -= 5;
    }      
    else if(ch == 'a' || ch == 'A'){
        ph -= 5;
    }
    else if(ch == 'd' || ch == 'D'){
        ph += 5; 
    }  
    else if(ch == '>')
        lh -= 3;
    else if(ch == '<')
        lh += 3;
    //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
    else if(ch == '-')
        ylight-=0.5;
    else if(ch == '+')
        ylight+=0.5;
    else if(ch == 'y')
        distance = (distance==2) ? 5 : 2;
    else if (ch=='z' && ambient>0)
      ambient -= 5;
   else if (ch=='Z' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='x' && diffuse>0)
      diffuse -= 5;
   else if (ch=='X' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='c' && specular>0)
      specular -= 5;
   else if (ch=='C' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='v' && emission>0)
      emission -= 5;
   else if (ch=='V' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='b' && shininess>-1)
      shininess -= 1;
   else if (ch=='B' && shininess<7)
      shininess += 1;

//  Translate shininess power to value (-1 => 0)
    shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
    th %= 360;
    lh %= 360;
    //Code for perspective changes modified from ex9.
    Project(fov, asp, dim);
    glutIdleFunc(move?idle:NULL);
    glutPostRedisplay();
}


void reshape(int width, int height)
{
    asp = (height>0) ? (double)width/height : 1;
    glViewport(0,0,width, height);
    Project(fov, asp, dim);
}


void special(int key, int x, int y)
{
    float fraction = 0.3f;
    if (key == GLUT_KEY_UP)
    {
        lx += directionX * fraction;
        ly += directionY * fraction;
        lz += directionZ * fraction;
    }    
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
    {
        lx -= directionX * fraction;
        ly -= directionY * fraction;
        lz -= directionZ * fraction;
    } 
    else if(key == GLUT_KEY_RIGHT)
    {
        angleZ -= 0.05;
        directionX = sin(angleZ);
        directionY = -cos(angleZ);
    }
    else if(key == GLUT_KEY_LEFT)
    {
        angleZ += 0.05;
        directionX = sin(angleZ);
        directionY = -cos(angleZ);
    }

    else if(key == GLUT_KEY_PAGE_UP)
        dim -= 1;
    else if(key == GLUT_KEY_PAGE_DOWN)
        dim += 1;
    
    ph %= 360;
    zh %= 360;
    Project(fov, asp, dim);
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Lego Pyramid by Rebecca Robb");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    texture[0] = LoadTexBMP("brick-textures.bmp");
    texture [1] = LoadTexBMP("pine-tree-textures.bmp");
    texture[2] = LoadTexBMP("grass.bmp");
    ErrCheck("init");
    glutMainLoop();
    return 0;
}
