#include <math.h>
#include "fluid_block.h"
#include "fluid_modes.h"

extern const unsigned int WindowSize;

#define N WindowSize

void SinusMode(FluidBlock* block, float time, float density, float force)
{
	float A = 16;
	float xPos = A * time;
	float yPos = N/2 + A*sin(time);
	float xDir = sin(time);
	float yDir = cos(time);
	FluidBlockSpawnSource(block, xPos, yPos, 4, 4, xDir, yDir, density, force);
}

void SpiralMode(FluidBlock* block, float time, float density, float force)
{
	float xPos = N/2;
	float yPos = N/2;
	float xDir = sin(time);
	float yDir = cos(time);
	FluidBlockSpawnSource(block, xPos, yPos, 4, 4, xDir, yDir, density, force);
}
