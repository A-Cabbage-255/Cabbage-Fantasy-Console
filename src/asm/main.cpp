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
		case OPC_IMM:
			std::cout << "opc | immediate r" << ((IMMInstruction*)i)->reg << ", " << ((IMMInstruction*)i)->value << "\n";
			break;
		case OPC_LIMM:
			std::cout << "opc | long immediate r" << ((IMMInstruction*)i)->reg << ", " << ((IMMInstruction*)i)->value << "\n";
			break;
		case OPC_INT:
			std::cout << "opc | interrupt with id 0x" << std::hex << ((INTInstruction*)i)->intID << std::dec << "\n";
			break;
		case OPC_GET:
			std::cout << "opc | get 8-bit number, store in r" << ((RAMInstruction*)i)->reg << "\n";
			break;
		case OPC_STR:
			std::cout << "opc | store 8-bit number from r" << ((RAMInstruction*)i)->reg << "\n";
			break;
		case OPC_GETL:
			std::cout << "opc | get 16-bit number, store in r" << ((RAMInstruction*)i)->reg << "\n";
			break;
		case OPC_STRL:
			std::cout << "opc | store 16-bit number from r" << ((RAMInstruction*)i)->reg << "\n";
			break;
		case OPC_JDIR:
			std::cout << "opc | direct jump to r" << ((L_JMPInstruction*)i)->high << ":r" << ((L_JMPInstruction*)i)->low << (((L_JMPInstruction*)i)->userMode ? " as user\n" : "\n");
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