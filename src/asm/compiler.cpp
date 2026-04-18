#include "compiler.h"

Compiler::Compiler(std::string path, Tokenizer* tok) {
	t = tok;
	file = SDL_IOFromFile(path.c_str(), "wb");
}

Compiler::~Compiler() {
	SDL_CloseIO(file);
}