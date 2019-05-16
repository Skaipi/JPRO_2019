#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

static const unsigned int WindowSize = 640;

void filledRect(void *r_in,
                int x, int y, int w, int h,
                int r, int g, int b, int a)
{
	SDL_Renderer* renderer = (SDL_Renderer*)r_in;
	int fillColorResult = SDL_SetRenderDrawColor(renderer, r, g, b, a);
	if (fillColorResult != 0) {
		fprintf(stderr, "SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Rect rect = { x, y, w, h };
	int renderResult = SDL_RenderFillRect(renderer, &rect);
	if (renderResult != 0) {
	fprintf(stderr, "SDL_RenderFillRect failed: %s\n", SDL_GetError());
	exit(1);
	}
}

SDL_Renderer* init(int w, int h) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Cannot initialise SDL: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Window *window = SDL_CreateWindow(
		"hello",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w, h,
		SDL_WINDOW_SHOWN
	);
	if (window == NULL) {
		fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (renderer == NULL) {
		fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0) {
		fprintf(stderr, "SDL_BlendMode failed: %s\n", SDL_GetError());
		exit(1);
	};

	return renderer;
}

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
	FILE* input_file;
	if (argc > 0) {
		input_file = fopen(args[0], "r");
		if (input_file == NULL) {
			fprintf(stderr, "Nie udalo sie otworzyc pliku %s", args[0]);
		}
	}

	int wndSize = 640;
	SDL_Renderer *renderer = init(WindowSize, WindowSize);

	for (;;) {
		if (pollEventsForQuit()) break;
    	int drawResult = SDL_SetRenderDrawColor(renderer, 16, 16, 16, 255);
		if (drawResult != 0) {
			fprintf(stderr, "SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
			exit(1);
		}
		SDL_RenderClear(renderer);
		filledRect(renderer, wndSize/2 - 50/2, wndSize/2 - 50/2, 50, 50, 255, 0, 0, 128);
		SDL_RenderPresent(renderer); // Update screen.
	}

	if (input_file != NULL) fclose(input_file);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
