#include <SDL3/SDL.h>
#include "../common.h"
#include "tokenizer.h"
#include "lexer.h"
#include "compiler.h"

//TODO Better Error handling W/O assert

int main(int argc, char* argv[]) {
	std::string fileName;
	std::string outName = "assets/rom.bin";

	if (argc == 3) {
		fileName = argv[1];
		outName = argv[2];
	} else {
		std::cout << "Name to compile: ";
		std::cin >> fileName;
		fileName = "assets/" + fileName + ".asm";
	}

	Tokenizer a(fileName);
	Lexer l(&a);
	Compiler c(outName.c_str());

	l.scanLabels();

	auto i = l.lex(l.nextInstruction(), c.getPos());
	while (i == nullptr) i = l.lex(l.nextInstruction(), c.getPos());
	while (i->code != OPC_EOF) {
		c.outIns(i);

		delete i;
		i = l.lex(l.nextInstruction(), c.getPos());
		while (i == nullptr) i = l.lex(l.nextInstruction(), c.getPos());
	}
	delete i;

	std::cout << "Assembling Finished!" << std::endl;
	return 0;
}