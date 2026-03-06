#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <functional>
#include <string>

using namespace std::literals::string_literals;

inline
void CONS_Log(std::string s) {
	std::cout << s;
}

inline
void CONS_LogPercent(long v, long max) {
	double fraction = (double)v / (double)max;
	std::string line = "\r[";
	for (unsigned i = 0; i < 75; i++) {
		if (i < fraction * 75) {
			line += "@";
		} else {
			line += "-";
		}
	}
	line += "]";
	std::cout << line << std::flush;
}

inline
void CONS_FinishPercent() {
	std::cout << std::endl;
}