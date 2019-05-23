#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fluid_block.h"

extern const unsigned int WindowSize;

#define N WindowSize

float GetAverageVelocity(FluidBlock* block)
{
	float tmp = 0.0;
	for	(unsigned int j=0; j < N; j++) {
		for (unsigned int i=0; i < N; i++) {
			tmp += sqrt(fabs(block->vxCurr[i + j*N]) + fabs(block->vyCurr[i + j*N]));
		}
	} return tmp / (N*N);
}

float GetAverageDensity(FluidBlock* block)
{
	float tmp = 0.0;
	for	(unsigned int j=0; j < N; j++) {
		for (unsigned int i=0; i < N; i++) {
			tmp += block->density[i + j*N];
		}
	}
	return tmp / (N*N);
}

float GetFilledSpace(FluidBlock* block)
{
	float tmp = 0.0;
	for	(unsigned int j=0; j < N; j++) {
		for (unsigned int i=0; i < N; i++) {
			if (block->density[i + j*N] > 1) {
				tmp += 1;
			}
		}
	}
	return tmp*100 / (N*N);
}

void SaveCurrentState(FILE* file, FluidBlock* block)
{
	fprintf(file, "======SIMULATION_OUTPUT======\n");
	fprintf(file, "Average velocity:  %9.5f\n", GetAverageVelocity(block));
	fprintf(file, "Average density:   %9.5f\n", GetAverageDensity(block));
	fprintf(file, "%% of space filled: %9.5f\n", GetFilledSpace(block));
	fprintf(file, "=============================\n");
}

void SaveSimulation(FluidBlock* block)
{
	FILE* output_file = fopen("output.txt", "w");
	if (output_file == NULL) {
		fprintf(stderr, "output file failed\n");
	}

	SaveCurrentState(output_file, block);
	fclose(output_file);
}
