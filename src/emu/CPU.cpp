#include "CPU.h"

CPU::CPU(Memory* mem) : m(mem) {
	registers = new uint8_t[16];
}

CPU::~CPU() {
	delete[] registers;
}

void CPU::tick() {
	uint16_t inst = m->getter16()(instPntr);
	execIns(inst);
	inst+=2;
}

void CPU::execIns(uint16_t i) {
	if (i >> 15) {
		execALU(i);
	}
}

//FYI: Algorithm for 16-bit multiplication
//AB * CD = (A*C << 16) + ((B*C + A*D) << 8) + B*D
void CPU::execALU(uint16_t i) {
	auto dest = (i >> 8) & 0b1111;
	auto a = (i >> 4) & 0b1111;
	auto b = i & 0b1111;

	switch (i >> 12) {
	case 0b000: //ADD
		registers[dest] = registers[a] + registers[b];
		carryFlag = (((unsigned)registers[a] & 0xFF) + ((unsigned)registers[b] & 0xFF)) > 255;
		break;
	case 0b001: //ADD W CAR
		registers[dest] = registers[a] + registers[b] + (carryFlag ? 1 : 0);
		carryFlag = (((unsigned)registers[a] & 0xFF) + ((unsigned)registers[b] & 0xFF) + (carryFlag ? 1 : 0)) > 255;
		break;
	case 0b010: //SUB W CAR
		registers[dest] = registers[a] - registers[b] - (carryFlag ? 1 : 0);
		carryFlag = (((int)registers[a] & 0xFF) + ((int)registers[b] & 0xFF) - (carryFlag ? 1 : 0)) < 0;
		break;
	case 0b011: //SUB
		registers[dest] = registers[a] - registers[b];
		carryFlag = (((int)registers[a] & 0xFF) + ((int)registers[b] & 0xFF)) < 0;
		break;
	case 0b100: //NAND
		registers[dest] = ~(registers[a] & registers[b]);
		break;
	case 0b101: { //MUL
		auto highDest = (dest >> 2) | 0b100;
		auto lowDest = (dest & 0b11) | 0b100;

		uint16_t res = ((uint16_t)registers[a] & 0xFF) * ((uint16_t)registers[b] & 0xFF);

		registers[highDest] = res >> 8;
		registers[lowDest] = res & 0xFF;
		break;
	}
	case 0b110: //SHL
		registers[dest] = registers[a] << registers[b];
		break;
	case 0b111: //SHR
		registers[dest] = registers[a] >> registers[b];
		break;
	default:
		//BAD. SHOULDN'T HAPPEN.
		break;
	}
}