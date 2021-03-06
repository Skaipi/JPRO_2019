#define _CRT_SECURE_NO_DEPRECATE // For visual studio compilation

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "fluid_block.h"
#include "fluid_graphics.h"
#include "fluid_output.h"
#include "fluid_modes.h"

#undef main

/* Those are indended global variables */
const unsigned int WindowSize = 160;
const unsigned int Scale = 4;
const unsigned int Iter = 8;
const double dt = 0.02;

// Sign function
int sgn(int value)
{
	if (value > 0) return 1;
	if (value < 0) return -1;
	return 0;
}

void ManageFileInput(FILE *input, float *diffusion, float *viscosity, float *density, float *force, int *mode, float *time)
{
	int i = 0;
	char line[128];
    while ( fgets( line, sizeof line, input ) != NULL ) // read a line
    {
		// Check if file is not too large
		if (i > 258) {
			fprintf(stderr, "Wrong file type provided");
			exit(1);
		}
		/* Check if line starts with pattern and get float value */
		if (strncmp(line, "Diffusion:", strlen("Diffusion:")) == 0) {
			// This regexp reads 2chars after ':'
			sscanf(line, "%*[^:]%*2c%f", diffusion);
		}
		if (strncmp(line, "Viscosity:", strlen("Viscosity:")) == 0) {
			sscanf(line, "%*[^:]%*2c%f", viscosity);
		}
		if (strncmp(line, "Density:", strlen("Density:")) == 0) {
			sscanf(line, "%*[^:]%*2c%f", density);
		}
		if (strncmp(line, "Force:", strlen("Force:")) == 0) {
			sscanf(line, "%*[^:]%*2c%f", force);
		}
		if (strncmp(line, "Mode:", strlen("Mode:")) == 0) {
			sscanf(line, "%*[^:]%*2c%d", mode);
		}
		if (strncmp(line, "Time:", strlen("Time:")) == 0) {
			sscanf(line, "%*[^:]%*2c%f", time);
		}

		i++;
    }
}

int main(int argc, char* args[]) {
	// Try to open input file
	FILE* input_file = NULL;
	if (argc > 1) {
		input_file = fopen(args[1], "r");
		if (input_file == NULL) {
			fprintf(stderr, "Nie udalo sie otworzyc pliku wejsciowego\n");
		}
		else {
			printf("file opened with succes\n");
		}
	}

	// Initialize simulation
	srand(time(NULL));
	float time = 0.0, simulation_time=8.0;
	float diffusion = 0;
	float viscosity = 0;
	float density = 32;
	float force = 0.5;
	int mode = 0;
	enum modes {
		DEFAULT,    // 0
		SINUS,      // 1 
		SPIRAL      // 2
	};

	// Handle input file
	if (input_file != NULL) ManageFileInput(input_file, &diffusion, &viscosity, &density, &force, &mode, &simulation_time);
	FluidBlock* myFluid = FluidBlockCreate(diffusion, viscosity);

	// SDL init
	SDL_Window *win = WindowInit();
	SDL_Renderer *renderer = RendererInit(win);
		
	// Initialize mouse events
	int mouseXCurr = 0, mouseXPrev = 0;
	int mouseYCurr = 0, mouseYPrev = 0;
	SDL_PumpEvents();

	// main loop
	for (;;) {
        // Exit
		if (PollEventsForQuit() || time > simulation_time) break;
		// Check simulation mode
		switch (mode) {
		case SINUS:
			SinusMode(myFluid, time, density, force);
			break;
		case SPIRAL:
			SpiralMode(myFluid, time, density, force);
			break;
		default:
			// Mouse input
			if (SDL_GetMouseState(&mouseXCurr, &mouseYCurr) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
				FluidBlockSpawnSource(myFluid, mouseXCurr/Scale, mouseYCurr/Scale, 4, 4,
                                      sgn(mouseXCurr-mouseXPrev), sgn(mouseYCurr-mouseYPrev), density, force);
				mouseXPrev = mouseXCurr;
				mouseYPrev = mouseYCurr;
			}
			break;
		}

        // Update
		FluidBlockSimulationStep(myFluid);
		// Draw
		Draw(renderer, myFluid);
		time += dt;
    }

	// Exit program area
	SaveSimulation(myFluid);
	if (input_file != NULL) fclose(input_file);
	FluidBlockFree(myFluid);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
