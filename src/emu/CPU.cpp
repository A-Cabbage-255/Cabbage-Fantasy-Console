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
	switch ((IntID)id) {
	case IntID::Power:
		finished = true;
		return;
	case IntID::Draw:
		pauseToRender();
		return;
	case IntID::Addr_Keyb_Input:
	case IntID::Addr_Usr_Mem_Higher:
	case IntID::Addr_Usr_Mem_Lower:
		interrupt(IntID::Ill_Interrupt);
		return;
	default:
		break;
	}

	kernelMode = true;
	
	Uint32 destination = m->getInterrupt(id);

	instPntr = destination - 2;
}

void CPU::tick() {
	Uint16 inst = m->getter16()(instPntr);
	execIns(inst);

	if (!kernelMode && (instPntr >= m->getInterrupt(std::to_underlying(IntID::Addr_Usr_Mem_Higher)) || instPntr < m->getInterrupt(std::to_underlying(IntID::Addr_Usr_Mem_Lower)) ) ) {
		interrupt(IntID::Ill_Command);
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
			execRAM(i);
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
	Uint16 aval = registers[a];
	Uint16 bval = registers[b];

	
	auto res = alu.execute((ALU::Operation)(i >> 12), registers[a], registers[b]);

	if (std::holds_alternative<Uint32>(res)) {
		auto highDest = (dest >> 2);
		auto lowDest = (dest & 0b11) | 0b100;
		
		if (alu.carry) registers[highDest] = std::get<Uint32>(res) >> 16;

		registers[lowDest] = std::get<Uint32>(res) & 0xFFFF;
	} else {
		registers[dest] = std::get<Uint16>(res);
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
		instPntr += 2;
		Uint16 change_unsigned = m->getter16()(instPntr);
		int16 change = reinterpret_cast<int16&>(change_unsigned);

		bool jmp = false;

		switch ((i >> 8) & 0b111) {
		case 0x0:
			jmp = toCheck == 0;
			break;
		case 0x1:
			jmp = toCheck < 0b1000000000000000; //FIXME broken?
			break;
		case 0x2:
			jmp = toCheck > 0b1000000000000000;
			break;
		case 0x3:
			jmp = toCheck == registers[1];
			break;
		case 0x4:
			jmp = alu.carry;
			break;
		}

		if ((i >> 11) & 1) jmp = !jmp;

		if (jmp) {
			instPntr += change;
			instPntr -= 4;
		}
	}
	if ((i >> 12) & 1) {
		if (!kernelMode) {
			interrupt(IntID::Ill_Command);
		}
		kernelMode = false;
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

void CPU::execRAM(Uint16 ins) {
	auto reg = ins & 0xF;
	unsigned addr = (((unsigned)registers[14]) << 16) | registers[15];
	if ((ins >> 4) & 1) {
		addr = (((unsigned)registers[10]) << 16) | registers[11];
	}

	if (!((ins >> 12) & 1)) {
		if (!((ins >> 10) & 1)) {
			registers[reg] = m->getter16()(addr);
		} else {
			m->setter16()(addr, registers[reg]);
		}
	} else {
		if (!((ins >> 10) & 1)) {
			registers[reg] = (registers[reg] & 0xFF00) | m->getter8()(addr);
		} else {
			m->setter8()(addr, registers[reg]);
		}
	}
}