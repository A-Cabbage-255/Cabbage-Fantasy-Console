#pragma once
#include "tokenizer.h"

class Compiler {
private:
	Tokenizer* t;

	SDL_IOStream* file;
public:
	Compiler(std::string outpath, Tokenizer* tok);
	~Compiler();
};