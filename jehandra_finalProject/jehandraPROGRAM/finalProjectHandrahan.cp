/******************************************
*
* Official Name:  Jillian Handrahan
*
* E-mail:  jehandra@syr.edu
*
* Assignment:  Final Assignment
*
* Environment/Compiler:  xcode 14.2
*
* Date submitted:  April 30, 2023
 
*thanks to 3Dexport.com for foliage.bmp!
* References:  particle2.cpp, beziercurveswithoutput.cpp, loadTextures.cpp, class code getID colorpicking.cpp.
*
* Interactions:
 Press 'd' to view the dragon fly
 Click the dragon fly in either window to view the dragon fly
 Press 'e' to view the bee
 Click the bee in either window to view the bee
 Press 'f' to view the butterfly
 Click the butterfly in either window  to view the butterfly
 Press 'w' to reset the selection
 Click the selected bug again to reset the selection
 Press ' ' (space bar) to halt flying animations to make selection easier
 Press 1 to view the scene from the north
 Press 2 to view the scene from the east
 Press 3 to view the scene from the south
 Press 4 to view the scene from the west
 Press a to view the scene from above
 Press r to view the scene as the movable player
 Press u to move the player up
 Press i to move the player down
 Press the left key to turn the player view left
 Press the right key to turn the player view right
 Press the up key to move the player forwards
 Press the down key to move the player backwards
 Right click and select 'Quit' to quit
 Right click and select 'Reset Bug Selection' to reset the bug selector
 Right click and hover over 'Bug Options' to choose which bug you would like to learn about
 Right click and hover over 'Color Options' to change the color of the light over the bug
 Right click and hover over 'Sun Color Options' to change the color of the light outside
 Press 'm' to decrease the global ambient light
 Press 'M' to increase the global ambient light
 Press 'v' to decrease the material shininess
 Press 'V' to increase the material shininess
*
*
*******************************************/

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#  include <GLUT/glut.h>
//#  include <OpenGL/glext.h>
#else
#  include <GL/glut.h>
//#  include <GL/glext.h>
#endif

#define PI 3.14159265

using namespace std;


// Globals.
static int isWindy = 1;
static int isSelectFly = 0;
static int isSelectBee = 0;
static int isSelectButter = 0;
static int beeRot = 90;
static int isInfo = 1;

static int infoWindow, bugWindow;
static int animationPeriod = 1; // Time interval between frames.

static float bugx  = 5;
static float bugy  = 35;
static float bugz  = 36;

static float flyx  = 5;
static float flyy  = 35;
static float flyz  = 36;

static int beer = 0;
static float beex  = 50;
static float statBeex =  45;
static float beey  = 56;
static float beez  = 25;

static float butterx  = -28;
static float buttery  = 50;
static float butterz  = 25;

static float displayRot = 0;

static float heightBug;
static float widthBug;

static float yVal = 0;
bool selecting = false;
int xClick,yClick;
int height, width;
float angle = 0;
static int hmm = 0;

static float distBee = 5;

static float radPlayer = 5.0;

static int stages = 0;
static int  north = 0;
static int east = 0;
static int  south   = 0;
static int west =  0;
static int above = 0;
static float cloudx = -160;
static int southMove = 90;

int itemID = 0;
int BEE = 1;
int FLY = 2;
static float wingAng = 0;
static int inc = 1;
static float zRot =  0;
static int flap = 1;
static int isAnimate = 1; // Animated?
static float m = 0.2; // Global ambient white light intensity.
static float ddifspec = .8; // Diffuse and specular white light intensity.
static float p = 0.10;
static float shine = 20.0;
static float sunA = .6;
static bool isSunny = true;

static float spotAngle = 15.0; // Spotlight cone half-angle.
static float spotExponent = 1.0; // Spotlight exponent = attenuation.

static bool isHive = false;

static float r = ddifspec;
static float g = ddifspec;
static float b = ddifspec;

static float rSun = .1;
static float gSun = .1;
static float bSun = .1;

static float xVal = 0, zVal = 97; // Co-ordinates of the user.

static unsigned int texture[3]; // Array of texture indices.
static float d = 0.0;

static float circle_vertices[62][3];
static float radius = 6;

static GLUquadricObj *qobj;

int num_particles = 10;
float speed = .5;
float coef = 1; /* perfectly elastic collisions */
int present_time;
int last_time;

// Control points.
//bezierCurvesWithOutput.cpp MODIFIED to use my points
static float controlPoints[6][3] =
{
    { -16.0, -32.0, 0.0}, { -30.0, 0.0, 0.0}, { -10.0, 7.0, 0.0},
    {0.0, 7.0, 0.0}, {27.0, 0.0, 0.0}, { 7.0, -32.0, 0.0}
};

static float controlPointsSur[6][4][3] =
{
    {{-2.0, 0.0, 5.0}, {-0.25, 0.0, 5.0}, {0.25, 0.0, 5.0}, {3.0, 0.0, 5.0}},
    {{-3.0, 0.0, 3.0}, {-0.25, 2.0, 3.0}, {0.25, 2.0, 3.0}, {3.0, 2.0, 3.0}},
    {{-3.0, 0.0, 1.0}, {-0.25, 6.0, 1.0}, {0.25, 6.0, 1.0}, {3.0, 7.0, 1.0}},
    {{-3.0, 0.0, -1.0}, {-0.25, 6.0, -1.0}, {0.25, 4.0, -1.0}, {3.0, 6.0, -1.0}},
    {{-3.0, 0.0, -3.0}, {-0.25, 2.0, -3.0}, {0.25, 3.0, -3.0}, {3.0, 2.0, -3.0}},
    {{-3.0, 0.0, -5.0}, {-0.25, 0.0, -5.0}, {0.25, 0.0, -5.0}, {3.0, 0.0, -4.0}},
};
//particle2.cpp MODIFIED to remove color
typedef struct particle
{
     float position[3];
     float velocity[3];
     float mass;
    
} particle;

particle particles[15]; /* particle system */

//for loop from particle2.cpp - mass changed
void particleSetUp()
{
    for(int i=0; i<num_particles; i++)
    {
          particles[i].mass = 5.0;
        
        for(int j=0; j<3; j++)
        {
            particles[i].position[j] = 2.0*((float) rand()/RAND_MAX)-1.0;
            particles[i].velocity[j] = speed*2.0*((float) rand()/RAND_MAX)-1.0;
        }
    }
}
//from particle2.cpp, changed the bounding box and math to suit my bees
void collision(int n)
{
     for (int i=0; i<3; i++)
     {
           if(particles[n].position[i]>=3.0)
           {
                particles[n].velocity[i] = -coef*particles[n].velocity[i];
                particles[n].position[i] = 3-coef*(particles[n].position[i]-3);
           }
           if(particles[n].position[i]<=-3.0)
           {
                particles[n].velocity[i] = -coef*particles[n].velocity[i];
                particles[n].position[i] = -3-coef*(particles[n].position[i]+3);
           }
     }
}
//from myIdle() particle2.cpp - changed to remove repulsion/forces
void beeSystem()
{
    float dt;
    present_time = glutGet(GLUT_ELAPSED_TIME);
    dt = 0.001*(present_time -  last_time);
    for(int i=0; i<num_particles; i++)
    {
       for(int j=0; j<3; j++)
       {
           particles[i].position[j]+=dt*particles[i].velocity[j];
           particles[i].velocity[j]+=dt/particles[i].mass;
       }
        collision(i);
    }
    last_time = present_time;
    glutPostRedisplay();
}
//from loadTextures.cpp UNMODIFIED
// Struct of bitmap file.
struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

