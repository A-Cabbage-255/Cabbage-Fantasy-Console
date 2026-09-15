#pragma once
#include "../common.h"

class PIC {
private:
	std::function<void(Uint8)> interrupt;
public:
	PIC(std::function<void(Uint8)> interruptCPU);
};