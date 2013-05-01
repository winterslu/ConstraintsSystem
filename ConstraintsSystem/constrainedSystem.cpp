/*
 
 USC/Viterbi/Computer Science
 "Constrained Particle System" Assignment 3
 System structure by Ke "Winters" Lu
 
 */

#include "jello.h"
#include "constrainedSystem.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_sf_bessel.h>

PerlinNoise PN;

void InitialData(struct chain * chain)
{
    chain->totalMass = TOTALMASS;
    chain->dt = TIMESTEP;
    chain->number = 10;
    
    chain->p[0].x = 0;chain->p[0].y = 0;chain->p[0].z = 0;
    chain->p[1].x = 0;chain->p[1].y = -0.1;chain->p[1].z = 0;
    chain->p[2].x = 0;chain->p[2].y = -0.2;chain->p[2].z = 0;
    chain->p[3].x = 0;chain->p[3].y = -0.3;chain->p[3].z = 0;
    chain->p[4].x = 0;chain->p[4].y = -0.4;chain->p[4].z = 0;
    chain->p[5].x = 0;chain->p[5].y = -0.5;chain->p[5].z = 0;
    chain->p[6].x = 0.1;chain->p[6].y = -0.5;chain->p[6].z = 0;
    chain->p[7].x = 0.2;chain->p[7].y = -0.5;chain->p[7].z = 0;
    chain->p[8].x = 0.3;chain->p[8].y = -0.5;chain->p[8].z = 0;
    chain->p[9].x = 0.4;chain->p[9].y = -0.5;chain->p[9].z = 0;
    chain->p[10].x = 0.5;chain->p[10].y = -0.5;chain->p[10].z = 0;
//    chain->p[0].x = 0;chain->p[0].y = 0;chain->p[0].z = 0;
//    chain->p[1].x = 0.1;chain->p[1].y = 0;chain->p[1].z = 0;
//    chain->p[2].x = 0.2;chain->p[2].y = 0;chain->p[2].z = 0;
//    chain->p[3].x = 0.3;chain->p[3].y = 0;chain->p[3].z = 0;
//    chain->p[4].x = 0.4;chain->p[4].y = 0;chain->p[4].z = 0;
//    chain->p[5].x = 0.5;chain->p[5].y = 0;chain->p[5].z = 0;
//    chain->p[6].x = 0.6;chain->p[6].y = 0;chain->p[6].z = 0;
//    chain->p[7].x = 0.7;chain->p[7].y = 0;chain->p[7].z = 0;
//    chain->p[8].x = 0.8;chain->p[8].y = 0;chain->p[8].z = 0;
//    chain->p[9].x = 0.9;chain->p[9].y = 0;chain->p[9].z = 0;
//    chain->p[10].x = 1.0;chain->p[10].y = 0;chain->p[10].z = 0;
    
    for(int i=0; i<=chain->number; i++)
    {
        chain->v[i].x = 0.0;
        chain->v[i].y = 0.0;
        chain->v[i].z = 0.0;
    }
    
    for(int i=0; i<=chain->number; i++)
    {
        chain->f[i].x = 0.0;
        chain->f[i].y = 0.0;
        chain->f[i].z = 0.0;
    }
}


