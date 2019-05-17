#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "fluid_block.h"
#include "fluid_graphics.h"

extern unsigned int WindowSize;
extern unsigned int Scale;

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
	for (unsigned int j=0; j < N*Scale; j += Scale) {
		for (unsigned int i=0; i < N*Scale; i += Scale) {
			SDL_Rect rect;
			rect.x = i;
			rect.y = j;
			rect.w = Scale;
			rect.h = Scale;
			
			float d = block->density[i/Scale + j*N/Scale];
			SDL_SetRenderDrawColor(renderer, 0, d, d, d);
			SDL_RenderFillRect(renderer, &rect);
			//SDL_RenderDrawPoint(renderer, i, j);
		}
	}
}
