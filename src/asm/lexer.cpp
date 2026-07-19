#include "lexer.h"

Lexer::Lexer(Tokenizer* tok) {
	t = tok;
}

void Lexer::scanLabels() {
	UnparsedInstruction cur;
	unsigned curbyte = 0;
	while (!(cur = nextInstruction()).eof) {
		if (cur.label) {
			consts.insert({cur.name, curbyte});
			continue;
		}

		curbyte += 2;

		if (cur.name == "LIM"s || (cur.name == "IMM"s && cur.arguments[1].number > UINT8_MAX)) {
			curbyte += 2;
		} else if (cur.name[0] == 'J' && cur.name != "JMP"s) {
			curbyte += 2;
		}
	}
	t->reset();
}

UnparsedInstruction Lexer::nextInstruction() {
	UnparsedInstruction ret;
	Token cur = t->parseToken();
	if (cur.type == TOKEN_EOF) {
		return {"", {}, false, true};
	}
	assert(cur.type == TOKEN_IDENTIFIER);

	ret.name = cur.str;

	if (t->isToken(token(TokenType::TOKEN_TILDE))) {
		return ret;
	} else if (t->isToken(token(TokenType::TOKEN_COLON))) {
		ret.label = true;
		return ret;
	}

	cur = t->parseToken();
	ret.arguments.push_back(cur);
	while (t->isToken(token(TokenType::TOKEN_COMMA))) {
		cur = t->parseToken();
		ret.arguments.push_back(cur);
	}

	return ret;
}

