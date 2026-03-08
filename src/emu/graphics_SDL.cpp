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

ModifiableTexture::ModifiableTexture(Window* parent, int width, int height) {
	win = parent;
	tex = (void*)SDL_CreateTexture((SDL_Renderer*)win->rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	nullcheck(tex);
	SDL_SetTextureBlendMode((SDL_Texture*)tex, SDL_BLENDMODE_NONE);
	SDL_SetTextureScaleMode((SDL_Texture*)tex, SDL_SCALEMODE_NEAREST);

	w = width;
	h = height;
}

ModifiableTexture::~ModifiableTexture() {
	SDL_DestroyTexture((SDL_Texture*)tex);
}

Uint8* ModifiableTexture::lockRegion(TextureRegion reg, int* pitch_OUT) {
	Uint8* arr;
	SDL_Rect r = {reg.x, reg.y, reg.w, reg.h};
	int pitch;
	safe(SDL_LockTexture((SDL_Texture*)tex, &r, (void**)&arr, &pitch));
	if (pitch_OUT != nullptr) {
		*pitch_OUT = pitch;
	}
	return arr;
}

void ModifiableTexture::unlock() {
	SDL_UnlockTexture((SDL_Texture*)tex);
}

void ModifiableTexture::modify(TextureRegion reg, std::function<Uint8(int x, int y)> r, std::function<Uint8(int x, int y)> g, std::function<Uint8(int x, int y)> b) {
	int pitch = 0;
	auto pix = lockRegion(reg, &pitch);

	for (int y = 0; y < reg.h; y++) {
		for (int x = 0; x < reg.w; x++) {
			pix[y * pitch + x * 4] = 255;
			pix[y * pitch + x * 4 + 1] = b(x, y);
			pix[y * pitch + x * 4 + 2] = g(x, y);
			pix[y * pitch + x * 4 + 3] = r(x, y);
		}
	}

	unlock();
}