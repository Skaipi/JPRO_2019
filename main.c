#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "fluid_block.h"
#include "fluid_graphics.h"

/* Those are indended global variables */
const unsigned int WindowSize = 320;
const unsigned int Iter = 4;
const double dt = 0.02;

// Handle exit event
int pollEventsForQuit() {
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
	if (argc > 0) {
		input_file = fopen(args[0], "r");
		if (input_file == NULL) {
			fprintf(stderr, "Nie udalo sie otworzyc pliku %s", args[0]);
		}
	}

	// Initialize simulation
	float diffiusion = 0;
	float viscosity = 0;
	FluidBlock* myFluid = FluidBlockCreate(diffiusion, viscosity);

	// Chcek SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
		fprintf(stderr, "SDL failed");
		return 3;
	}
	SDL_Window *win = SDL_CreateWindow("Base Code", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,WindowSize,WindowSize, SDL_WINDOW_SHOWN);
	if (win == NULL) {
		fprintf(stderr, "SDL failed");
		SDL_Quit();
		return 4;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		puts("SDL failed");
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	// main loop
	for (;;) {
		if (pollEventsForQuit()) break;
		// keybord interuption
		//const Uint8 *state = SDL_GetKeyboardState(NULL);
		//if (state[SDL_SCANCODE_SPACE]) {
			FluidBlockAddDensity(myFluid, WindowSize/2, WindowSize/2, 255);
			FluidBlockAddVelocity(myFluid, WindowSize/2, WindowSize/2, 15, 50);
			
			FluidBlockAddDensity(myFluid, WindowSize/2-1, WindowSize/2, 255);
			FluidBlockAddVelocity(myFluid, WindowSize/2-1, WindowSize/2, 15, 50);
			FluidBlockAddDensity(myFluid, WindowSize/2+1, WindowSize/2, 255);
			FluidBlockAddVelocity(myFluid, WindowSize/2+1, WindowSize/2, 15, 50);
			FluidBlockAddDensity(myFluid, WindowSize/2-1, WindowSize/2-1, 255);
			FluidBlockAddVelocity(myFluid, WindowSize/2-1, WindowSize/2-1, 15, 50);
			FluidBlockAddDensity(myFluid, WindowSize/2+1, WindowSize/2-1, 255);
			FluidBlockAddVelocity(myFluid, WindowSize/2+1, WindowSize/2-1, 15, 50);
			FluidBlockAddDensity(myFluid, WindowSize/2-1, WindowSize/2+1, 255);
			FluidBlockAddVelocity(myFluid, WindowSize/2-1, WindowSize/2+1, 15, 50);
			FluidBlockAddDensity(myFluid, WindowSize/2+1, WindowSize/2+1, 255);
			FluidBlockAddVelocity(myFluid, WindowSize/2+1, WindowSize/2+1, 15, 50);
			FluidBlockAddDensity(myFluid, WindowSize/2, WindowSize/2-1, 255);
			FluidBlockAddVelocity(myFluid, WindowSize/2, WindowSize/2-1, 15, 50);
			FluidBlockAddDensity(myFluid, WindowSize/2, WindowSize/2+1, 255);
			FluidBlockAddVelocity(myFluid, WindowSize/2, WindowSize/2+1, 15, 50);
		//}
		FluidBlockSimulationStep(myFluid);
		// drawing
		Draw(renderer, myFluid);
    }

	// Exit program area
	if (input_file != NULL) fclose(input_file);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
