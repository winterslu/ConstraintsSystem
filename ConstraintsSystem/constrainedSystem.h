/*
 
 USC/Viterbi/Computer Science
 "Constrained Particle System" Assignment 3
 System structure by Ke "Winters" Lu
 
 */

#ifndef ConstraintsSystem_constrainedSystem_h
#define ConstraintsSystem_constrainedSystem_h

#define GRAVITY     10
#define DAMPINGCOEF 10
#define USERFORCE   700
#define KCOLLISION  10000
#define TIMESTEP    0.00005
#define TOTALMASS   1.0



#define BSALPHA     200

void InitialData(struct chain * chain);

void computePhysics(struct chain * chain, struct point a[]);

void computeAcceleration(struct chain * chain, struct point a[]);

void Euler(struct chain * chain);

void RK4(struct chain * chain);

#endif
