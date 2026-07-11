#include "CPU.h"
#include <bitset>

CPU::CPU(Memory* mem, std::function<void(void)> drawFrame) : m(mem), pauseToRender(drawFrame) {
	registers = new Uint16[16];
	registers[0] = 0;
	instPntr = 0;
}

CPU::~CPU() {
	delete[] registers;
}

void CPU::interrupt(Uint8 id) {
	if (id == INT_QUIT) {
		finished = true;
		return;
	} else if (id == INT_PAUSE_TO_RENDER) {
		pauseToRender();
		return;
	}

	kernelMode = true;
	
	Uint32 destination = m->getInterrupt(id);

	instPntr = destination - 2;
}

void CPU::tick() {
	Uint16 inst = m->getter16()(instPntr);
	execIns(inst);

	if (!kernelMode && (instPntr >= m->getInterrupt(INT_UPPER_USER_BOUND) || instPntr < m->getInterrupt(INT_LOWER_USER_BOUND))) {
		interrupt(INT_USER_ILL_ATT);
	}

	if (timer > 0) {
		timer--;
		if (timer == 0) {
			interrupt(0x01);
		}
	}

	instPntr+=2;
}

void CPU::execIns(Uint16 i) {
	if (!(i >> 15)) {
		execALU(i);
	} else if (!((i >> 14) & 0b1)) {
		execJump(i);
	} else if (!((i >> 13) & 0b1)) {
		if (!((i >> 11) & 0b1)) {

		} else {
			interrupt(i & 0xFF);
		}
	} else {
		execIMM(i);
	}
}

void CPU::execALU(Uint16 i) {
	auto dest = (i >> 8) & 0b1111;
	auto a = (i >> 4) & 0b1111;
	auto b = i & 0b1111;

	switch (i >> 12) {
	case 0b000: //ADD
		registers[dest] = registers[a] + registers[b];
		carryFlag = (((unsigned)registers[a] & 0xFFFF) + ((unsigned)registers[b] & 0xFFFF)) > 65535;
		break;
	case 0b001: //ADD W CAR
		registers[dest] = registers[a] + registers[b] + (carryFlag ? 1 : 0);
		carryFlag = (((unsigned)registers[a] & 0xFFFF) + ((unsigned)registers[b] & 0xFFFF) + (carryFlag ? 1 : 0)) > 65535;
		break;
	case 0b010: //SUB W CAR
		registers[dest] = registers[a] - registers[b] - (carryFlag ? 1 : 0);
		carryFlag = (((int)registers[a] & 0xFFFF) - ((int)registers[b] & 0xFFFF) - (carryFlag ? 1 : 0)) < 0;
		break;
	case 0b011: //SUB
		registers[dest] = registers[a] - registers[b];
		carryFlag = (((int)registers[a] & 0xFFFF) - ((int)registers[b] & 0xFFFF)) < 0;
		break;
	case 0b100: //NAND
		registers[dest] = ~(registers[a] & registers[b]);
		break;
	case 0b101: { //MUL
		auto highDest = (dest >> 2) | 0b100;
		auto lowDest = (dest & 0b11) | 0b100;

		Uint32 res = ((Uint32)registers[a] & 0xFFFFu) * ((Uint32)registers[b] & 0xFFFFu);

		registers[highDest] = res >> 16;
		registers[lowDest] = res & 0xFFFF;
		break;
	}
	//TODO SHIFT CARRY BIT IMPLEMENTATION
	case 0b110: //SHL
		registers[dest] = registers[a] << registers[b];
		break;
	case 0b111: //SHR
		registers[dest] = registers[a] >> registers[b];
		break;
	default:
		std::cout << "ERR\n";
		//BAD. SHOULDN'T HAPPEN.
		break;
	}

	registers[0] = 0;
}

void CPU::execJump(Uint16 i) {
	if ((i >> 13) & 1) {
		Uint16 highWord = registers[(i >> 4) & 0xF];
		Uint16 lowWord = registers[i & 0xF];

		instPntr = ((Uint32)highWord << 16) | (Uint32)lowWord;
		instPntr -= 2;
	} else {
		Uint16 toCheck = registers[(i >> 4) & 0xF];
		int16 change = reinterpret_cast<int16&>(registers[i & 0xF]);

		bool jmp = false;

		switch ((i >> 8) & 0b111) {
		case 0x0:
			jmp = toCheck == 0;
			break;
		case 0x1:
			jmp = toCheck < 0b1000000000000000;
			break;
		case 0x2:
			jmp = toCheck >= 0b1000000000000000;
			break;
		case 0x3:
			jmp = toCheck == registers[1];
			break;
		case 0x4:
			jmp = carryFlag;
			break;
		}

		if ((i >> 11) & 1) jmp = !jmp;

		if (jmp) {
			instPntr += change;
			instPntr -= 2;
		}
	}
	if ((i >> 12) & 1) {
		if (!kernelMode) {
			interrupt(INT_USER_ILL_ATT);
		}
		kernelMode = false;
		timer = 200;
	}
}

void CPU::execIMM(Uint16 ins) {
	if ((ins >> 12) & 0x1) {
		auto dest = (ins >> 8) & 0xF;
		instPntr += 2;
		auto val = m->getter16()(instPntr);

		registers[dest] = val;
	} else {
		auto dest = (ins >> 8) & 0xF;
		auto val = ins & 0xFF;

		registers[dest] = val;
	}
}