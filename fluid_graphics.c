#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "fluid_block.h"
#include "fluid_graphics.h"

extern unsigned int WindowSize;
extern unsigned int Scale;

#define N WindowSize

SDL_Window* WindowInit()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
		fprintf(stderr, "SDL failed");
		exit(1);
	}


	SDL_Window *win = SDL_CreateWindow("Fluid Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       WindowSize*Scale, WindowSize*Scale, SDL_WINDOW_SHOWN);
	if (win == NULL) {
		fprintf(stderr, "WindowInit() failed");
		SDL_Quit();
		exit(1);
	}
	return win;
}

SDL_Renderer* RendererInit(SDL_Window *win)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		fprintf(stderr, "RendererInit() failed");
		SDL_DestroyWindow(win);
		SDL_Quit();
		exit(1);
	}
	return renderer;
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
			
			int d = (int)block->density[i/Scale + j*N/Scale];

			SDL_SetRenderDrawColor(renderer, 0, d, d, d);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}
