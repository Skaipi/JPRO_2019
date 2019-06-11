#pragma once

/* Initialize window */
SDL_Window* WindowInit();

/* Initialize window renderer */
SDL_Renderer* RendererInit(SDL_Window *win);

/* Handle exit event */
int PollEventsForQuit();

/* Main drawing function */
void Draw(SDL_Renderer* renderer, FluidBlock* block);

/* Draw fluid (per pixel) */
void DrawDensity(SDL_Renderer* framebuffer, FluidBlock* block);
