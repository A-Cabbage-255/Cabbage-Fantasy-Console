#include "screen.h"
#include <SDL3/SDL.h>

typedef struct VideoCTX {
	SDL_Window* win;
	SDL_Renderer* rend;
} VideoCTX;

Screen::Screen(std::function<Uint8(Uint32)> spriteMem, std::function<Uint8(Uint32)> spriteFlags, std::function<Uint8(Uint32)> spriteData, std::function<Uint8(Uint32)> palette)
 : getSpriteData(spriteMem), getOAM1(spriteFlags), getOAM2(spriteData), getPalette(palette) {
	context = new VideoCTX;
	context->win = SDL_CreateWindow("", 1600, 900, SDL_WINDOW_HIGH_PIXEL_DENSITY);
	context->rend = SDL_CreateRenderer(context->win, NULL);
	SDL_SetRenderLogicalPresentation(context->rend, 800, 450, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
}

Screen::~Screen() {
	SDL_DestroyRenderer(context->rend);
	SDL_DestroyWindow(context->win);
	delete context;
}