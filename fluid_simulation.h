#pragma once

/* Set boundary of simulation box */
void SetBoundary(int b, float *x);

/* Solve linear differential equation */
void LinSolve(int b, float *x, float *x0, float a, float c);

/* Make equation linear */
void Diffuse (int b, float *x, float *x0, float diff);

//TODO: Check what it does
void Project(float *velocX, float *velocY, float *p, float *div);

/* Move object by vector */
void AddVection(int b, float *d, float *d0,  float *velocX, float *velocY);
