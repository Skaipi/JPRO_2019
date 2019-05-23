#define _CRT_SECURE_NO_DEPRECATE // For visual studio compilation

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "fluid_block.h"
#include "fluid_graphics.h"
#include "output.h"

#undef main

/* Those are indended global variables */
const unsigned int WindowSize = 160;
const unsigned int Scale = 4;
const unsigned int Iter = 4;
const double dt = 0.02;

int sgn(int value)
{
	if (value > 0) return 1;
	if (value < 0) return -1;
	return 0;
}

void ManageFileInput(FILE *input, float *diffiusion, float *viscosity, float *density, float *force)
{
	fscanf(input, "%f", diffiusion);
	fscanf(input, "%f", viscosity);
	fscanf(input, "%f", density);
	fscanf(input, "%f", force);
}

// Handle exit event
int PollEventsForQuit() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return true;
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_ESCAPE) {
				return true;
			}
			break;
		}
	}
	return 0;
}

int main(int argc, char* args[]) {
	// Input handle area
	FILE* input_file;
	// TODO: check why argc > 1 crashes program
	if (argc > 0) {
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
	float time = 0.0, simulation_time=5.0;
	float diffiusion = 0;
	float viscosity = 0;
	float density = 32;
	float force = 1;
	if (input_file) ManageFileInput(input_file, &diffiusion, &viscosity, &density, &force);
	FluidBlock* myFluid = FluidBlockCreate(diffiusion, viscosity);

	// Chcek SDL init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
		fprintf(stderr, "SDL failed");
		return 1;
	}
	SDL_Window *win = SDL_CreateWindow("Fluid Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       WindowSize*Scale, WindowSize*Scale, SDL_WINDOW_SHOWN);
	if (win == NULL) {
		fprintf(stderr, "SDL failed");
		SDL_Quit();
		return 2;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		fprintf(stderr, "SDL failed");
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 3;
	}

	// Initialize mouse events
	int mouseXCurr = 0, mouseXPrev = 0; 
	int mouseYCurr = 0, mouseYPrev = 0;
	SDL_PumpEvents();

	// main loop
	for (;;) {
		if (PollEventsForQuit() || time > simulation_time) break;
		// Keybord input
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_SPACE]) {
		}
		// Mouse input
		if (SDL_GetMouseState(&mouseXCurr, &mouseYCurr) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			FluidBlockSpawnSource(myFluid, mouseXCurr/Scale, mouseYCurr/Scale, 4, 4,
                                  sgn(mouseXCurr-mouseXPrev), sgn(mouseYCurr-mouseYPrev), density, force);
			mouseXPrev = mouseXCurr;
			mouseYPrev = mouseYCurr;
		}

		FluidBlockSimulationStep(myFluid);
		// drawing
		Draw(renderer, myFluid);
		time += dt;
    }

	// Exit program area
	SaveSimulation(myFluid);
	if (input_file != NULL) fclose(input_file);
	FluidBlockFree(myFluid);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