//from loadTextures.cpp UNMODIFIED
// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(string filename)
{
   BitMapFile *bmp = new BitMapFile;
   unsigned int size, offset, headerSize;
  
   // Read input file name.
   ifstream infile(filename.c_str(), ios::binary);
 
   // Get the starting point of the image data.
   infile.seekg(10);
   infile.read((char *) &offset, 4);
   
   // Get the header size of the bitmap.
   infile.read((char *) &headerSize,4);

   // Get width and height values in the bitmap header.
   infile.seekg(18);
   infile.read( (char *) &bmp->sizeX, 4);
   infile.read( (char *) &bmp->sizeY, 4);

   // Allocate buffer for the image.
   size = bmp->sizeX * bmp->sizeY * 24;
   bmp->data = new unsigned char[size];

   // Read bitmap data.
   infile.seekg(offset);
   infile.read((char *) bmp->data , size);
   
   // Reverse color from bgr to rgb.
   int temp;
   for (int i = 0; i < size; i += 3)
   {
      temp = bmp->data[i];
      bmp->data[i] = bmp->data[i+2];
      bmp->data[i+2] = temp;
   }

   return bmp;
}

//used loadTextures.cpp to learn how to do this
void loadExternalTextures()
{
   BitMapFile *image[3];

   image[0] = getBMPData("jehandraTEXTURES/grass.bmp");
   image[1] = getBMPData("jehandraTEXTURES/sky.bmp");
   image[2] = getBMPData("jehandraTEXTURES/foliage.bmp");
   
   // Bind grass image to texture index[0].
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);
    
    // Bind leaf image to texture index[2]
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2]->sizeX, image[2]->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image[2]->data);
}

