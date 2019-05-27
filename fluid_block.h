#pragma once

/* This structure represents block of liquid */
struct FluidBlock {
    int size;
    float dt;
    float diff;
    float visc;
    
    float *s;
    float *density;
    
    float *vxCurr;
    float *vyCurr;

    float *vxPrev;
    float *vyPrev;
};
typedef struct FluidBlock FluidBlock;

/* Free allocated memory */
void FluidBlockFree(FluidBlock *block);

/* Works like constructor */
FluidBlock* FluidBlockCreate(int diffusion, int viscosity);

void FluidBlockAddDensity(FluidBlock *block, int x, int y, float amount);

void FluidBlockAddVelocity(FluidBlock *block, int x, int y, float amountX, float amountY);

/* Make 1 simulation step with block */
void FluidBlockSimulationStep(FluidBlock *block);

/* Make turbulence box */
void FluidBlockSpawnSource(FluidBlock* block, int x, int y, int w, int h, int dirX, int dirY, float density, float force);
