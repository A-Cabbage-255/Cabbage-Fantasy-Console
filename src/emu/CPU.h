#pragma once
#include "memory.h"
#include "../common.h"

constexpr unsigned INT_QUIT = 0x00;
constexpr unsigned INT_LOWER_USER_BOUND = 0xE0;
constexpr unsigned INT_UPPER_USER_BOUND = 0xE1;
constexpr unsigned INT_PAUSE_TO_RENDER = 0xFE;
constexpr unsigned INT_USER_ILL_ATT = 0xFF;

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

	void execALU(Uint16 ins);
	void execJump(Uint16 ins);
	void execIMM(Uint16 ins);
	void execRAM(Uint16 ins);

	void execIns(Uint16 ins);
	void tick();
};