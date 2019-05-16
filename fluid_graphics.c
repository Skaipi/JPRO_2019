#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "fluid_block.h"
#include "fluid_graphics.h"

extern unsigned int WindowSize;

#define N WindowSize

void Draw(SDL_Renderer* renderer, FluidBlock* block)
{
	int drawResult = SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 255);
	if (drawResult != 0) {
		fprintf(stderr, "SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_RenderClear(renderer);
	DrawDensity(renderer, block);
	SDL_RenderPresent(renderer); // Update screen.
}

void DrawDensity(SDL_Renderer* renderer, FluidBlock* block)
{
	for (unsigned int j=0; j < N; j++) {
		for (unsigned int i=0; i< N; i++) {
			float d = block->density[i + j*N];
			SDL_SetRenderDrawColor(renderer, 0, d, d, d);
			SDL_RenderDrawPoint(renderer, i, j);
		}
	}
}
