/*
 
 USC/Viterbi/Computer Science
 "Constrained Particle System" Assignment 3
 System structure by Ke "Winters" Lu
 
 */


#ifndef _SHOWOBJ_H_
#define _SHOWOBJ_H_




//int pointMap(int side, int i, int j);

//void showCube(struct world * jello);

void showChain(struct chain * chain);

void showCircle();

void showAxis();

void showPlane();

void showGUIText(GLfloat x, GLfloat y, char *string);

void showBoundingBox();

void showInclinedPlane(struct world * jello);

#endif
