#include "compiler.h"

Compiler::Compiler(std::string path) {
	file = SDL_IOFromFile(path.c_str(), "wb");
}

Compiler::~Compiler() {
	SDL_CloseIO(file);
}

void Compiler::outIns_ALU(ALUInstruction* i) {
	Uint16 opc = i->code - OPC_ADD;
	
	SDL_WriteU16BE(file, (opc << 12) | (i->dest << 8) | (i->a << 4) | i->b);
}

void Compiler::outIns(BasicInstruction* i) {
	switch (i->code) {
		case OPC_ADD:
		case OPC_ADC:
		case OPC_SUB:
		case OPC_SWC:
		case OPC_NAND:
		case OPC_SHL:
		case OPC_SHR:
			outIns_ALU((ALUInstruction*)i);
			break;
		default:
			std::cerr << "ERROR\n";
			break;
	}
}