BasicInstruction* Lexer::lex(UnparsedInstruction inst, unsigned nextbytepos) {
	if (inst.eof) return new BasicInstruction({OPC_EOF});

	if (inst.label) return nullptr;

	if (inst.name == "NOP"s) {
		assert(inst.arguments.size() == 0);

		return new ALUInstruction({OPC_ADD, 0, 0, 0});
	} else if (inst.name == "DRW"s) {
		assert(inst.arguments.size() == 0);

		return new INTInstruction({OPC_INT, 0xFE});
	} else if (inst.name == "END"s) {
		assert(inst.arguments.size() == 0);
		
		return new INTInstruction({OPC_INT, 0x00});
	} else if (inst.name == "IMM"s) {
		assert(inst.arguments.size() == 2);

		Uint16 value = inst.arguments[1].number;
		if (inst.arguments[1].type == TOKEN_IDENTIFIER) {
			value = consts[inst.arguments[1].str];
		}
		if (inst.arguments[1].number > UINT8_MAX) {
			return new IMMInstruction({OPC_LIMM, inst.arguments[0].number, value});
		} else {
			return new IMMInstruction({OPC_IMM, inst.arguments[0].number, value});
		}
	} else if (inst.name == "LIM"s) {
		assert(inst.arguments.size() == 2);
		
		Uint16 value = inst.arguments[1].number;
		if (inst.arguments[1].type == TOKEN_IDENTIFIER) {
			std::cout << "Collapsing label " << inst.arguments[1].str << " to " << consts[inst.arguments[1].str] << "\n"; //??
			value = consts[inst.arguments[1].str];
		}
		return new IMMInstruction({OPC_LIMM, inst.arguments[0].number, value});
	} else if (inst.name == "INT"s) {
		assert(inst.arguments.size() == 1);

		return new INTInstruction({OPC_INT, inst.arguments[0].number});
	} else if (inst.name == "SET"s) {
		assert(inst.arguments.size() == 1);

		return new RAMInstruction({OPC_STR, inst.arguments[0].number});
	} else if (inst.name == "GET"s) {
		assert(inst.arguments.size() == 1);

		return new RAMInstruction({OPC_GET, inst.arguments[0].number});
	} else if (inst.name == "S16"s) {
		assert(inst.arguments.size() == 1);

		return new RAMInstruction({OPC_STRL, inst.arguments[0].number});
	} else if (inst.name == "G16"s) {
		assert(inst.arguments.size() == 1);

		return new RAMInstruction({OPC_GETL, inst.arguments[0].number});
	} else if (inst.name == "JMP"s) {
		assert((inst.arguments.size() == 3) || (inst.arguments.size() == 2));

		if (inst.arguments.size() == 3) {
			assert(inst.arguments[2].str == "USR"s);
			return new L_JMPInstruction({OPC_JDIR, true, inst.arguments[0].number, inst.arguments[1].number});
		} else if (inst.arguments.size() == 2) {
			return new L_JMPInstruction({OPC_JDIR, false, inst.arguments[0].number, inst.arguments[1].number});
		}
	} else if (inst.name == "JEZ"s) {
		assert((inst.arguments.size() == 3) || (inst.arguments.size() == 2));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 2) {
			assert(inst.arguments[2].str == "USR"s);
			return new JMPInstruction({OPC_JEZ, true, inst.arguments[0].number, inst.arguments[1].number});
		} else {
			return new JMPInstruction({OPC_JEZ, false, inst.arguments[0].number, inst.arguments[1].number});
		}
	} else if (inst.name == "JGZ"s) {
		assert((inst.arguments.size() == 3) || (inst.arguments.size() == 2));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 2) {
			assert(inst.arguments[2].str == "USR"s);
			return new JMPInstruction({OPC_JGZ, true, inst.arguments[0].number, inst.arguments[1].number});
		} else {
			return new JMPInstruction({OPC_JGZ, false, inst.arguments[0].number, inst.arguments[1].number});
		}
	} else if (inst.name == "JLZ"s) {
		assert((inst.arguments.size() == 3) || (inst.arguments.size() == 2));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 2) {
			assert(inst.arguments[2].str == "USR"s);
			return new JMPInstruction({OPC_JLZ, true, inst.arguments[0].number, inst.arguments[1].number});
		} else {
			return new JMPInstruction({OPC_JLZ, false, inst.arguments[0].number, inst.arguments[1].number});
		}
	} else if (inst.name == "JE1"s) {
		assert((inst.arguments.size() == 3) || (inst.arguments.size() == 2));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 2) {
			assert(inst.arguments[2].str == "USR"s);
			return new JMPInstruction({OPC_JE1, true, inst.arguments[0].number, inst.arguments[1].number});
		} else {
			return new JMPInstruction({OPC_JE1, false, inst.arguments[0].number, inst.arguments[1].number});
		}
	} else if (inst.name == "JCF"s) {
		assert((inst.arguments.size() == 2) || (inst.arguments.size() == 1));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 1) {
			assert(inst.arguments[1].str == "USR"s);
			return new JMPInstruction({OPC_JCF, true, inst.arguments[0].number, 0});
		} else {
			return new JMPInstruction({OPC_JCF, false, inst.arguments[0].number, 0});
		}
	} else if (inst.name == "JNZ"s) {
		assert((inst.arguments.size() == 3) || (inst.arguments.size() == 2));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 2) {
			assert(inst.arguments[2].str == "USR"s);
			return new JMPInstruction({OPC_JNEZ, true, inst.arguments[0].number, inst.arguments[1].number});
		} else {
			return new JMPInstruction({OPC_JNEZ, false, inst.arguments[0].number, inst.arguments[1].number});
		}
	} else if (inst.name == "JLE"s) {
		assert((inst.arguments.size() == 3) || (inst.arguments.size() == 2));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 2) {
			assert(inst.arguments[2].str == "USR"s);
			return new JMPInstruction({OPC_JNGZ, true, inst.arguments[0].number, inst.arguments[1].number});
		} else {
			return new JMPInstruction({OPC_JNGZ, false, inst.arguments[0].number, inst.arguments[1].number});
		}
	} else if (inst.name == "JGE"s) {
		assert((inst.arguments.size() == 3) || (inst.arguments.size() == 2));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 2) {
			assert(inst.arguments[2].str == "USR"s);
			return new JMPInstruction({OPC_JNLZ, true, inst.arguments[0].number, inst.arguments[1].number});
		} else {
			return new JMPInstruction({OPC_JNLZ, false, inst.arguments[0].number, inst.arguments[1].number});
		}
	} else if (inst.name == "JN1"s) {
		assert((inst.arguments.size() == 3) || (inst.arguments.size() == 2));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 2) {
			assert(inst.arguments[2].str == "USR"s);
			return new JMPInstruction({OPC_JNE1, true, inst.arguments[0].number, inst.arguments[1].number});
		} else {
			return new JMPInstruction({OPC_JNE1, false, inst.arguments[0].number, inst.arguments[1].number});
		}
	} else if (inst.name == "JNC"s) {
		assert((inst.arguments.size() == 2) || (inst.arguments.size() == 1));

		if (inst.arguments[0].type == TOKEN_IDENTIFIER) {
			int16 ofst = (int16)consts[inst.arguments[0].str] - (int16)nextbytepos;
			inst.arguments[0].number = (Uint32)*reinterpret_cast<Uint16*>(&ofst);
		}

		if (inst.arguments.size() > 1) {
			assert(inst.arguments[1].str == "USR"s);
			return new JMPInstruction({OPC_JNCF, true, inst.arguments[0].number, 0});
		} else {
			return new JMPInstruction({OPC_JNCF, false, inst.arguments[0].number, 0});
		}
	} else if (inst.name == "ADD"s) {
		assert(inst.arguments.size() == 3);

		return new ALUInstruction({OPC_ADD, inst.arguments[0].number, inst.arguments[1].number, inst.arguments[2].number});
	} else if (inst.name == "ADC"s) {
		assert(inst.arguments.size() == 3);

		return new ALUInstruction({OPC_ADC, inst.arguments[0].number, inst.arguments[1].number, inst.arguments[2].number});
	} else if (inst.name == "SUB"s) {
		assert(inst.arguments.size() == 3);

		return new ALUInstruction({OPC_SUB, inst.arguments[0].number, inst.arguments[1].number, inst.arguments[2].number});
	} else if (inst.name == "SWC"s) {
		assert(inst.arguments.size() == 3);

		return new ALUInstruction({OPC_SWC, inst.arguments[0].number, inst.arguments[1].number, inst.arguments[2].number});
	} else if (inst.name == "NND"s) {
		assert(inst.arguments.size() == 3);

		return new ALUInstruction({OPC_NAND, inst.arguments[0].number, inst.arguments[1].number, inst.arguments[2].number});
	} else if (inst.name == "MUL"s) {
		assert(inst.arguments.size() == 3);
		assert(inst.arguments[0].number < 4);
		assert(inst.arguments[1].number - 4 < 4);

		return new MultInstruction({OPC_MUL, inst.arguments[0].number, inst.arguments[1].number, inst.arguments[2].number, inst.arguments[3].number});
	} else if (inst.name == "SHL"s) {
		assert(inst.arguments.size() == 3);

		return new ALUInstruction({OPC_SHL, inst.arguments[0].number, inst.arguments[1].number, inst.arguments[2].number});
	} else if (inst.name == "SHR"s) {
		assert(inst.arguments.size() == 3);

		return new ALUInstruction({OPC_SHR, inst.arguments[0].number, inst.arguments[1].number, inst.arguments[2].number});
	} else if (inst.name == "MOV"s) {
		assert(inst.arguments.size() == 2);

		return new ALUInstruction({OPC_ADD, inst.arguments[0].number, inst.arguments[1].number, 0});
	}

	std::cerr << "ERROR UNKNOWN INSTRUCTION: " << inst.name << "\n";

	assert(false);
	return nullptr; //Execution never reaches this, line is here to prevent warnings
}