void computePhysics(struct chain * chain, struct point a[])
{
    
    int n = chain->number;
    double mass = chain->totalMass/(chain->number+1.0);
    
    gsl_matrix *U = gsl_matrix_alloc (n*3+1, n*3+1);
    gsl_matrix_set_zero(U);
    
    gsl_matrix_view M = gsl_matrix_submatrix(U, 0, 0, n*2, n*2);
    gsl_matrix_view nC = gsl_matrix_submatrix(U, n*2, 0, n+1, n*2);
    gsl_matrix_view nCt = gsl_matrix_submatrix(U, 0, n*2, n*2, n+1);

    
    //Set Matrix M
    for(int i = 0; i < n*2; i++)
        gsl_matrix_set(&M.matrix, i, i, mass/n);
    
    //Set Matrix NablaC
    gsl_matrix_set(&nC.matrix, 0, 0, chain->p[1].x*2.0);
    gsl_matrix_set(&nC.matrix, 0, 1, chain->p[1].y*2.0);
    for(int i = 1; i < n; i++)
    {
        gsl_matrix_set(&nC.matrix, i, i*2-2, (chain->p[i].x - chain->p[i+1].x)*2.0);
        gsl_matrix_set(&nC.matrix, i, i*2-1, (chain->p[i].y - chain->p[i+1].y)*2.0);
        gsl_matrix_set(&nC.matrix, i, i*2,   (chain->p[i+1].x - chain->p[i].x)*2.0);
        gsl_matrix_set(&nC.matrix, i, i*2+1, (chain->p[i+1].y - chain->p[i].y)*2.0);
    }
    if(isConstraint)
    {
        gsl_matrix_set(&nC.matrix, n, n*2-2, chain->p[n].x*2.0);
        gsl_matrix_set(&nC.matrix, n, n*2-1, chain->p[n].y*2.0 + 1.0);
    }else
    {
        gsl_matrix_set(&nC.matrix, n, n*2-2, 0.0);
        gsl_matrix_set(&nC.matrix, n, n*2-1, 0.0);
    }
    
    //Set Matrix NablaCt
    gsl_matrix_set(&nCt.matrix, 0, 0, chain->p[1].x*2.0);
    gsl_matrix_set(&nCt.matrix, 1, 0, chain->p[1].y*2.0);
    for(int i = 1; i < n; i++)
    {
        gsl_matrix_set(&nCt.matrix, i*2-2, i, (chain->p[i].x - chain->p[i+1].x)*2.0);
        gsl_matrix_set(&nCt.matrix, i*2-1, i, (chain->p[i].y - chain->p[i+1].y)*2.0);
        gsl_matrix_set(&nCt.matrix, i*2, i,   (chain->p[i+1].x - chain->p[i].x)*2.0);
        gsl_matrix_set(&nCt.matrix, i*2+1, i, (chain->p[i+1].y - chain->p[i].y)*2.0);
    }
    if(isConstraint)
    {
        gsl_matrix_set(&nCt.matrix, n*2-2, n, chain->p[n].x*2.0);
        gsl_matrix_set(&nCt.matrix, n*2-1, n, chain->p[n].y*2.0 + 1.0);
    }else
    {
        gsl_matrix_set(&nCt.matrix, n*2-2, n, 0.0);
        gsl_matrix_set(&nCt.matrix, n*2-1, n, 0.0);
    }
    
    gsl_matrix *V = gsl_matrix_alloc(n*3+1, n*3+1);
    gsl_vector *s = gsl_vector_alloc(n*3+1);
    gsl_vector *workvec = gsl_vector_alloc(n*3+1);
    
    
//    for (int i = 0; i < n*2+n+1; i++)
//    {
//        for (int j = 0; j < n*2+n+1; j++)
//            printf ("%.1f  ", gsl_matrix_get (U, i, j));
//        printf ("\n");
//    }
    gsl_linalg_SV_decomp(U, V, s, workvec);

    
    
    //Filter
    double max = gsl_vector_max(s);
    for(int i=0; i<n*3+1; i++)
        if(gsl_vector_get(s, i) < max * 0.000001)
            gsl_vector_set(s, i, 0.0);
    
    
    
    
    gsl_vector *b = gsl_vector_alloc(n*3+1);
    gsl_vector *b1 = gsl_vector_alloc(n+1);
    gsl_vector *x = gsl_vector_alloc(n*3+1);
    gsl_vector_set_zero(b);
    gsl_vector_set_zero(b1);
    
    gsl_vector_view fext = gsl_vector_subvector(b, 0, n*2);
    gsl_vector_view bs = gsl_vector_subvector(b, n*2, n+1);
    
    
    //Set Vector fext
    for(int i = 0; i < n; i++)
    {
        gsl_vector_set(&fext.vector, i*2,   chain->f[i+1].x);
        gsl_vector_set(&fext.vector, i*2+1, chain->f[i+1].y);
    }

    //Set Vector bs
    gsl_vector_set(&bs.vector, 0, - chain->v[1].x*chain->v[1].x*2.0 - chain->v[1].y*chain->v[1].y*2.0);
    for(int i = 1; i < n; i++)
        gsl_vector_set(&bs.vector, i, 
                       - (chain->v[i].x-chain->v[i+1].x)*chain->v[i].x*2.0 -
                       (chain->v[i].y-chain->v[i+1].y)*chain->v[i].y*2.0 -
                       (chain->v[i+1].x-chain->v[i].x)*chain->v[i+1].x*2.0 -
                       (chain->v[i+1].y-chain->v[i].y)*chain->v[i+1].y*2.0
                       );
    if(isConstraint)
        gsl_vector_set(&bs.vector, n, -chain->v[n].x*chain->v[n].x*2.0 - (chain->v[n].y*2.0+1.0)* chain->v[n].y);
    else  gsl_vector_set(&bs.vector, n, 0.0);

    
    //Compute Baumgarte stabilization
    gsl_vector_set(b1, 0, - chain->p[1].x*chain->v[1].x*2.0 - chain->p[1].y*chain->v[1].y*2.0);
    for(int i = 1; i < n; i++)
        gsl_vector_set(b1, i, 
                       - (chain->p[i].x-chain->p[i+1].x)*chain->v[i].x*2.0 -
                       (chain->p[i].y-chain->p[i+1].y)*chain->v[i].y*2.0 -
                       (chain->p[i+1].x-chain->p[i].x)*chain->v[i+1].x*2.0 -
                       (chain->p[i+1].y-chain->p[i].y)*chain->v[i+1].y*2.0
                       );
    if(isConstraint)
        gsl_vector_set(b1, n, -chain->p[n].x*chain->v[n].x*2.0 - (chain->p[n].y*2.0+1.0)* chain->v[n].y);
    else gsl_vector_set(b1, n, 0.0);
    
    gsl_vector_scale(b1, BSALPHA * 2.0);
    gsl_vector_add(&bs.vector, b1);
    gsl_vector_set_zero(b1);
    
    
    gsl_vector_set(b1, 0, - chain->p[1].x*chain->p[1].x - chain->p[1].y*chain->p[1].y + 0.01);
    for(int i = 1; i < n; i++)
        gsl_vector_set(b1, i, 
                       - (chain->p[i+1].x-chain->p[i].x) * (chain->p[i+1].x-chain->p[i].x) 
                       - (chain->p[i+1].y-chain->p[i].y) * (chain->p[i+1].y-chain->p[i].y) + 0.01
                       );
    if(isConstraint)
        gsl_vector_set(b1, n, - chain->p[n].x*chain->p[n].x - (chain->p[n].y+0.5) * (chain->p[n].y+0.5) + 0.25);
    else gsl_vector_set(b1, n, 0.0);
    
    gsl_vector_scale(b1, BSALPHA * BSALPHA);
    gsl_vector_add(&bs.vector, b1);
    

    
    //Solve The Equation
    gsl_linalg_SV_solve(U, V, s, b, x);
    
    for(int i=0; i<chain->number; i++)
    {
        a[i].x = gsl_vector_get(x, i*2);
        a[i].y = gsl_vector_get(x, i*2+1);
    }
    
//    for (int i = n*2; i < n*3+1; i++)
//    {
//        printf ("%g  ", gsl_vector_get(b, i));
//        printf ("\n");
//    }
//    printf ("\n");
    
    
    gsl_vector_free(x);
    gsl_vector_free(b1);
    gsl_vector_free(b);
    
    gsl_vector_free(workvec);
    gsl_vector_free(s);
    gsl_matrix_free(V);
    
    gsl_matrix_free(U);
    
}



