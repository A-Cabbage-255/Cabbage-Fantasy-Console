#include <SDL3/SDL.h>
#include "../common.h"
#include "tokenizer.h"
#include "lexer.h"

int main(int argc, char* argv[]) {
	Tokenizer a("assets/asm.asm");
	Lexer l(&a);

	BasicInstruction* i = l.lex();
	while (i->code != OPC_EOF) {
		switch (i->code) {
		case OPC_NULL:
			std::cout << "ERROR OPCODE NULL\n";
			break;
		case OPC_PWR:
			std::cout << "opc | power off\n";
			break;
		case OPC_SCREEN:
			std::cout << "opc | draw\n";
			break;
		case OPC_NOP:
			std::cout << "opc | no op\n";
			break;
		default:
			std::cout << "ERROR unknown opcode\n";
			break;
		}

		delete i;
		i = l.lex();
	}
	delete i;

	return 0;
}