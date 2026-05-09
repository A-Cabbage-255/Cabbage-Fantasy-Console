#include "lexer.h"

Lexer::Lexer(Tokenizer* tok) {
	t = tok;
}

BasicInstruction* Lexer::lex() {
	Token tok = t->parseToken();

	//std::cout << tok << "\n";

	if (tok.type == TOKEN_IDENTIFIER) {
		if (tok.str == "NOP"s) {
			return new BasicInstruction({OPC_NOP});
		} else if (tok.str == "DRW"s) {
			return new BasicInstruction({OPC_SCREEN});
		} else if (tok.str == "END"s) {
			return new BasicInstruction({OPC_PWR});
		} else if (tok.str == "IMM"s) {
			unsigned reg = t->parseToken().number;
			t->expectToken(token(TokenType::TOKEN_COMMA));
			unsigned value = t->parseToken().number;

			if (value > UINT8_MAX) {
				return new IMMInstruction({OPC_LIMM, reg, (Uint16)value});
			} else {
				return new IMMInstruction({OPC_IMM, reg, (Uint16)value});
			}
		} else if (tok.str == "LIM"s) {
			unsigned reg = t->parseToken().number;
			t->expectToken(token(TokenType::TOKEN_COMMA));
			unsigned value = t->parseToken().number;

			return new IMMInstruction({OPC_LIMM, reg, (Uint16)value});
		} else if (tok.str == "INT"s) {
			unsigned id = t->parseToken().number;

			return new INTInstruction({OPC_INT, id});
		} else if (tok.str == "SET"s) {
			unsigned reg = t->parseToken().number;

			return new RAMInstruction({OPC_STR, reg});
		} else if (tok.str == "GET"s) {
			unsigned reg = t->parseToken().number;

			return new RAMInstruction({OPC_GET, reg});
		} else if (tok.str == "S16"s) {
			unsigned reg = t->parseToken().number;

			return new RAMInstruction({OPC_STRL, reg});
		} else if (tok.str == "G16"s) {
			unsigned reg = t->parseToken().number;

			return new RAMInstruction({OPC_GETL, reg});
		} else if (tok.str == "JMP"s) {
			unsigned high = t->parseToken().number;
			t->expectToken(token(TokenType::TOKEN_COMMA));
			unsigned low = t->parseToken().number;
			bool usr = false;
			if (t->isToken(token(TokenType::TOKEN_COMMA))) {
				t->expectToken({TOKEN_IDENTIFIER, 0, "USR"});
				usr = true;
			}

			return new L_JMPInstruction({OPC_JDIR, usr, high, low});
		} else {
			t->expectToken(token(TOKEN_COLON));
			return new LABLMetaInstruction({OPC_LABEL, tok.str});
		}
	} else if (tok.type == TOKEN_EOF) {
		return new BasicInstruction({OPC_EOF});
	}
	
	return new BasicInstruction({OPC_NULL});
}