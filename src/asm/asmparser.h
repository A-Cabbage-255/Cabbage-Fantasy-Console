#pragma once
#include "../common.h"
#include "SDL3/SDL.h"
#include <map>

//TODO COMPLETELY REFACTOR WITH TOKEN PARSING ETC.
class Assembler {
private:
	SDL_IOStream* reading = nullptr;

	std::string readLine();
public:
	bool eof = false;
	int pass = 0;
	unsigned line = 0;
	
	std::map<std::string, Uint32> pointers;

	Assembler(std::string filePath);
	~Assembler();

	void parseLine();
};