/* Computes acceleration to every control point of the chain, 
 which is in state given by 'chain'.
 Returns result in array 'a'. */
void computeAcceleration(struct chain * chain, struct point a[])
{
    for(int i=1; i<=chain->number; i++)
    {
        chain->f[i].x = 0;
        chain->f[i].y = 0;
    }
    
    
    if(isRandomforce)
        for(int i=1; i<=chain->number; i++)
        {
            chain->f[i].x = PN.PerlinNoise1D(tempCount++, 1000, 0.7) * 500;
            chain->f[i].y = PN.PerlinNoise1D(tempCount, 1000, 0.7) * 500;
        }
    
    
    
    
    //Gravity Force
    if(isGravity)
        for(int i=1; i<=chain->number; i++)
            chain->f[i].y = -GRAVITY;
    
    //Damping Force
    if(isDamping)
        for(int i=1; i<=chain->number; i++)
        {
            chain->f[i].x += - (chain->v[i].x * DAMPINGCOEF);
            chain->f[i].y += - (chain->v[i].y * DAMPINGCOEF);
        }

    
    //User Force
    for(int i=1; i<=chain->number; i++)
        chain->f[i].x += g_vDiffPos[0] * 0.5;
    for(int i=1; i<=chain->number; i++)
        chain->f[i].y += g_vDiffPos[1] * 0.5;
    
    if(pushUp)
        for(int i=1; i<=chain->number; i++)
            chain->f[i].y += USERFORCE;
    if(pushDown)
        for(int i=1; i<=chain->number; i++)
            chain->f[i].y -= USERFORCE;
    if(pushLeft)
        for(int i=1; i<=chain->number; i++)
            chain->f[i].x -= USERFORCE;
    if(pushRight)
        for(int i=1; i<=chain->number; i++)
            chain->f[i].x += USERFORCE;
    
    //Plane Collision
    if(isPlane)
        for(int i=1; i<=chain->number; i++)
            if(chain->p[i].y > 0)
            {
                chain->f[i].y += -1.0 * chain->p[i].y * KCOLLISION;
            }
    
    computePhysics(chain, a);
    
}



