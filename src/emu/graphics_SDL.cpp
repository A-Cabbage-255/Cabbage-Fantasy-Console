#include "graphics.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#define PREPROC_STR(x) #x
#define XPREPROC_STR(x) PREPROC_STR(x)

#define safe(x) {bool success = x; if (!success) {SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR Line %s\n\tFailed to call %s\n\t%s\n\n", XPREPROC_STR(__LINE__), #x, SDL_GetError()); assert(false);} }
#define nullcheck(x) {if (x == NULL) {SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR Line %s\n\tExpected pointer %s was returned as NULL\n\t%s\n\n", XPREPROC_STR(__LINE__), #x, SDL_GetError()); assert(false);} }

unsigned Window::numWindows = 0;

Window::Window(std::string title, int w, int h) {
	if (numWindows == 0) {
		safe(SDL_InitSubSystem(SDL_INIT_VIDEO));
	}
	numWindows++;

	win = (void*)SDL_CreateWindow(title.c_str(), w, h, SDL_WINDOW_HIGH_PIXEL_DENSITY);
	nullcheck(win);

	rend = (void*)SDL_CreateRenderer((SDL_Window*)win, NULL);
	nullcheck(rend);
}
Window::Window(std::string title, int w, int h, int logicalw, int logicalh) {
	if (numWindows == 0) {
		safe(SDL_InitSubSystem(SDL_INIT_VIDEO));
	}
	numWindows++;

	win = (void*)SDL_CreateWindow(title.c_str(), w, h, SDL_WINDOW_HIGH_PIXEL_DENSITY);
	nullcheck(win);

	rend = (void*)SDL_CreateRenderer((SDL_Window*)win, NULL);
	nullcheck(rend);

	safe(SDL_SetRenderLogicalPresentation((SDL_Renderer*)rend, logicalw, logicalh, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE));
}

Window::~Window() {
	SDL_DestroyRenderer((SDL_Renderer*)rend);
	SDL_DestroyWindow((SDL_Window*)win);

	numWindows--;
	if (numWindows == 0) {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
}

void Window::setIcon(std::string p) {
	SDL_Surface* i = IMG_Load(p.c_str());
	nullcheck(i);

	safe(SDL_SetWindowIcon((SDL_Window*)win, i));
}

bool Window::tick() {
	safe(SDL_RenderPresent((SDL_Renderer*)rend));

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_EVENT_QUIT:
			return false;
		}
	}
	return true;
}

void Window::clear(Uint8 r, Uint8 g, Uint8 b) {
	safe(SDL_SetRenderDrawColor((SDL_Renderer*)rend, r, g, b, 255));
	safe(SDL_RenderClear((SDL_Renderer*)rend));
}

void Texture::draw(float x, float y) {
	SDL_FRect r = {x, y, (float)w, (float)h};
	safe(SDL_RenderTexture((SDL_Renderer*)win->rend, (SDL_Texture*)tex, NULL, &r));
}
void Texture::draw(FloatRect src, FloatRect dest) {
	SDL_FRect sourceR = {src.x, src.y, src.w, src.h};
	SDL_FRect destR = {dest.x, dest.y, dest.w, dest.h};
	safe(SDL_RenderTexture((SDL_Renderer*)win->rend, (SDL_Texture*)tex, &sourceR, &destR));
}

ModifiablePalettedTexture::ModifiablePalettedTexture(Window* parent, int width, int height) {
	win = parent;
	tex = (void*)SDL_CreateTexture((SDL_Renderer*)win->rend, SDL_PIXELFORMAT_INDEX8, SDL_TEXTUREACCESS_STREAMING, width, height);
	nullcheck(tex);
	safe(SDL_SetTextureBlendMode((SDL_Texture*)tex, SDL_BLENDMODE_NONE));
	safe(SDL_SetTextureScaleMode((SDL_Texture*)tex, SDL_SCALEMODE_NEAREST));

	palette = (void*)SDL_CreatePalette(256);

	safe(SDL_SetTexturePalette((SDL_Texture*)tex, (SDL_Palette*)palette));

	w = width;
	h = height;
}

ModifiablePalettedTexture::~ModifiablePalettedTexture() {
	SDL_DestroyPalette((SDL_Palette*)palette);
	SDL_DestroyTexture((SDL_Texture*)tex);
}

Uint8* ModifiablePalettedTexture::lockRegion(TextureRegion reg, int* pitch_OUT) {
	Uint8* arr;
	SDL_Rect r = {reg.x, reg.y, reg.w, reg.h};
	int pitch;
	safe(SDL_LockTexture((SDL_Texture*)tex, &r, (void**)&arr, &pitch));
	if (pitch_OUT != nullptr) {
		*pitch_OUT = pitch;
	}
	return arr;
}

void ModifiablePalettedTexture::unlock() {
	SDL_UnlockTexture((SDL_Texture*)tex);
}

void ModifiablePalettedTexture::modify(TextureRegion reg, std::function<Uint8(int x, int y)> col) {
	int pitch = 0;
	auto pix = lockRegion(reg, &pitch);

	for (int y = 0; y < reg.h; y++) {
		for (int x = 0; x < reg.w; x++) {
			pix[y * pitch + x] = col(x, y);
		}
	}

	unlock();
}

void ModifiablePalettedTexture::changePaletteColor(Uint8 idx, Uint8 r, Uint8 g, Uint8 b) {
	SDL_Color c = {r, g, b, 255};
	safe(SDL_SetPaletteColors((SDL_Palette*)palette, &c, idx, 1));
}