void drawCloud(float xc, float yc, float zc, int radius) {
    glPushMatrix();
    glTranslatef(xc, yc, zc);
    glColor3f(1, 1, 1);
    glutSolidSphere(radius, 20, 20);
    glTranslatef(-radius*1.5, 0, 0);
    glutSolidSphere(radius/1.5, 20, 20);
    glTranslatef(radius*3, 0, 5);
    glutSolidSphere(radius/1.2, 20, 20);
    glTranslatef(-radius, 0, -radius);
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

void drawTrees(float xt, float yt, float zt, float size) {
    yt-=30;
    glPushMatrix();
    glScalef(size, size, size);
    glPushMatrix();
    glColor3f(.8, .4, .3);  //trunk
    glTranslatef(xt, yt, zt);
    glScaled(1, 25, 1);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(1, 2, 20, 20);
    glPopMatrix();
    glPushMatrix();
    
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glColor3f(.4, .6, .3); //leaves
    glTranslatef(xt, yt+35, zt);
    gluSphere(qobj, 17, 50, 50);
    glColor3f(.4, .9, .4);
    glTranslatef(12, -6, -.5);
    glutSolidSphere(10, 50, 50);
    glColor3f(.3, .8, .4);
    glTranslatef(-24, 0, -1);
    glutSolidSphere(13, 50, 50);
    glColor3f(.4, .8, .3);
    glTranslatef(10, -3, 5.7);
    glutSolidSphere(12, 50, 50);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopMatrix();
}

void drawFly1(float ang) {
    // Draw the sphere head
    // dragonfly
    glPushMatrix();
    glScaled(.8, .8, .8);
    glTranslatef(0, -5, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(-ang, 0, 0, 1);
    glRotatef(10, 1, 0, 0);

    glPushMatrix();
    glColor3f(.3, .3, 1);
    glScalef(.5, 5, .5); // Scale the sphere
    glutWireSphere(1.0, 20, 20); // Draw the sphere
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(.4, .4, .8);
    glTranslatef(0, 5.2, 0.0); // Move the sphere head up
    glutSolidSphere(0.5, 20, 20); // Draw the sphere head
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 5.2, 0);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, -1);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(-1, 0, -1);
    glEnd();
    glColor3f(1, 0, 1);
    glTranslatef(1, 0, -1);
    glutSolidCube(.5);
    glTranslatef(-2, 0, 0);
    glutSolidCube(.5);
    glPopMatrix();
    
    //right wings
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glTranslatef(3.8, 2, 0);
    glBegin(GL_TRIANGLE_FAN);
    int  num_segments = 20;
    glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 0.5f * sin(angle); // Height is 1, so radius is 0.5
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glRotatef(-30, 0, 0, 1);
    glTranslatef(3.1, 1.7, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 0.5f * sin(angle); // Height is 1, so radius is 0.5
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    //left wings
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glTranslatef(-3.8, 2, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 0.5f * sin(angle); // Height is 1, so radius is 0.5
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glRotatef(30, 0, 0, 1);
    glTranslatef(-3.1, 1.7, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 0.5f * sin(angle); // Height is 1, so radius is 0.5
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

void drawFly2(float ang) {
    // Draw the sphere head
    // bee
    glPushMatrix();
    glScaled(.8, .8, .8);
    glTranslatef(0, -5, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(-ang, 0, 0, 1);
    glRotatef(10, 1, 0, 0);

    //body
    glPushMatrix();
    glColor3f(.8, .8, .1);
    glScalef(1, 2, 1); // Scale the sphere
    glutSolidSphere(1.0, 20, 20); // Draw the sphere
    glPopMatrix();
    
    //stripes
    glPushMatrix();
   // glTranslatef(0, 1, 0);
    glColor3f(0, 0, 0);
    glScaled(.6, 1, .6);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(.5, 1.2, 20, 20);
    glTranslatef(0, 0, -1);
    glutSolidTorus(.5, .9, 20, 20);
    glTranslatef(0, 0, 2);
    glutSolidTorus(.5, .9, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(.3, .3, .3);
    glTranslatef(0, 2.2, 0.0); // Move the sphere head up
    glutSolidSphere(0.5, 20, 20); // Draw the sphere head
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 2.2, 0);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, -1);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(-1, 0, -1);
    glEnd();
    glColor3f(1, 1, 0);
    glTranslatef(1, 0, -1);
    glScaled(.2, .2, .2);
    glutSolidIcosahedron();
    glTranslatef(-(2/.2), 0, 0);
    glutSolidIcosahedron();
    glPopMatrix();
    
    //right wings
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glTranslatef(1.9, .75, 0);
    glBegin(GL_TRIANGLE_FAN);
    int  num_segments = 20;
    glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 1.0f * cos(angle); // Width is 2, so radius is 1
            float y = 1.0f * sin(angle); // Height is 2, so radius is 1
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glRotatef(-30, 0, 0, 1);
    glTranslatef(2, .5, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 1.0f * cos(angle); // Width is 2, so radius is 1
            float y = 1.0f * sin(angle); // Height is 2, so radius is 1
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    //left wings
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glTranslatef(-1.9, .75, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 1.0f * cos(angle); // Width is 2, so radius is 1
            float y = 1.0f * sin(angle); // Height is 2, so radius is 1
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glRotatef(30, 0, 0, 1);
    glTranslatef(-2, .5, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 1.0f * cos(angle); // Width is 2, so radius is 1
            float y = 1.0f * sin(angle); // Height is 2, so radius is 1
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    
    //stinger
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(0, -1.6, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(.5, 2, 20, 20);
    glPopMatrix();
    
    glPopMatrix();
}

void drawFly3(float ang) {
    // Draw the sphere head
    // butterfly
    glPushMatrix();
    glScaled(.8, .8, .8);
    glTranslatef(0, -5, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(-ang, 0, 0, 1);
    glRotatef(10, 1, 0, 0);

    glPushMatrix();
    glColor3f(.8, .3, .3);
    glScalef(.5, 5, .5); // Scale the sphere
    glutWireSphere(1.0, 20, 20); // Draw the sphere
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(.9, .4, .4);
    glTranslatef(0, 5.2, 0.0); // Move the sphere head up
    glutSolidSphere(0.5, 20, 20); // Draw the sphere head
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 5.2, 0);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, -1);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(-1, 0, -1);
    glEnd();
    glColor3f(0, 1, 1);
    glTranslatef(1, 0, -1);
    glScaled(.2, .2, .2);
    glutSolidTeapot(1);
    glTranslatef(-(2/.2), 0, 0);
    glutSolidTeapot(1);
    glPopMatrix();

    //right wings
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glTranslatef(3.8, 2, 0);
    glBegin(GL_TRIANGLE_FAN);
    int  num_segments = 20;
    glColor4f(.9, .9, 1, .5);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 2.0f * sin(angle); // Height is 4, so radius is 2
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glRotatef(-30, 0, 0, 1);
    glTranslatef(3.1, 1.7, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(.9, .9, 1, .5);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 2.0f * sin(angle); // Height is 4, so radius is 2
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    //left wings
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glTranslatef(-3.8, 2, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(.9, .9, 1, .5);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 2.0f * sin(angle); // Height is 4, so radius is 2
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glRotatef(30, 0, 0, 1);
    glTranslatef(-3.1, 1.7, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(.9, .9, 1, .5);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 2.0f * sin(angle); // Height is 4, so radius is 2
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

void drawFlower(void) {
    //learned to do this from bezierCurvesWithOutput.cpp
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 6, controlPoints[0]);
    glEnable(GL_MAP1_VERTEX_3);
    glColor3f(1.0, 0.0, 1.0);
    glMapGrid1f(100, 0.0, 1.0);
    glLineWidth(5);
    glEvalMesh1(GL_LINE, 0, 100);
    
    glPushMatrix();
    glTranslatef(40, -45, 0);
    glRotatef(-90, 0, 0, 1);
    glEvalMesh1(GL_LINE, 0, 100);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-50, -35, 0);
    glRotatef(90, 0, 0, 1);
    glEvalMesh1(GL_LINE, 0, 100);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-10, -85, 0);
    glRotatef(180, 0, 0, 1);
    glEvalMesh1(GL_LINE, 0, 100);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(.3, .2, .1);
    glTranslatef(-5, -40, 0);
    glutSolidSphere(20, 30, 30);
    glPopMatrix();
}

void drawSelects(void) {
    glLineWidth(1);
    // Draw the sphere head
    // dragonfly
    glPushMatrix();
    glScaled(.8, .8, .8);
    glRotatef(90, 1, 0, 0);
    glTranslatef(flyx, flyy, flyz); //put in starting pos
    glRotatef(-zRot, 0, 0, 1);

    glPushMatrix();
    if (selecting) {
        glColor3f(1, 0, 0);
    }
    else glColor3f(.3, .3, 1);
    glScalef(.5, 5, .5); // Scale the sphere
    glutWireSphere(1.0, 20, 20); // Draw the sphere
    glPopMatrix();
    
    glPushMatrix();
    if (selecting) {
        glColor3f(1, 0, 0);
    }
    else glColor3f(.4, .4, .8);
    glTranslatef(0, 5.2, 0.0); // Move the sphere head up
    glutSolidSphere(0.5, 20, 20); // Draw the sphere head
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 5.2, 0);
    if (selecting) {
        glColor3f(1, 0, 0);
    }
    else glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, -1);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(-1, 0, -1);
    glEnd();
    if (selecting) {
        glColor3f(1, 0, 0);
    }
    else glColor3f(1, 0, 1);
    glTranslatef(1, 0, -1);
    glutSolidCube(.5);
    glTranslatef(-2, 0, 0);
    glutSolidCube(.5);
    glPopMatrix();
    
    //right wings
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glTranslatef(3.8, 2, 0);
    glBegin(GL_TRIANGLE_FAN);
    int  num_segments = 20;
    if (selecting) {
        glColor3f(1, 0, 0);
    }
    else glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 0.5f * sin(angle); // Height is 1, so radius is 0.5
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glRotatef(-30, 0, 0, 1);
    glTranslatef(3.1, 1.7, 0);
    glBegin(GL_TRIANGLE_FAN);
    if (selecting) {
        glColor3f(1, 0, 0);
    }
    else glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 0.5f * sin(angle); // Height is 1, so radius is 0.5
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    //left wings
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glTranslatef(-3.8, 2, 0);
    glBegin(GL_TRIANGLE_FAN);
    if (selecting) {
        glColor3f(1, 0, 0);
    }
    else glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 0.5f * sin(angle); // Height is 1, so radius is 0.5
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glRotatef(30, 0, 0, 1);
    glTranslatef(-3.1, 1.7, 0);
    glBegin(GL_TRIANGLE_FAN);
    if (selecting) {
        glColor3f(1, 0, 0);
    }
    else glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 0.5f * sin(angle); // Height is 1, so radius is 0.5
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    glPopMatrix();
    
    // bee
    glPushMatrix();
    glScaled(.8, .8, .8);
    glRotatef(90, 1, 0, 0);
    glTranslatef(beex, beey, beez); //put in starting pos
    glRotatef(-beeRot, 0, 0, 1);

    //body
    glPushMatrix();
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(.8, .8, .1);
    glScalef(1, 2, 1); // Scale the sphere
    glutSolidSphere(1.0, 20, 20); // Draw the sphere
    glPopMatrix();
    
    //stripes
    glPushMatrix();
   // glTranslatef(0, 1, 0);
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(0, 0, 0);
    glScaled(.6, 1, .6);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(.5, 1.2, 20, 20);
    glTranslatef(0, 0, -1);
    glutSolidTorus(.5, .9, 20, 20);
    glTranslatef(0, 0, 2);
    glutSolidTorus(.5, .9, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(.3, .3, .3);
    glTranslatef(0, 2.2, 0.0); // Move the sphere head up
    glutSolidSphere(0.5, 20, 20); // Draw the sphere head
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 2.2, 0);
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, -1);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(-1, 0, -1);
    glEnd();
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(1, 1, 0);
    glTranslatef(1, 0, -1);
    glScaled(.2, .2, .2);
    glutSolidIcosahedron();
    glTranslatef(-(2/.2), 0, 0);
    glutSolidIcosahedron();
    glPopMatrix();
    
    //right wings
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glTranslatef(1.9, .75, 0);
    glBegin(GL_TRIANGLE_FAN);
    num_segments = 20;
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 1.0f * cos(angle); // Width is 2, so radius is 1
            float y = 1.0f * sin(angle); // Height is 2, so radius is 1
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glRotatef(-30, 0, 0, 1);
    glTranslatef(2, .5, 0);
    glBegin(GL_TRIANGLE_FAN);
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 1.0f * cos(angle); // Width is 2, so radius is 1
            float y = 1.0f * sin(angle); // Height is 2, so radius is 1
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    //left wings
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glTranslatef(-1.9, .75, 0);
    glBegin(GL_TRIANGLE_FAN);
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 1.0f * cos(angle); // Width is 2, so radius is 1
            float y = 1.0f * sin(angle); // Height is 2, so radius is 1
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glRotatef(30, 0, 0, 1);
    glTranslatef(-2, .5, 0);
    glBegin(GL_TRIANGLE_FAN);
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(.9, .9, 1);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 1.0f * cos(angle); // Width is 2, so radius is 1
            float y = 1.0f * sin(angle); // Height is 2, so radius is 1
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    
    //stinger
    glPushMatrix();
    if (selecting) {
        glColor3f(0, 1, 0);
    }
    else glColor3f(0, 0, 0);
    glTranslatef(0, -1.6, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(.5, 2, 20, 20);
    glPopMatrix();
    glPopMatrix();
    
    // Draw the sphere head
    // butterfly
    glPushMatrix();
    glScaled(.8, .8, .8);
    glRotatef(90, 1, 0, 0);
    glTranslatef(butterx, buttery, butterz); //put in starting pos
    glRotatef(zRot, 0, 0, 1);
    glRotatef(-30, 1, 0, 0);

    glPushMatrix();
    if (selecting) {
        glColor3f(0, 0, 1);
    }
    else glColor3f(.8, .3, .3);
    glScalef(.5, 5, .5); // Scale the sphere
    glutWireSphere(1.0, 20, 20); // Draw the sphere
    glPopMatrix();
    
    glPushMatrix();
    if (selecting) {
        glColor3f(0, 0, 1);
    }
    else glColor3f(.9, .4, .4);
    glTranslatef(0, 5.2, 0.0); // Move the sphere head up
    glutSolidSphere(0.5, 20, 20); // Draw the sphere head
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 5.2, 0);
    if (selecting) {
        glColor3f(0, 0, 1);
    }
    else glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, -1);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(-1, 0, -1);
    glEnd();
    if (selecting) {
        glColor3f(0, 0, 1);
    }
    else glColor3f(0, 1, 1);
    glTranslatef(1, 0, -1);
    glScaled(.2, .2, .2);
    glutSolidTeapot(1);
    glTranslatef(-(2/.2), 0, 0);
    glutSolidTeapot(1);
    glPopMatrix();

    
    //right wings
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glTranslatef(3.8, 2, 0);
    glDepthMask(GL_FALSE);
    glBegin(GL_TRIANGLE_FAN);
    num_segments = 20;
    if (selecting) {
        glColor3f(0, 0, 1);
    }
    else glColor4f(.9, .9, 1, .5);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 2.0f * sin(angle); // Height is 4, so radius is 2
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-wingAng, 0, 1, 0);
    glRotatef(-30, 0, 0, 1);
    glTranslatef(3.1, 1.7, 0);
    glBegin(GL_TRIANGLE_FAN);
    if (selecting) {
        glColor3f(0, 0, 1);
    }
    else glColor4f(.9, .9, 1, .5);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 2.0f * sin(angle); // Height is 4, so radius is 2
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    //left wings
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glTranslatef(-3.8, 2, 0);
    glBegin(GL_TRIANGLE_FAN);
    if (selecting) {
        glColor3f(0, 0, 1);
    }
    else glColor4f(.9, .9, 1, .5);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 2.0f * sin(angle); // Height is 4, so radius is 2
            glVertex3f(x, y, 0);
        }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glRotatef(wingAng, 0, 1, 0);
    glRotatef(30, 0, 0, 1);
    glTranslatef(-3.1, 1.7, 0);
    glBegin(GL_TRIANGLE_FAN);
    if (selecting) {
        glColor3f(0, 0, 1);
    }
    else glColor4f(.9, .9, 1, .5);
        glVertex2f(0.0, 0.0); // Center of the oval
        for (int i = 0; i <= num_segments; ++i) {
            float angle = 2.0f * PI * i / num_segments;
            float x = 3.5f * cos(angle); // Width is 7, so radius is 3.5
            float y = 2.0f * sin(angle); // Height is 4, so radius is 2
            glVertex3f(x, y, 0);
        }
    glEnd();
    glDepthMask(GL_TRUE);
    glPopMatrix();
    glPopMatrix();
}

void drawCubes(float size) {
    float numCubesAcross = 50.0/size;
    glPushMatrix();
    for (int i = 0; i < numCubesAcross;i++) {
        for (int j = 0; j < numCubesAcross;j++){
            glutSolidCube(size);
            glTranslatef(size, 0, 0);
        }
        glTranslatef(-(size*numCubesAcross), -size, 0);
    }
    glPopMatrix();
}

int distFrom() {
    distBee = sqrt(pow(xVal-40,2)+pow(yVal,2)+pow(zVal-42,2));
    if (distBee < radPlayer+20) {
        return 1;
    }
    else {
        return 0;
    }
}
//from color picking example in class MODIFIED heavily to suit my needs and situations
void getID(int x, int y)
{
    unsigned char pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    
    if ((int)pixel[0]==0&&(int)pixel[1]==255&&(int)pixel[2]==0)
    {
        itemID=BEE;  //green, bee
        if (isSelectBee){
            isInfo = 1;
            isSelectBee= 0;
        }
        else {
            isInfo = 0;
            isSelectBee=1;
        }
        if (isSelectFly) isSelectFly = 0;
        if (isSelectButter) isSelectButter = 0;
    }
    else if ((int)pixel[0]==0&&(int)pixel[1]==0&&(int)pixel[2]==255)
    {
        if (isSelectButter){
            isInfo = 1;
            isSelectButter= 0;
        }
        else {
            isInfo = 0;
            isSelectButter=1;
        }
        if (isSelectFly) isSelectFly = 0;
        if (isSelectBee) isSelectBee = 0;
        
    }
    else if ((int)pixel[0]==255&&(int)pixel[1]==0&&(int)pixel[2]==0)
    {
        itemID=FLY;  //red, fly
        if (isSelectFly){
            isInfo = 1;
            isSelectFly= 0;
        }
        else {
            isInfo = 0;
            isSelectFly=1;
        }
        if (isSelectBee) isSelectBee = 0;
        if (isSelectButter) isSelectButter = 0;
    }
    else itemID=0;
    
    selecting=false;
}

//from rendering text example from class UNMODIFIED
void writeBitmapString(void *font, const char *string)
{  const char *c;
   for (c = string; *c != '\0'; c++)
   glutBitmapCharacter(font, *c);
}

void platform(void){
    glPushMatrix();
    glColor3f(.4, .4, .4);
    glScaled(1, .1, 1);
    glTranslatef(0, -65, 0);
    glRotatef(10, 1, 0, 0);
    glutSolidSphere(6, 10, 10);
    glPopMatrix();
}

void setUpInfo(void){
    if (isSelectFly) {
        glRasterPos3f(-5,10,0);
        const char *bug1 = "This is a Blue Dragonfly";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, bug1);
        glRasterPos3f(-12.2,8,0);
        const char *fun1 = "they can move and rotate each of their wings independently";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, fun1);
        glRasterPos3f(-8, 6, 0);
        const char *help1 = "Press 'w' to go back to the selection screen";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, help1);
        drawFly1(displayRot);
        platform();
        glColor3f(0,0,0);
    }
    if (isSelectBee) {
        glRasterPos3f(-5,10,0);
        const char *bug2 = "This is a Yellow Bee";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, bug2);
        glRasterPos3f(-8,8,0);
        const char *fun2 = "A honey bee can fly up to 15 miles per hour";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, fun2);
        glRasterPos3f(-8, 6, 0);
        const char *help2 = "Press 'w' to go back to the selection screen";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, help2);
        drawFly2(displayRot);
        platform();
        glColor3f(0,0,0);
    }
    if (isSelectButter) {
        glRasterPos3f(-5,10,0);
        const char *bug3 = "This is a Red Butterfly";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, bug3);
        glRasterPos3f(-8,8,0);
        const char *fun3 = "Some Butterflies Have transparent wings";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, fun3);
        glRasterPos3f(-8, 6, 0);
        const char *help3 = "Press 'w' to go back to the selection screen";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, help3);
        drawFly3(displayRot);
        platform();
        glColor3f(0,0,0);
    }
}

