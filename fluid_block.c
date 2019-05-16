#include <stdlib.h>
#include "fluid_block.h"
#include "fluid_simulation.h"

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

FluidBlock* FluidBlockCreate(int size, int diffusion, int viscosity, float dt)
{
    FluidBlock *block = malloc(sizeof(FluidBlock));
    int N = size;
    
    block->size = size;
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
	int N = block->size;
    block->density[x + y*N] += amount;
}

void FluidBlockAddVelocity(FluidBlock *block, int x, int y, float amountX, float amountY)
{
    int N = block->size;
    int index = x + y*N;
    
    block->vxCurr[index] += amountX;
    block->vyCurr[index] += amountY;
}

void FluidBlockSimulationStep(FluidBlock *block)
{
	//TODO: Check if possible without deep copy
	int N          = block->size;
	float visc     = block->visc;
	float diff     = block->diff;
	float dt       = block->dt;
	float *s       = block->s;
	float *density = block->density;
	float *vxCurr  = block->vxCurr;
	float *vyCurr  = block->vyCurr;
	float *vxPrev  = block->vxPrev;
	float *vyPrev  = block->vyPrev;

	Diffuse(1, vxPrev, vxCurr, visc, dt, 4, N);
	Diffuse(2, vyPrev, vyCurr, visc, dt, 4, N);

	Project(vxPrev, vyPrev, vxCurr, vyCurr, 4, N);

	AddVection(1, vxCurr, vxPrev, vxPrev, vyPrev, dt, N);
	AddVection(2, vyCurr, vyPrev, vxPrev, vyPrev, dt, N);

	Project(vxCurr, vyCurr, vxPrev, vyPrev, 4, N);

	Diffuse(0, s, density, diff, dt, 4, N);
	AddVection(0, density, s, vxCurr, vyCurr, dt, N);
}
