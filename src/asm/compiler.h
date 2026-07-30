#pragma once
#include "lexer.h"

class Compiler {
private:
	SDL_IOStream* file;

	void outIns_ALU(ALUInstruction* i);
	void outIns_JMP(JMPInstruction* i);
	void outIns_LJMP(L_JMPInstruction* i);
	void outIns_INT(INTInstruction* i);
	void outIns_IMM(IMMInstruction* i);
	void outIns_RAM(RAMInstruction* i);
	void outIns_STACK(RAMInstruction* i);

	size_t curRegionStart = 0;
	size_t outputtedLoc = 0;
public:
	Compiler(std::string outpath);
	~Compiler();

	void outIns(BasicInstruction* i);

	void beginRegion(Uint32 startPos);

	unsigned getPos();
	unsigned getFilePos();
};