/* performs one step of Euler Integration */
/* as a result, updates the chain structure */
void Euler(struct chain * chain)
{
    point a[chain->number];
    
    computeAcceleration(chain, a);
    
    for (int i=1; i<=chain->number; i++)
    {
        chain->p[i].x += chain->dt * chain->v[i].x;
        chain->p[i].y += chain->dt * chain->v[i].y;
        chain->v[i].x += chain->dt * a[i-1].x;
        chain->v[i].y += chain->dt * a[i-1].y;
    }
}


/* performs one step of RK4 Integration */
/* as a result, updates the chain structure */
void RK4(struct chain * chain)
{
    point F1p[chain->number], F1v[chain->number], 
        F2p[chain->number], F2v[chain->number],
        F3p[chain->number], F3v[chain->number],
        F4p[chain->number], F4v[chain->number];
    
    point a[chain->number];
    
    struct chain buffer = *chain;
    
    computeAcceleration(chain, a);
    
    for (int i=1; i<=chain->number; i++)
    {
        pMULTIPLY(chain->v[i],chain->dt,F1p[i-1]);
        pMULTIPLY(a[i-1],chain->dt,F1v[i-1]);
        pMULTIPLY(F1p[i-1],0.5,buffer.p[i]);
        pMULTIPLY(F1v[i-1],0.5,buffer.v[i]);
        pSUM(chain->p[i],buffer.p[i],buffer.p[i]);
        pSUM(chain->v[i],buffer.v[i],buffer.v[i]);
    }
    
    computeAcceleration(&buffer, a);
    
    for (int i=1; i<=chain->number; i++)
    {
        // F2p = dt * buffer.v;
        pMULTIPLY(buffer.v[i], chain->dt, F2p[i-1]);
        // F2v = dt * a(buffer.p,buffer.v);     
        pMULTIPLY(a[i-1], chain->dt, F2v[i-1]);
        pMULTIPLY(F2p[i-1], 0.5, buffer.p[i]);
        pMULTIPLY(F2v[i-1], 0.5, buffer.v[i]);
        pSUM(chain->p[i], buffer.p[i], buffer.p[i]);
        pSUM(chain->v[i], buffer.v[i], buffer.v[i]);
    }
    
    computeAcceleration(&buffer, a);
    
    for (int i=1; i<=chain->number; i++)
    {
        // F3p = dt * buffer.v;
        pMULTIPLY(buffer.v[i], chain->dt, F3p[i-1]);
        // F3v = dt * a(buffer.p,buffer.v);     
        pMULTIPLY(a[i-1], chain->dt, F3v[i-1]);
        pMULTIPLY(F3p[i-1], 0.5, buffer.p[i]);
        pMULTIPLY(F3v[i-1], 0.5, buffer.v[i]);
        pSUM(chain->p[i], buffer.p[i], buffer.p[i]);
        pSUM(chain->v[i], buffer.v[i], buffer.v[i]);
    }
    
    computeAcceleration(&buffer, a);
    
    
    for (int i=1; i<=chain->number; i++)
    {
        // F3p = dt * buffer.v;
        pMULTIPLY(buffer.v[i], chain->dt, F4p[i-1]);
        // F3v = dt * a(buffer.p,buffer.v);     
        pMULTIPLY(a[i-1], chain->dt, F4v[i-1]);
        
        pMULTIPLY(F2p[i-1], 2, buffer.p[i]);
        pMULTIPLY(F3p[i-1], 2, buffer.v[i]);
        pSUM(buffer.p[i], buffer.v[i], buffer.p[i]);
        pSUM(buffer.p[i], F1p[i-1], buffer.p[i]);
        pSUM(buffer.p[i], F4p[i-1], buffer.p[i]);
        pMULTIPLY(buffer.p[i], 1.0 / 6, buffer.p[i]);
        pSUM(buffer.p[i], chain->p[i], chain->p[i]);
        
        pMULTIPLY(F2v[i-1], 2, buffer.p[i]);
        pMULTIPLY(F3v[i-1], 2, buffer.v[i]);
        pSUM(buffer.p[i], buffer.v[i], buffer.p[i]);
        pSUM(buffer.p[i], F1v[i-1], buffer.p[i]);
        pSUM(buffer.p[i], F4v[i-1], buffer.p[i]);
        pMULTIPLY(buffer.p[i], 1.0 / 6, buffer.p[i]);
        pSUM(buffer.p[i], chain->v[i], chain->v[i]);
    }
    
    return; 
    
}





