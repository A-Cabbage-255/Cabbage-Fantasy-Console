#include <SDL3/SDL.h>
#include "../common.h"
#include "tokenizer.h"
#include "lexer.h"
#include "compiler.h"

//TODO Better Error handling W/O assert

int main(int argc, char* argv[]) {
	std::string fileName;
	std::cout << "Name to compile: ";
	std::cin >> fileName;
	fileName = "assets/" + fileName + ".asm";

	Tokenizer a(fileName);
	Lexer l(&a);
	Compiler c("assets/rom.bin");

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

	std::cout << "Assembling Finished!\n" << std::endl;
	return 0;
}