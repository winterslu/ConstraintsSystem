/*
 
 USC/Viterbi/Computer Science
 "Constrained Particle System" Assignment 3
 System structure by Ke "Winters" Lu
 
 */

#include "jello.h"
#include "showObj.h"
#include "input.h"
#include "physics.h"
#include "lighting.h"
#include "constrainedSystem.h"

// camera parameters
double Theta = 0;//pi / 6;
double Phi = 0;//pi / 6;
double R = 1;

// Recording parameters
double timestamp;

// mouse control
int g_iMenuId;
int g_vMousePos[2];
int g_iLeftMouseButton,g_iMiddleMouseButton,g_iRightMouseButton;
int g_vLeftPressPos[2];
int g_vDiffPos[2];
int g_iLastCircle[2];
int g_iCircle[2];

// number of images saved to disk so far
int sprite=0;

// temp
double tempCount=0;

// these variables control what is displayed on screen
int Pause=0, viewingMode=0, saveScreenToFile=0;
bool isGravity=false, isDamping=true, isRandomforce=false, isRK4=true, isConstraint=true, isPlane=true;
int pushUp=0, pushDown=0, pushLeft=0, pushRight=0;

// TimeCounter
PerformanceCounter timeCounter;
int fps = 0;

//struct world jello;
struct chain chain;

int windowWidth, windowHeight;

//std::ofstream an("/Users/Winters/Desktop/output0.txt");

void myinit()
{
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0,1.0,0.01,1000.0);
    
    // set background color to grey
    glClearColor(0.5, 0.5, 0.5, 0.0);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_MULTISAMPLE);
    
    // lighting initialization
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aGa);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    
    
    
    InitialData(&chain);
    // Analysis
    // **********
    //if(an.fail()) 
    //    return;
    // **********
    
    
    return; 
}

