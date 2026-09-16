#pragma once
#include "memory.h"
#include "../common.h"
#include "interrupts.h"

class CPU {
private:
	Memory* m = nullptr;

	std::function<void(void)> pauseToRender;
public:
	bool finished = false;

	Uint32 instPntr = 0;

	int timer = 0;

	Uint16* registers;
	bool carryFlag;

	bool kernelMode = true;

	CPU(Memory* mem, std::function<void(void)> drawFrame);
	~CPU();

	void interrupt(Uint8 id);
	void interrupt(IntID id) {interrupt(std::to_underlying(id));}

	void execALU(Uint16 ins);
	void execJump(Uint16 ins);
	void execIMM(Uint16 ins);
	void execRAM(Uint16 ins);

	void execIns(Uint16 ins);
	void tick();
};