void drawSceneBug(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
    if (isInfo) {
        glRasterPos3f(-10,10,0);
        const char *instruction = "Select a Bug from the garden to hear a fun fact";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, instruction);
        glRasterPos3f(-11,8,0);
        const char *orelse = "If you cannot find the desired bug, select from below";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, orelse);
        glRasterPos3f(-6,6,0);
        const char *keyop = "or tap the key below the bug";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, keyop);

        //bug1 button
        glPushMatrix();
        glTranslatef(-8, 4, 0);
        glScaled(.5, .5, .5);
        platform();
        drawFly1(0);
        glPopMatrix();
        glColor3f(0,0,0);
        glRasterPos3f(-9,-1,0);
        const char *flykey = "'d'";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, flykey);
        
        //bug2 button
        glPushMatrix();
        glTranslatef(0, 4, 0);
        glScaled(.5, .5, .5);
        platform();
        drawFly2(-40);
        glPopMatrix();
        glColor3f(0,0,0);
        glRasterPos3f(-1,-1,0);
        const char *flykey2 = "'e'";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, flykey2);
        
        //bug3 button
        glPushMatrix();
        glTranslatef(8, 4, 0);
        glScaled(.5, .5, .5);
        platform();
        drawFly3(-40);
        glPopMatrix();
        glColor3f(0,0,0);
        glRasterPos3f(7,-1,0);
        const char *flykey3 = "'f'";
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, flykey3);
    }
    setUpInfo();
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();        glutPostRedisplay();
    glutSwapBuffers();
}
// Drawing routine.
void drawScene(void)
{
    glutSetWindow(bugWindow);
    // Place scene in frustum.
      //south
      if (south) gluLookAt(0, 0, southMove, 0, 0, 0, 0, 1, 0); // Set the camera position and orientation
      
      //north
     else  if (north) gluLookAt(0, 0, -90, 0, 0, 0, 0, 1, 0);

      //east
     else  if  (east)  gluLookAt(90, 0, 0, 0, 0, 0, 0, 1, 0);

      //west
      else if (west) gluLookAt(-90, 0, 0, 0, 0, 0, 0, 1, 0);
      
      //above
      else if (above) {
          gluLookAt(0, 110, 0, 0, 0, 0, 0, 0, 1);
          glRotatef(180, 0, 1, 0);
      }
      else {
          gluLookAt(xVal, yVal-15, zVal, xVal-sin(angle*(PI/180.0)), yVal-15, zVal-cos(angle*(PI/180.0)), 0, 1, 0);
      }
    float lightPos0[] = { 0.0, 120.0, 0.0, 0.0 }; //location of sun

    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    
    float sunAmb[] = {sunA,sunA,sunA,1};
    float lightDifAndSpec1[] = { r, g, b, 1.0 };
    float lightDifAndSpec0[] = { rSun, gSun, bSun, 1.0 };
    float globAmb[] = { m, m, m, 1.0 };
    // Light0 properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);
    glPushMatrix();
     glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glPopMatrix();
    
    //Bug light props
    float fLightPos[] = { bugx, -bugy+20, bugz, 1.0 }; // Spotlight position.
    float spotDirection[] = {0.0, -1.0, 0.0}; // Spotlight direction.

    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDifAndSpec1);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lightDifAndSpec1);
    glLightfv(GL_LIGHT2, GL_POSITION, fLightPos);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spotAngle);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, spotExponent);
    
    glPushMatrix();

      glPushMatrix();
      float matAmbAndDif[] = {.7, .4, .4, 1.0};
      float matAmbAndDifBack[] = {.7, .5, .7, 1.0};
    
      float matSpec[] = { .7, .4, .4, 1.0 };

      float matShine[] = { shine };
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
      glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDifBack);
       glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matShine);
    
    matAmbAndDif[0] = .7;
    matAmbAndDif[1] = .4;
    matAmbAndDif[2] = .3;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);

    glEnable(GL_NORMALIZE);

    float matambDif2[] = {.6, .3, .3};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matambDif2);
    glShadeModel(GL_SMOOTH);
    
    //log
    //clipping
    static double clip_plane[] = { 1.0, 0.0, 0.0, 15};
    
    glPushMatrix();
    glClipPlane(GL_CLIP_PLANE0, clip_plane);
    glEnable(GL_CLIP_PLANE0);

    glRotatef(-55, 0, 1, 0);
    glScaled(1, 1, 20);
    glTranslatef(-5, -30, -.1);
    glutSolidTorus(1, 2, 20, 20);
    glScaled(1, 1, .05);
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

    //KEEP COLOR ***********************************************************
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_LIGHT0);
    
    //flower for butterfly
    glPushMatrix();
    glTranslatef(-20, -20, 50);
    glLineWidth(5);
    glColor3f(.1, .5, .1);
    glBegin(GL_LINES);
        glVertex3f(1, -2, 0);
        glVertex3f(1, -10, 0);
    
    glEnd();
    glRotatef(45, 0, 0, 1);
    glScaled(.05, .05, .05);
    drawFlower();
    glPopMatrix();
    
    //bee particles
    for(int i=0; i<num_particles; i++)
    {
        glPushMatrix();
        glTranslatef((statBeex-7)+particles[i].position[0], (beey-77)+particles[i].position[1], (beez+20)+particles[i].position[2]);
        glScaled(.3, .3, .3);
        if (i % 2) {
            drawFly2(beer);
        }
        else {
            drawFly2(-beer);
        }
        glPopMatrix();
    }
    glPopMatrix();

    //fly lamp
    if (isSelectFly) {
        glDisable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef(flyx*.8, -(flyz*.8)+20, flyy*.8);
        glColor3f(r, g, b);
        glutSolidSphere(3, 10, 10);
        glRotatef(-90, 1, 0, 0);
        glEnable(GL_LIGHTING);
        glColor3f(.4, .4, .4);
        glutSolidCone(6, 4, 40, 40);
        glPopMatrix();

        bugx=flyx*.8;
        bugy=flyz*.8;
        bugz=flyy*.8;
        glEnable(GL_LIGHT2);
    }
    //bee lamp
    else if (isSelectBee) {
        glDisable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef(beex*.8, -(beez*.8)+20, beey*.8);
        glColor3f(r, g, b);
        glutSolidSphere(3, 10, 10);
        glRotatef(-90, 1, 0, 0);
        glEnable(GL_LIGHTING);
        glColor3f(.4, .4, .4);
        glutSolidCone(6, 4, 40, 40);
        glColor3f(.4, .6, .4);
        glTranslatef(-12.5, 12.5, -32.4);
        glScaled(.5, .5, 1);
        drawCubes(5);
        glPopMatrix();

        bugx=beex*.8;
        bugy=beez*.8;
        bugz=beey*.8;
        glEnable(GL_LIGHT2);
    }
    //butter lamp
    else if (isSelectButter) {
        glDisable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef(butterx*.8, -(butterz*.8)+20, buttery*.8);
        glColor3f(r, g, b);
        glutSolidSphere(3, 10, 10);
        glRotatef(-90, 1, 0, 0);
        glEnable(GL_LIGHTING);
        glColor3f(.4, .4, .4);
        glutSolidCone(6, 4, 40, 40);
        glColor3f(.4, .6, .4);
        glTranslatef(-12.5, 12.5, -32.4);
        glScaled(.5, .5, 1);
        drawCubes(5);
        glPopMatrix();

        bugx=butterx*.8;
        bugy=butterz*.8;
        bugz=buttery*.8;
        glEnable(GL_LIGHT2);
    }
    else {
        glDisable(GL_LIGHT2);
    }

    glEnable(GL_TEXTURE_2D);
    // Map the grass texture onto a rectangle along the xz-plane.
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
       glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -29.95, 100.0);
       glTexCoord2f(8.0, 0.0); glVertex3f(100.0, -29.95, 100.0);
       glTexCoord2f(8.0, 8.0); glVertex3f(100.0, -29.95, -100.0);
       glTexCoord2f(0.0, 8.0); glVertex3f(-100.0, -29.95, -100.0);
    glEnd();

    // Map the sky texture onto a rectangle parallel to the xy-plane.
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_POLYGON);
       glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -30.0, -100.0);
       glTexCoord2f(1.0, 0.0); glVertex3f(100.0, -30.0, -100.0);
       glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, -100.0);
       glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, -100.0);
    glEnd();
    
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -30.0, 100.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(-100.0, -30.0, -100.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(-100.0, 120.0, -100.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, 100.0);
     glEnd();
    
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0); glVertex3f(100.0, -30.0, 100.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(100.0, -30.0, -100.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, -100.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(100.0, 120.0, 100.0);
     glEnd();
    glBegin(GL_POLYGON);
       glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -30.0, 100.0);
       glTexCoord2f(1.0, 0.0); glVertex3f(100.0, -30.0, 100.0);
       glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, 100.0);
       glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, 100.0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    //sun section
    //drawhand
    glPushMatrix();
    glTranslatef(xVal-sin(angle*(PI/180.0)), yVal, zVal-cos(angle*(PI/180.0)));
    glRotatef(angle, 0, 1, 0);
    glTranslatef(5, -20, -7);
    glColor3f(.9, .6, .6);
    glScalef(1, 1, 2);
    glutSolidCube(2);
    glPopMatrix();
        
    //rock path
    glPushMatrix();
    glTranslatef(3, -29.9, 48);
    glScalef(1, .05, 1);
    glColor3f(.5, .5, .5);
    glutSolidSphere(5, 10, 10);
    glTranslatef(-6, 0, 1);
    glutSolidSphere(1, 10, 10);
    glTranslatef(-4, 0, -2);
    glutSolidSphere(2, 10, 10);
    glTranslatef(4, 0, 8);
    glutSolidSphere(4, 10, 10);
    glTranslatef(7, 0, 1);
    glutSolidSphere(3, 10, 10);
    glTranslatef(4, 0, 2);
    glutSolidSphere(1, 10, 10);
    glTranslatef(-6, 0, 7);
    glutSolidSphere(6, 10, 10);
    glTranslatef(-8, 0, -3);
    glutSolidSphere(2, 10, 10);
    glPopMatrix();
    
    //bez
    glPushMatrix();
    glTranslatef(32, -32, -32);
    // Specify and enable the Bezier surface.
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, controlPointsSur[0][0]);
    glEnable(GL_MAP2_VERTEX_3);

    glColor3f(.4, .4, .4);
    glLineWidth(4);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEvalMesh2(GL_LINE, 0, 20, 0, 20);
    
    glRotatef(180, 0, 1, 0);
    glTranslatef(-5, 0, 0);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEvalMesh2(GL_LINE, 0, 20, 0, 20);
    
    
    glPopMatrix();

    float matShine2[] = {10.0};
    float outsideSpec[] = {r, g, b, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, outsideSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, outsideSpec);
    
    //tree
    drawTrees(50, 0, -60, 1);
    drawTrees(70, -6, -10, .6);
    drawTrees(-30, -1, -70, .8);
    glPushMatrix();
    glRotatef(30, 0, 1, 0);
    drawTrees(-40, 0, -80, 1);
    glPopMatrix();
    drawTrees(-60, -1, 20, 1);
    drawTrees(55, 0, 40, 1);
    glDisable(GL_NORMALIZE);
    
    //hive
    glPushMatrix();
    glTranslatef(40, -20, 42);
    glColor3f(.9, .8, .3);
    glRotatef(-60, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(1, 4.1, 20, 20);
    glTranslatef(0, 4.5, 0);
    glColor3f(0, 0, 0);
    glutSolidSphere(1, 20, 20);
    glColor3f(.9, .8, .3);
    glTranslatef(0, -4.5, -1.4);
    glutSolidTorus(1, 3.8, 20, 20);
    glTranslatef(0, 0, -1.4);
    glutSolidTorus(1, 3, 20, 20);
    glTranslatef(0,0,-1.4);
    glutSolidTorus(1, 2.6, 20, 20);
    glTranslatef(0,0,-.5);
    glutSolidSphere(2.5, 20, 20);
    glTranslatef(0, 0, 4.7);
    glutSolidTorus(1, 3.8, 20, 20);
    glTranslatef(0, 0, 1.4);
    glutSolidTorus(1, 3, 20, 20);
    glTranslatef(0,0,.5);
    glutSolidSphere(2.5, 20, 20);
    glPopMatrix();
    
    //clouds
    glPushMatrix();
    drawCloud(cloudx, 50, 10, 8);
    drawCloud(cloudx + 10, 55, -30, 3);
    drawCloud(cloudx + 25, 50, -20, 6);
    drawCloud(cloudx + 35, 50, 10, 9);
    drawCloud(cloudx + 50, 55, -30, 3);
    drawCloud(cloudx + 65, 50, -20, 6);
    drawCloud(cloudx + 75, 55, 15, 3);
    drawCloud(cloudx + 90, 50, -20, 6);
    glPopMatrix();

    //grass
    glShadeModel(GL_FLAT);
    glEnable(GL_NORMALIZE);
    glPushMatrix();
    glColor3f(.4, .6, .4);
    glScaled(2, 1, 2);
    glTranslatef(0, -80, 0);
    glutSolidCube(100);
    glPopMatrix();
    glShadeModel(GL_SMOOTH);
    //grass cubes
    glPushMatrix();
    glColor3f(.4, .6, .4);
    glTranslatef(-5, -32.4, 0);
    glRotatef(90, 1, 0, 0);
    drawCubes(5);
    glTranslatef(-50, 0, 0);
    drawCubes(5);
    glPopMatrix();
    
    //pond
    glPushMatrix();
    glColor3f(.4, .6, .9);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, 29.9);
    glScaled(1, 1, .01);
    glutSolidSphere(40, 60, 60);
    glPopMatrix();
    
    circle_vertices[0][0] = 0;
    circle_vertices[0][1] = 0;
    circle_vertices[0][2] = 0;
    
    for (int i = 1; i < 62; i++)
    {
        circle_vertices[i][0] = 0 + radius * cos(i * 2.0 * M_PI / 60.0);
        circle_vertices[i][1] = 0;
        circle_vertices[i][2] = 0 + radius * sin(i * 2.0 * M_PI / 60.0);
    }

    //lily
    glPushMatrix();
    glColor3f(.5, .8, .5);
    glTranslatef(4, -29, 30);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 58; i++)
    {
        glVertex3fv(circle_vertices[i]);
    }
    glEnd();
    glPopMatrix();

    glDisable(GL_LIGHT0);
    //shadows on the ground
        glPushMatrix();
        glTranslatef(0, -29.9, 0);
        glScaled(1, .01, 1);
        drawTrees(50, 0, -60, 1);
        drawTrees(70, -6, -10, .6);
        drawTrees(-30, -1, -70, .8);
        glPushMatrix();
        glRotatef(30, 0, 1, 0);
        drawTrees(-40, 0, -80, 1);
        glPopMatrix();
        drawTrees(-60, -1, 20, 1);
        drawTrees(55, 0, 40, 1);
        glPopMatrix();
    glDisable(GL_LIGHTING);
    
    //sun
    glPushMatrix();
    if (isSunny) {
        glColor3f(1, 1, 0);
    }
    else {
        glColor3f(.6, .6, .6);
    }
    glTranslatef(0, 120, 0);
    glutSolidSphere(10, 10, 10);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    if (selecting) {
        glDisable(GL_LIGHTING);
        drawSelects();
        getID(xClick, yClick);
        glEnable(GL_LIGHTING);
    }
    else {
        drawSelects();
        glutSwapBuffers();
    }
    glPopMatrix();
    glDisable(GL_NORMALIZE);
   glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1);
}