void reshape(int w, int h) 
{
    // Prevent a divide by zero, when h is zero.
    // You can't make a window of zero height.
    if(h == 0)
        h = 1;
    
    glViewport(0, 0, w, h);
    
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the perspective
    double aspectRatio = 1.0 * w / h;
    gluPerspective(60.0f, aspectRatio, 0.01f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 
    
    windowWidth = w;
    windowHeight = h;
    
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    // camera parameters are Phi, Theta, R
    gluLookAt(R * sin(Phi) * cos (Theta), R * sin(Theta) - 0.5, R * cos(Phi) * cos(Theta), 0.0,-0.5,0.0, 0.0,1.0,0.0);
    
    // macro to set up light i
#define LIGHTSETUP(i)\
glLightfv(GL_LIGHT##i, GL_POSITION, lP##i);\
glLightfv(GL_LIGHT##i, GL_AMBIENT, lKa##i);\
glLightfv(GL_LIGHT##i, GL_DIFFUSE, lKd##i);\
glLightfv(GL_LIGHT##i, GL_SPECULAR, lKs##i);\
glEnable(GL_LIGHT##i)
    
    LIGHTSETUP (0);
    //LIGHTSETUP (1);
    //LIGHTSETUP (2);
    //LIGHTSETUP (3);
    //LIGHTSETUP (4);
    //LIGHTSETUP (5);
    //LIGHTSETUP (6);
    //LIGHTSETUP (7);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
    //glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5);
    
    // set up box color
    glMaterialfv(GL_FRONT, GL_AMBIENT, mBa);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mBd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mBs);
    glMaterialfv(GL_FRONT, GL_EMISSION, mBe);
    glMaterialf(GL_FRONT, GL_SHININESS, 20);
    // show the bounding box
    
    if(isConstraint)
        showCircle();
    
    showAxis();
    
    if (isPlane) {
        showPlane();
    }
    
    
    showChain(&chain);
    
    
    
    //showBoundingBox();
    // show the inclined plane
    //showInclinedPlane(&jello);
    
    
    // set up cube color
//    glMaterialfv(GL_FRONT, GL_AMBIENT, mKa);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, mKd);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
//    glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
//    glMaterialf(GL_FRONT, GL_SHININESS, 240);
    // show the cube
    
    
    
    
    
    // HUD
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, windowWidth, windowHeight, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1.0, 1.0, 0.0);
    char temp[80] ="";
    sprintf(temp, "Gravity Switch(key:'g'): %s", (isGravity)?"ON":"OFF");
    showGUIText(10, 20, temp);
    sprintf(temp, "Damping Switch(key:'d'): %s", (isDamping)?"ON":"OFF");
    showGUIText(10, 40, temp);
    sprintf(temp, "RandomF Switch(key:'r'): %s", (isRandomforce)?"ON":"OFF");
    showGUIText(10, 60, temp);
    sprintf(temp, "Plane Switch(key:'e'): %s", (isPlane)?"ON":"OFF");
    showGUIText(10, 80, temp);
    sprintf(temp, "Constraint Switch(key:'c'): %s", (isConstraint)?"ON":"OFF");
    showGUIText(10, 100, temp);
    sprintf(temp, "RK4 Euler Switch(key:'t'): %s", (isRK4)?"RK4":"Euler");
    showGUIText(10, 120, temp);
    sprintf(temp, "User Force up:'k',down:'l',left:'<',right:'>' and Left Mouse");
    showGUIText(10, 140, temp);
    sprintf(temp, "FPS:%d frame/s", fps);
    showGUIText(10, 160, temp);
    sprintf(temp, "Press Space to save Screen.");
    showGUIText(10, 180, temp);
    
    
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex2f(g_vLeftPressPos[0], g_vLeftPressPos[1]);
        glVertex2f(g_vLeftPressPos[0]+g_vDiffPos[0], g_vLeftPressPos[1]-g_vDiffPos[1]);
    glEnd();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glutSwapBuffers();
}

void doIdle()
{
    /* Calculate FPS */
    
    timeCounter.StopCounter();
    timestamp += timeCounter.GetElapsedTime();
    //printf("FPS:%d\n",int(1/timeCounter.GetElapsedTime()));
    fps = int(1/timeCounter.GetElapsedTime());
    timeCounter.StartCounter();
    
    
    char s[20]="picxxxx.ppm";
    // save screen to file
    s[3] = 48 + (sprite / 1000);
    s[4] = 48 + (sprite % 1000) / 100;
    s[5] = 48 + (sprite % 100 ) / 10;
    s[6] = 48 + sprite % 10;
    if (saveScreenToFile==1 && timestamp >= 1/15)
    {
        //saveScreenshot(windowWidth, windowHeight, s);
        saveScreenToFile=0; // save only once, change this if you want continuos image generation (i.e. animation)
        sprite++;
        timestamp = 0;
    }
    
    
    
    if (Pause == 0)
    {
        //Pause = 1;
        tempCount += 0.01;
        if(!isConstraint)
        {
            g_iLastCircle[0] = chain.p[10].x;
            g_iLastCircle[1] = chain.p[10].y;
        }
        
        if(isRK4)
            RK4(&chain);
        else Euler(&chain);
        
        if(!isConstraint)
        {
            g_iCircle[0] = chain.p[10].x - g_iLastCircle[0];
            g_iCircle[1] = chain.p[10].y - g_iLastCircle[1];
        }
        
        pushUp=pushDown=pushLeft=pushRight=0;
        
    }
    
    glutPostRedisplay();
}

int main (int argc, char ** argv)
{
    
    
    glutInit(&argc,argv);
    
    /* double buffered window, use depth testing, 640x480 */
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE | GLUT_DEPTH);
    
    windowWidth = 640;
    windowHeight = 480;
    glutInitWindowSize (windowWidth, windowHeight);
    glutInitWindowPosition (300,200);
    glutCreateWindow ("Constraint System");
    
    /* tells glut to use a particular display function to redraw */
    glutDisplayFunc(display);
    
    /* replace with any animate code */
    glutIdleFunc(doIdle);
    
    /* callback for mouse drags */
    glutMotionFunc(mouseMotionDrag);
    
    /* callback for window size changes */
    glutReshapeFunc(reshape);
    
    /* callback for mouse movement */
    glutPassiveMotionFunc(mouseMotion);
    
    /* callback for mouse button changes */
    glutMouseFunc(mouseButton);
    
    /* register for keyboard events */
    glutKeyboardFunc(keyboardFunc);
    
    /* do initialization */
    myinit();
    
    /* forever sink in the black hole */
    glutMainLoop();
    
    return(0);
}

