#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fluid_block.h"
#include "fluid_output.h"

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

float GetHighestVelocity(FluidBlock* block)
{
	float tmp = sqrt(fabs(block->vxCurr[0]) + fabs(block->vyCurr[0]));
	for (unsigned int j=0; j < N; j++) {
		for (unsigned int i=0; i < N; i++) {
			if (tmp < sqrt(fabs(block->vxCurr[i + j*N]) + fabs(block->vyCurr[i + j*N]))) {
				tmp = sqrt(fabs(block->vxCurr[i + j*N]) + fabs(block->vyCurr[i + j*N]));
			}			
		}
	}
	return tmp;
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

float GetHighestDensity(FluidBlock* block)
{
	float tmp = block->density[0];
	for (unsigned int j=0; j < N; j++) {
		for (unsigned int i=0; i < N; i++) {
			if (tmp < block->density[i + j*N]) {
				tmp = block->density[i + j*N];
			}
		}
	}
	return tmp;
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
	fprintf(file, "Highest velocity:  %9.5f\n", GetHighestVelocity(block));
	fprintf(file, "Average density:   %9.5f\n", GetAverageDensity(block));
	fprintf(file, "Highest density:   %9.5f\n", GetHighestDensity(block));
	fprintf(file, "%% of space filled: %9.5f\n", GetFilledSpace(block));
	fprintf(file, "=============================\n");
}

void SaveSimulation(FluidBlock* block)
{
	FILE* output_file = fopen("output.txt", "w");
	if (output_file == NULL) {
		fprintf(stderr, "output file failed\n");
		return;
	}

	SaveCurrentState(output_file, block);
	fclose(output_file);
}
