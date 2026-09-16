#include "PIC.h"

InterruptController::InterruptController(std::function<void(IntID)> interruptCPU) {
	interrupt = interruptCPU;
}

bool InterruptController::checkLine(int lineid) {
	assert(lineid >= 0 && lineid <= numLines);
	if (intLines[lineid] == nullptr) return false;
	return *(intLines[lineid]);
}

void InterruptController::connect(int lineid, bool* line) {
	assert(lineid >= 0 && lineid <= numLines);
	intLines[lineid] = line;
}

void InterruptController::tick() {
	if (checkLine(0)) {
		return;
	}
	for (unsigned i = 1; i < numLines; i++) {
		if (checkLine(i)) {
			interrupt(IntID::Input_Keyboard);
			break;
		}
	}
}