#include <stdlib.h>
#include "fluid_point.h"

void FluidPointFree(FluidPoint *point)
{
    free(point->s);
    free(point->density);
    
    free(point->vxCurr);
    free(point->vyCurr);
    
    free(point->vxPrev);
    free(point->vyPrev);
    
    free(point);
}

/* Works like constructor */
FluidPoint* FluidPointCreate(int size, int diffusion, int viscosity, float dt)
{
    FluidPoint *point = malloc(sizeof(FluidPoint));
    int N = size;
    
    point->size = size;
    point->dt = dt;
    point->diff = diffusion;
    point->visc = viscosity;
    
    point->s = calloc(N * N, sizeof(float));
    point->density = calloc(N * N, sizeof(float));
    
    point->vxCurr = calloc(N * N, sizeof(float));
    point->vyCurr = calloc(N * N, sizeof(float));
    
    point->vxPrev = calloc(N * N, sizeof(float));
    point->vyPrev = calloc(N * N, sizeof(float));
    
    return point;
}