void animate(int value)
{
    if (isSelectFly || isSelectBee || isSelectButter) {
        displayRot+=4;
    }
    beer+=1;
    if (isAnimate){
        if (beex < 50 && !isHive) {
            beex+=.5;
        }
        if (beex > 49) {
            isHive = true;
            beeRot = -90;
        }
        if (isHive){
            beex-=.5;
        }
        if (beex == 30) {
            isHive = false;
            beeRot = 90;
        }
    }
    if (isWindy) {
        if (cloudx <= 80) cloudx += 1;
        if (cloudx == 80) cloudx = -190;
    }
    glutSetWindow(bugWindow);
    if (isAnimate)
   {
       if (wingAng < 25.0 && inc) {
           wingAng += 5.0;
       }
       if (wingAng >= 25.0){
           inc = 0;
       }
       if (inc == 0 && wingAng > -25) {
           wingAng -= 5;
       }
       if (wingAng == -25) {
           inc = 1;
       }
       if (flyz >= 4.0 && stages <= 32) {
           flyz-=.1;
           stages++;
       }
       if (stages < 162 && flap == 1) {
           zRot+=1;
           stages++;
       }
       if (stages >=  162 &&  stages < 305 && flap == 1)  {
           flyx += .28;
           flyy -=.45;
           flyz -= .075;
           stages++;
       }
       if (stages > 304 && stages < 425  &&  flap == 1) {
           zRot++;
           stages++;
       }
       if (stages > 424 && stages < 557 && flap == 1) {
           flyx -= .51;
           flyz -= .36;
           stages++;
       }
       if (stages > 556 && stages < 720 && flap == 1) {
           zRot++;
           stages++;
       }
       if (stages > 719 && stages < 821 && flap == 1) {
           flyx += .4;
           flyy += .058;
           flyz+=.551;
           stages++;
       }
       if (stages > 820 && stages < 905 && flap == 1){
           zRot--;
           stages++;
       }
       if (stages > 904 && flyy < 38 && flap == 1) {
           flyx -= .1024;
           flyy += .451;
           flyz += .014;
           stages++;
       }
       if (zRot < 360 && stages > 1028 && flap == 1) {
           zRot++;
           flyz+=.13;
           stages++;
       }
       if (stages >= 1050  && flap == 1) {
           stages = 0;
           zRot = 0;
           wingAng = 0;
       }
   }
   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 27:
         exit(0);
         break;
       case ' ':
          if (isAnimate) isAnimate = 0;
          else isAnimate = 1;
            if (stages == 0) {
                stages++;
                flap = 1;
            }
           glutSetWindow(bugWindow);
          glutPostRedisplay();
          break;
       case 'd':
            if (isSelectBee) {
               isSelectBee = 0;
                isSelectFly = 1;
           }
           if (isSelectButter) {
               isSelectButter = 0;
               isSelectFly = 1;
           }
           else {
               isSelectFly = 1;
               isInfo = 0;
           }
           break;
       case 'e':
            if (isSelectFly) {
               isSelectFly = 0;
              // isInfo = 1;
           }
           if (isSelectButter) {
               isSelectButter = 0;
           }
           if (!isSelectBee) {
               isSelectBee = 1;
               isInfo = 0;
           }
           break;
       case 'f':
           if (isSelectBee) {
              isSelectBee = 0;
               isSelectButter = 1;
          }
          if (isSelectFly) {
              isSelectFly = 0;
              isSelectButter = 1;
          }
          else {
              isSelectButter = 1;
              isInfo = 0;
          }
           break;
       case 'w':
           if (isSelectBee)  {
               isSelectBee = 0;
               isInfo = 1;
           }
           if (isSelectFly){
               isSelectFly = 0;
               isInfo = 1;
           }
           if (isSelectButter){
               isSelectButter = 0;
               isInfo = 1;
           }
           break;
           
       case '1':
           north = 1;
           east  =  0;
           south = 0;
           west  = 0;
           above = 0;
           break;
       case '2':
           north = 0;
           east  =  1;
           south = 0;
           west  = 0;
           above = 0;
           break;
       case '3':
           north = 0;
           east  =  0;
           south = 1;
           west  = 0;
           above = 0;
           break;
       case '4':
           north = 0;
           east  =  0;
           south = 0;
           west  = 1;
           above = 0;
           break;
       case 'a':
           north = 0;
           east  =  0;
           south = 0;
           west  = 0;
           above = 1;
           break;
       case 'r':
           north = 0;
           east  =  0;
           south = 0;
           west  = 0;
           above = 0;
           break;
       case 'u':
           yVal+=2;
           break;
       case 'i':
           yVal-=2;
           break;
       case 'm':
           if (m > 0.0) m -= 0.05;
          glutPostRedisplay();
          break;
       case 'M':
           if (m < 1.0) m += 0.05;
           glutPostRedisplay();
           break;
       case 'v':
           if (shine > 0.0) shine -= 5.0;
          glutPostRedisplay();
          break;
       case 'V':
           if (shine < 100.0) shine += 5.0;
           glutPostRedisplay();
           break;
      default:
         break;
   }
}

