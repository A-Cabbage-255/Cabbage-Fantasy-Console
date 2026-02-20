#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <string>

using namespace std::literals::string_literals;

inline
void CONS_Log(std::string s) {
	std::cout << s;
}