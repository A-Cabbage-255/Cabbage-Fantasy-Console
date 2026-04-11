#include "asmparser.h"

Assembler::Assembler(std::string filePath) {
	//TODO
}
Assembler::~Assembler() {
	//TODO
}

std::string Assembler::readLine() {
	std::string ret = "";

	unsigned char current;
	while (SDL_ReadU8(file, &current)) {
		if (current == '\n') {
			break;
		}
		ret += current;
	}
	if (current != '\n') {
		if (SDL_GetIOStatus(file) == SDL_IO_STATUS_EOF) {
			eof = true;
		} else {
			std::cout << "SDL ERROR when reading file\n\t" << SDL_GetError() << "\n" << std::flush;
			assert(false);
		}
	}

	return ret;
}

void Assembler::parseLine() {
	if (pass == 0) {
		std::cout << "Line\n\t" << readLine() << '\n';
	}
}