void specialKeyInput(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT) angle = angle + 5.0;
    if (key == GLUT_KEY_RIGHT) angle = angle - 5.0;
    if( key == GLUT_KEY_UP)
    {
        if (distFrom()) {
            xVal = xVal;
            zVal=zVal;
        }
        else {
            xVal = xVal - 3*sin(angle * PI/180.0);
            zVal = zVal - 3*cos(angle * PI/180.0);
        }
    }
    if( key == GLUT_KEY_DOWN)
    {
        xVal = xVal + 3*sin(angle * PI/180.0);
        zVal = zVal + 3*cos(angle * PI/180.0);
    }

    // Angle correction.
    if (angle > 360.0) angle -= 360.0;
    if (angle < 0.0) angle += 360.0;
    glutSetWindow(bugWindow);
   glutPostRedisplay();
}

void printInteraction(void)
{
   cout << "Interactions:" << endl;
    cout << "Press 'd' to view the dragon fly" << endl;
    cout << "Click the dragon fly in either window to view the dragon fly" << endl;
    cout << "Press 'e' to view the bee" << endl;
    cout << "Click the bee in either window to view the bee" << endl;
    cout << "Press 'f' to view the butterfly" << endl;
    cout << "Click the butterfly in either window  to view the butterfly" << endl;
    cout << "Press 'w' to reset the selection" << endl;
    cout << "Click the selected bug again to reset the selection" << endl;
    cout << "Press ' ' (space bar) to halt flying animations to make selection easier" << endl;
    cout << "Press 1 to view the scene from the north " << endl;
     cout << "Press 2 to view the scene from the east" << endl;
     cout << "Press 3 to view the scene from the south" << endl;
     cout << "Press 4 to view the scene from the west" << endl;
     cout << "Press a to view the scene from above" << endl;
     cout << "Press r to view the scene as the movable player" << endl;
     cout << "Press u to move the player up" << endl;
     cout << "Press i to move the player down" << endl;
    cout << "Press the left key to turn the player view left" << endl;
    cout << "Press the right key to turn the player view right" << endl;
    cout << "Press the up key to move the player forwards" << endl;
    cout << "Press the down key to move the player backwards" << endl;
    cout << "Right click and select 'Quit' to quit" << endl;
    cout << "Right click and select 'Reset Bug Selection' to reset the bug selector" << endl;
    cout << "Right click and hover over 'Bug Options' to choose which bug you would like to learn about" << endl;
    cout << "Right click and hover over 'Color Options' to change the color of the light over the bug" << endl;
    cout << "Right click and hover over 'Sun Color Options' to change the color of the light outside" << endl;
    cout << "Press 'm' to decrease the global ambient light" << endl;
    cout << "Press 'M' to increase the global ambient light" << endl;
    cout << "Press 'v' to decrease the material shininess" << endl;
    cout << "Press 'V' to increase the material shininess" << endl;
}

