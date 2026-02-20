#include <iostream>
#include <SDL3/SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("title", 1600, 900, SDL_WINDOW_HIGH_PIXEL_DENSITY);
    SDL_Renderer* rend = SDL_CreateRenderer(win, NULL);
    
    SDL_Event e;
    bool running = true;
    while (running){
        while (SDL_PollEvent(&e)){
            switch (e.type){
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
            }
        }
        SDL_SetRenderDrawColor(rend, 50, 255, 90, 255);
        SDL_RenderClear(rend);

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderLine(rend, 0, 0, 1600, 450);

        SDL_RenderPresent(rend);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}