#pragma once
#include "lexer.h"

class Compiler {
private:
	SDL_IOStream* file;

	void outIns_ALU(ALUInstruction* i);
public:
	Compiler(std::string outpath);
	~Compiler();

	void outIns(BasicInstruction* i);
};