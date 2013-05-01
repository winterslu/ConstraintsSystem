/*
 
 USC/Viterbi/Computer Science
 "Constrained Particle System" Assignment 3
 System structure by Ke "Winters" Lu
 
 */

#include "jello.h"
#include "input.h"
//#include "ActorPicking.h"

/* Write a screenshot, in the PPM format, to the specified filename, in PPM format */
void saveScreenshot(int windowWidth, int windowHeight, char *filename)
{
  if (filename == NULL)
    return;

  // Allocate a picture buffer 
  Pic * in = pic_alloc(windowWidth, windowHeight, 3, NULL);

  printf("File to save to: %s\n", filename);

  for (int i=windowHeight-1; i>=0; i--) 
  {
    glReadPixels(0, windowHeight-i-1, windowWidth, 1, GL_RGB, GL_UNSIGNED_BYTE,
      &in->pix[i*in->nx*in->bpp]);
  }

  if (ppm_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");

  pic_free(in);
}

/* converts mouse drags into information about rotation/translation/scaling */
void mouseMotionDrag(int x, int y)
{
  int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};

  if (g_iRightMouseButton) // handle camera rotations
  {
    Phi += vMouseDelta[0] * 0.01;
    Theta += vMouseDelta[1] * 0.01;
    
    if (Phi>2*pi)
      Phi -= 2*pi;
    
    if (Phi<0)
      Phi += 2*pi;
    
    if (Theta>pi / 2 - 0.01) // dont let the point enter the north pole
      Theta = pi / 2 - 0.01;
    
    if (Theta<- pi / 2 + 0.01)
      Theta = -pi / 2 + 0.01;
    
    g_vMousePos[0] = x;
    g_vMousePos[1] = y;
  }
    
  if (g_iLeftMouseButton)
  {
      g_vDiffPos[0] = x - g_vLeftPressPos[0];
      g_vDiffPos[1] = g_vLeftPressPos[1] - y;
      //Pick(x, y);
      //mouseMotion(x, y);
  }

}

void mouseMotion (int x, int y)
{
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mouseButton(int button, int state, int x, int y)
{
  switch (button)
  {
    case GLUT_LEFT_BUTTON:
      if(!g_iLeftMouseButton && !state)
      {
          g_vLeftPressPos[0] = x;
          g_vLeftPressPos[1] = y;
      }else if(g_iLeftMouseButton && state)
      {
          g_vDiffPos[0] = 0;
          g_vDiffPos[1] = 0;
      }
      g_iLeftMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_MIDDLE_BUTTON:
      g_iMiddleMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_RIGHT_BUTTON:
      g_iRightMouseButton = (state==GLUT_DOWN);
      break;
  }
 
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

// gets called whenever a key is pressed
void keyboardFunc (unsigned char key, int x, int y)
{
    printf("%d ", key);
  switch (key)
  {
    case 27:
      exit(0);
      break;

    case 'e':
      isPlane = !isPlane;
      break;

    case 'v':
      viewingMode = 1 - viewingMode;
      break;
          
    case 'g':
      isGravity = !isGravity;
      break;
          
    case 'd':
      isDamping = !isDamping;
      break;
          
    case 'r':
      isRandomforce = !isRandomforce;
      break;

    case 'p':
      Pause = 1 - Pause;
      break;
          
    case 't':
      isRK4 = !isRK4;
      break;
    
    case 'c':
      isConstraint = !isConstraint;
      break;
    
          

    case 'z':
      R -= 0.1;
      if (R < 0.1)
        R = 0.1;
      break;

    case 'x':
      R += 0.1;
      break;

    case ' ':
      saveScreenToFile = 1 - saveScreenToFile;
      break;
          
    case 44:
      pushLeft = 1;
      break;
          
    case 107:
      pushUp = 1;
      break;
          
    case 46:
      pushRight = 1;
      break;
          
    case 108:
      pushDown = 1;
      break;
          
  }
}
