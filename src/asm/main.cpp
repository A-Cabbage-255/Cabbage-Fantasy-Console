#include <SDL3/SDL.h>
#include "../common.h"
#include "tokenizer.h"
#include "lexer.h"
#include "compiler.h"

int main(int argc, char* argv[]) {
	std::cout << "START ASM\n";

	Tokenizer a("assets/asm.asm");
	Lexer l(&a);
	Compiler c("assets/rom.bin");

	l.scanLabels();

	auto i = l.lex(l.nextInstruction());
	while (i == nullptr) i = l.lex(l.nextInstruction());
	while (i->code != OPC_EOF) {
		c.outIns(i);

		delete i;
		i = l.lex(l.nextInstruction());
		while (i == nullptr) i = l.lex(l.nextInstruction());
	}
	delete i;

	std::cout << "END ASM" << std::endl;
	return 0;
}