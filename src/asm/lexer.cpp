#include "lexer.h"

Lexer::Lexer(Tokenizer* tok) {
	t = tok;
}

BasicInstruction* Lexer::lex() {
	Token tok = t->parseToken();

	if (tok.type == TOKEN_IDENTIFIER) {
		if (tok.str == "NOP"s) {
			return new BasicInstruction({OPC_NOP});
		} else if (tok.str == "DRW"s) {
			return new BasicInstruction({OPC_SCREEN});
		} else if (tok.str == "END"s) {
			return new BasicInstruction({OPC_PWR});
		}
	} else if (tok.type == TOKEN_EOF) {
		return new BasicInstruction({OPC_EOF});
	}
	
	return new BasicInstruction({OPC_NULL});
}