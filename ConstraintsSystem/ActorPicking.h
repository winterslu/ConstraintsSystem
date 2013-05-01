/*
 
 USC/Viterbi/Computer Science
 "Constrained Particle System" Assignment 3
 System structure by Ke "Winters" Lu
 
 */

#ifndef JelloCube_ActorPicking_h
#define JelloCube_ActorPicking_h

void LetGoActor();
bool Pick(int x, int y);
bool PickActor(int x, int y);
void MoveActor(int x, int y);
void ViewProject(point * v, int &xi, int &yi, float &depth);
void ViewUnProject(int xi, int yi, float depth, point * v);


#endif