void top_menu(int id)
{
   if (id==1) exit(0);
    if (id == 2) {
        if (isSelectFly) isSelectFly = 0;
        if (isSelectBee) isSelectBee = 0;
        if (isSelectButter) isSelectButter = 0;
        if (!isInfo) isInfo = 1;
    }
}

void orb_menu(int id)
{
   if (id==1)
   {
       if (isSelectBee) isSelectBee = 0;
       if (isSelectButter) isSelectButter = 0;
       isSelectFly = 1;
       isInfo= 0;
   }
   if (id==2)
   {
       if (isSelectFly) isSelectFly = 0;
       if (isSelectButter) isSelectButter = 0;
       isSelectBee = 1;
       isInfo = 0;
   }
   if (id==3)
   {
       if (isSelectFly) isSelectFly = 0;
       if (isSelectBee) isSelectBee = 0;
       isSelectButter = 1;
       isInfo = 0;
   }
   glutPostRedisplay();
}

void color_menu(int id)
{
   if (id==1)
   {
       r =1.0;
       b = 0.0;
       g = 0.0;
   }
    if (id==2)
    {
        r =0.0;
        b = 1.0;
        g = 0.0;
    }
   if (id==3)
   {
       r =0.0;
       b = 0.0;
       g = 1.0;
   }
    if (id==4)
    {
        r =ddifspec;
        g=ddifspec;
        b=ddifspec;
    }
   glutPostRedisplay();
}
void color_menu_sun(int id)
{
   if (id==1)
   {
       rSun =1.0;
       bSun = 0.0;
       gSun = 0.0;
   }
    if (id==2)
    {
        rSun =0.0;
        bSun = 1.0;
        gSun = 0.0;
    }
   if (id==3)
   {
       rSun =0.0;
       bSun = 0.0;
       gSun = 1.0;
   }
    if (id==4)
    {
        rSun =.1;
        gSun=.1;
        bSun=.1;
    }
   glutPostRedisplay();
}
void makeMenu(void)
{
   int sub_menu;
   sub_menu = glutCreateMenu(orb_menu);
   glutAddMenuEntry("Dragonfly", 1);
   glutAddMenuEntry("Bee", 2);
   glutAddMenuEntry("Butterfly",3);
    int sub_menu2;
   sub_menu2 = glutCreateMenu(color_menu);
   glutAddMenuEntry("Red", 1);
   glutAddMenuEntry("Blue",2);
   glutAddMenuEntry("Green",3);
   glutAddMenuEntry("White",4);
    int sub_menu3;
   sub_menu3 = glutCreateMenu(color_menu_sun);
   glutAddMenuEntry("Red", 1);
   glutAddMenuEntry("Blue",2);
   glutAddMenuEntry("Green",3);
   glutAddMenuEntry("White",4);

   glutCreateMenu(top_menu);
   glutAddSubMenu("Bug Options", sub_menu);
   glutAddSubMenu("Color Options", sub_menu2);
   glutAddSubMenu("Sun Color Options", sub_menu3);
    glutAddMenuEntry("Reset Bug Selection", 2);
   glutAddMenuEntry("Quit",1);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// Initialization routine.
void setup(void)
{
    if (isSunny ){
        glClearColor(.7, .9, 1.0, 0.0);
    }
    else {
        glClearColor(.1, .1, .1, 0.0);
    }
    srand((unsigned) time(0));
    beer = rand();

    glGenTextures(3, texture);

    loadExternalTextures();

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Initialization routine.
void setupBug(void)
{
    glClearColor(1, 1, 1, 0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
    gluPerspective(65,1,1,250);
   glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    height = h;
}

// OpenGL window reshape routine.
void resizeBug(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
     gluPerspective(65,1,1,200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    heightBug=h;
    widthBug = w;
}

void mouseControl(int button, int state, int x, int y)
{
    if(state==GLUT_DOWN && button == GLUT_LEFT)
    {
        selecting=true;
        xClick=x;
        yClick=height-y; //for mouse vs window coordinates
    }
}
void mouseBug(int button, int state, int x, int y)
{
    glutSetWindow(infoWindow);
    x = x - (widthBug / 2);
    y = heightBug-y-(heightBug/2);
    x = x/10;
    y = y/10;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (x >= -23 && x <= -10 && y >= -4 && y <= 7 ) {
            isSelectFly = 1;
            isInfo = 0;
        }
        if (x >= -5 && x <= 6 && y >= -4 && y <= 7 ) {
            isSelectBee = 1;
            isInfo = 0;
        }
        if (x >= 9 && x <= 22 && y >= -4 && y <= 7 ) {
            isSelectButter = 1;
            isInfo = 0;
        }
    }
}
// Main routine.
int main(int argc, char **argv)
{
    glutTimerFunc(5, animate, 1);
    glutIdleFunc(beeSystem);
    particleSetUp();

    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(50, 100);
    bugWindow = glutCreateWindow ("Bug Garden");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutMouseFunc(mouseControl);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    makeMenu();

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(900, 100);
    infoWindow = glutCreateWindow("Bug Info");
    setupBug();
    glutDisplayFunc(drawSceneBug);
    glutReshapeFunc(resizeBug);
    glutMouseFunc(mouseBug);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    makeMenu();
    glutMainLoop();

   return 0;
}


