#include "compiler.h"
#include <SDL3_image/SDL_image.h>

Compiler::Compiler(std::string path) {
	file = SDL_IOFromFile(path.c_str(), "wb");

	SDL_WriteU32BE(file, 0);
	SDL_WriteU32BE(file, 0);
	curRegionStart = getFilePos();
}

Compiler::~Compiler() {
	Uint32 cur = getFilePos();
	auto size = cur - curRegionStart;

	SDL_SeekIO(file, curRegionStart - 4, SDL_IO_SEEK_SET);
	SDL_WriteU32BE(file, size);
	SDL_SeekIO(file, cur, SDL_IO_SEEK_SET);

	SDL_CloseIO(file);
}

unsigned Compiler::getFilePos() {
	return SDL_TellIO(file);
}

unsigned Compiler::getPos() {
	return outputtedLoc;
}

void Compiler::beginRegion(Uint32 strt) {
	Uint32 cur = getFilePos();
	auto size = cur - curRegionStart;

	SDL_SeekIO(file, curRegionStart - 4, SDL_IO_SEEK_SET);
	SDL_WriteU32BE(file, size);
	SDL_SeekIO(file, cur, SDL_IO_SEEK_SET);
	SDL_WriteU32BE(file, strt);
	SDL_WriteU32BE(file, 0);
	curRegionStart = getFilePos();

	outputtedLoc = strt;
}

void Compiler::outIns_ALU(ALUInstruction* i) {
	Uint16 opc = i->code - OPC_ADD;

	SDL_WriteU16BE(file, (opc << 12) | (i->dest << 8) | (i->a << 4) | i->b);

	outputtedLoc += 2;
}

void Compiler::outIns_JMP(JMPInstruction* i) {
	Uint16 opc = i->code - OPC_JEZ;

	SDL_WriteU16BE(file, 0x8000 | (i->userMode * 0x1000) | (opc << 8) | (i->check << 4));
	SDL_WriteU16BE(file, i->offset);

	outputtedLoc += 4;
}

void Compiler::outIns_LJMP(L_JMPInstruction* i) {
	SDL_WriteU16BE(file, 0xA000 | (i->userMode * 0x1000) | (i->high << 4) | i->low);

	outputtedLoc += 2;
}

void Compiler::outIns_INT(INTInstruction* i) {
	SDL_WriteU16BE(file, 0xC800 | i->intID);

	outputtedLoc += 2;
}

void Compiler::outIns_IMM(IMMInstruction* i) {
	SDL_WriteU16BE(file, 0xE000 | (0x1000 * (i->code == OPC_LIMM)) | (i->reg << 8) | (i->value & 0xFF));
	if (i->code == OPC_LIMM) {
		SDL_WriteU16BE(file, i->value);

		outputtedLoc += 2;
	}

	outputtedLoc += 2;
}

void Compiler::outIns_QIMM(Q_IMMInstruction* i) {
	SDL_WriteU16BE(file, 0xF000 | (i->regh << 8));
	SDL_WriteU16BE(file, i->value >> 16);
	SDL_WriteU16BE(file, 0xF000 | (i->regl << 8));
	SDL_WriteU16BE(file, i->value & 0xFFFF);

	outputtedLoc += 8;
}

void Compiler::outIns_RAM(RAMInstruction* i) {
	SDL_WriteU16BE(file, 0xC000 | (0x1000 * (i->code == OPC_GET || i->code == OPC_STR)) | (0x0400 * (i->code == OPC_STR || i->code == OPC_STRL)) | i->reg);

	outputtedLoc += 2;
}

void Compiler::outIns_STACK(RAMInstruction* i) {
	if (i->code == OPC_PUSH) {
		SDL_WriteU16BE(file, 0xD410 | i->reg); // set #r (except w stack as the addr)
		SDL_WriteU16BE(file, 0x4000); // carry flag ON
		SDL_WriteU16BE(file, 0x2BB0); // swc lsp, lsp, 0
		SDL_WriteU16BE(file, 0x2AA0); // swc lsp, lsp, 0

		outputtedLoc += 8;
	} else if (i->code == OPC_POP) {
		SDL_WriteU16BE(file, 0x4000); // carry flag ON
		SDL_WriteU16BE(file, 0x1BB0); // adc lsp, lsp, 0
		SDL_WriteU16BE(file, 0x1AA0); // adc lsp, lsp, 0
		SDL_WriteU16BE(file, 0xD010 | i->reg); // get #r (except w stack as the addr)

		outputtedLoc += 8;
	}
}

void Compiler::outIns_RELCALL(REL_CallInstruction* i) {
	Uint32 value = getPos() + 14;
	SDL_WriteU16BE(file, 0xFC00);
	SDL_WriteU16BE(file, value >> 16);
	SDL_WriteU16BE(file, 0xFD00);
	SDL_WriteU16BE(file, value & 0xFFFF); //qim

	SDL_WriteU16BE(file, 0x4000); //scf

	SDL_WriteU16BE(file, 0x8400 | (i->userMode * 0x1000)); //jcf
	SDL_WriteU16BE(file, i->offset);

	outputtedLoc += 14;
}

void Compiler::outImg(DirectImageMetaInstruction* i) {
	unsigned startRow = 0x20000000 + i->index * 0x400000; //unsure if this is correct

	SDL_Surface* surf = IMG_Load(i->path.c_str());

	for (unsigned j = 0; j < surf->h; j++) {
		beginRegion(startRow);

		for (unsigned i = 0; i < surf->w; i++) {
			Uint8 r = 0,g = 0,b = 0,a = 0;
			SDL_ReadSurfacePixel(surf, i, j, &r, &g, &b, &a);
			Uint8 avg = ((unsigned)r + (unsigned)g + (unsigned)b) / 3;
			SDL_WriteU8(file, avg);
		}

		startRow += 0x800;
	}

	SDL_DestroySurface(surf);
}

void Compiler::outIns(BasicInstruction* i) {
	switch (i->code) { //TODO MUL
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
			break;
		case OPC_INT:
			outIns_INT((INTInstruction*)i);
			break;
		case OPC_LIMM:
		case OPC_IMM:
			outIns_IMM((IMMInstruction*)i);
			break;
		case OPC_QIMM:
			outIns_QIMM((Q_IMMInstruction*)i);
			break;
		case OPC_GET:
		case OPC_GETL:
		case OPC_STR:
		case OPC_STRL:
			outIns_RAM((RAMInstruction*)i);
			break;
		case OPC_PUSH:
		case OPC_POP:
			outIns_STACK((RAMInstruction*)i);
			break;
		case OPC_CALLR:
			outIns_RELCALL((REL_CallInstruction*)i);
			break;
		case OPC_META_ADDRCHANGE:
			beginRegion(((ADDRChangeMetaInstruction*)i)->location);
			break;
		case OPC_META_DATA:
			if (((DirectDataMetaInstruction*)i)->size == 2) {
				SDL_WriteU16BE(file, ((DirectDataMetaInstruction*)i)->value);
				outputtedLoc += 2;
			}
			break;
		case OPC_META_IMAGE:
			outImg((DirectImageMetaInstruction*)i);
			break;
		default:
			std::cerr << "ERROR" << std::endl;
			break;
	}
}