#pragma once
#include "memory.h"

class CPU {
private:
	Memory* m = nullptr;
public:
	uint32_t instPntr = 0;

	uint8_t* registers;
	bool carryFlag;

	CPU(Memory* mem);
	~CPU();

	void execALU(uint16_t ins);

	void execIns(uint16_t ins);
	void tick();
};