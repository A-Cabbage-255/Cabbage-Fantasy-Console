#pragma once
#include "../common.h"
#include "interrupts.h"

/*
Line ID 0 is used for CPU acknowledge, CPU pulls high while executing an interrupt
the rest are regular external lines
*/

class InterruptController {
public:
	static constexpr size_t numLines = 16;
private:
	std::function<void(IntID)> interrupt;

	bool* intLines[numLines] = {nullptr};

	bool checkLine(int lineid);
public:
	InterruptController(std::function<void(IntID)> interruptCPU);

	void connect(int lineid, bool* line);

	void tick();
};