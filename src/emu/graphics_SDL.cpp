#include "graphics.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#define PREPROC_STR(x) #x
#define XPREPROC_STR(x) PREPROC_STR(x)

#define safe(x) {bool success = x; if (!success) {SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR Line %s\n\tFailed to call %s\n\t%s\n\n", XPREPROC_STR(__LINE__), #x, SDL_GetError()); assert(false);} }

unsigned Window::numWindows = 0;

Window::Window(std::string title, int w, int h) {
	if (numWindows == 0) {
		safe(SDL_InitSubSystem(SDL_INIT_VIDEO));
	}
	numWindows++;
}