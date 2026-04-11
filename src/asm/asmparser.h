#pragma once
#include "../common.h"
#include "SDL3/SDL.h"

class Assembler {
private:
	int pass = 0;

	SDL_IOStream* file = nullptr;

	std::string readLine();
public:
	bool eof = false;

	Assembler(std::string filePath);
	~Assembler();

	void parseLine();
};