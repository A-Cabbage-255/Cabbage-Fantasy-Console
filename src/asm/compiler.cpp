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

void Compiler::outIns_JMP(JMPInstruction* i) {
	Uint16 opc = i->code - OPC_JEZ;

	SDL_WriteU16BE(file, 0x8000 | (i->userMode * 0x1000) | (opc << 8) | (i->check << 4) | i->offset);
}

void Compiler::outIns_LJMP(L_JMPInstruction* i) {
	SDL_WriteU16BE(file, 0xA000 | (i->userMode * 0x1000) | (i->high << 4) | i->low);
}

void Compiler::outIns_INT(INTInstruction* i) {
	SDL_WriteU16BE(file, 0xC800 | i->intID);
}

void Compiler::outIns_IMM(IMMInstruction* i) {
	SDL_WriteU16BE(file, 0xE000 | (0x1000 * (i->code == OPC_LIMM)) | (i->reg << 8) | (i->value & 0xFF));
	if (i->code == OPC_LIMM) {
		SDL_WriteU16BE(file, i->value);
	}
}

void Compiler::outIns_RAM(RAMInstruction* i) {
	SDL_WriteU16BE(file, 0xC000 | (0x1000 * (i->code == OPC_GET || i->code == OPC_STR)) | (0x0400 * (i->code == OPC_STR || i->code == OPC_STRL)) | i->reg);
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
		case OPC_JEZ:
		case OPC_JGZ:
		case OPC_JLZ:
		case OPC_JE1:
		case OPC_JCF:
		case OPC_JNEZ:
		case OPC_JNGZ:
		case OPC_JNLZ:
		case OPC_JNE1:
		case OPC_JNCF:
			outIns_JMP((JMPInstruction*)i);
			break;
		case OPC_JDIR:
			outIns_LJMP((L_JMPInstruction*)i);
		case OPC_INT:
			outIns_INT((INTInstruction*)i);
			break;
		case OPC_LIMM:
		case OPC_IMM:
			outIns_IMM((IMMInstruction*)i);
			break;
		case OPC_GET:
		case OPC_GETL:
		case OPC_STR:
		case OPC_STRL:
			outIns_RAM((RAMInstruction*)i);
			break;
		default:
			std::cerr << "ERROR" << std::endl;
			break;
	}
}