#include <math.h>
#include "fluid_block.h"
#include "modes.h"

extern const unsigned int WindowSize;

#define N WindowSize

void SinusMode(FluidBlock* block, float time, float density, float force)
{
	float A = 16;
	float xPos = A * time;
	float yPos = N/2 + A*sin(time);
	float xDir = sin(time);
	float yDir = cos(time);
	FluidBlockSpawnSource(block, xPos, yPos, 6, 6, xDir, yDir, density, force);
}

void SpikeMode(FluidBlock* block, float time, float density, float force)
{
	//FluidBlockSpawnSource();
}
