#pragma once

/* Main drawing function */
void Draw(SDL_Renderer* renderer, FluidBlock* block);

/* Draw fluid (per pixel) */
void DrawDensity(SDL_Renderer* framebuffer, FluidBlock* block);
