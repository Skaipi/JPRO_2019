#include <stdlib.h>
#include "fluid_block.h"
#include "fluid_simulation.h"

extern const unsigned int WindowSize;
extern const unsigned int Iter;
extern const double dt;

#define N WindowSize

void FluidBlocktFree(FluidBlock *block)
{
    free(block->s);
    free(block->density);
    
    free(block->vxCurr);
    free(block->vyCurr);
    
    free(block->vxPrev);
    free(block->vyPrev);
    
    free(block);
}

FluidBlock* FluidBlockCreate(int diffusion, int viscosity)
{
    FluidBlock *block = (FluidBlock *)malloc(sizeof(FluidBlock));
    
    block->size = N;
    block->dt = dt;
    block->diff = diffusion;
    block->visc = viscosity;
    
    block->s = calloc(N * N, sizeof(float));
    block->density = calloc(N * N, sizeof(float));

    block->vxCurr = calloc(N * N, sizeof(float));
    block->vyCurr = calloc(N * N, sizeof(float));

    block->vxPrev = calloc(N * N, sizeof(float));
    block->vyPrev = calloc(N * N, sizeof(float));
    
    return block;
}

void FluidBlockAddDensity(FluidBlock *block, int x, int y, float amount)
{
    block->density[x + y*N] += amount;
}

void FluidBlockAddVelocity(FluidBlock *block, int x, int y, float amountX, float amountY)
{
    int index = x + y*N;
    
    block->vxCurr[index] += amountX;
    block->vyCurr[index] += amountY;
}

void FluidBlockSimulationStep(FluidBlock *block)
{
	//TODO: Check if possible without deep copy
	float visc     = block->visc;
	float diff     = block->diff;
	float *s       = block->s;
	float *density = block->density;
	float *vxCurr  = block->vxCurr;
	float *vyCurr  = block->vyCurr;
	float *vxPrev  = block->vxPrev;
	float *vyPrev  = block->vyPrev;

	Diffuse(1, vxPrev, vxCurr, visc);
	Diffuse(2, vyPrev, vyCurr, visc);

	Project(vxPrev, vyPrev, vxCurr, vyCurr);

	AddVection(1, vxCurr, vxPrev, vxPrev, vyPrev);
	AddVection(2, vyCurr, vyPrev, vxPrev, vyPrev);

	Project(vxCurr, vyCurr, vxPrev, vyPrev);

	Diffuse(0, s, density, diff);
	AddVection(0, density, s, vxCurr, vyCurr);
}
