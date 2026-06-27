#pragma once
#include "tokenizer.h"

typedef enum OPCode {
	OPC_NULL = 0,
	OPC_EOF = 1,

	OPC_ADD = 10,
	OPC_ADC = 11,
	OPC_SWC = 12,
	OPC_SUB = 13,
	OPC_NAND = 14,
	OPC_MUL = 15,
	OPC_SHL = 16,
	OPC_SHR = 17,


	OPC_JEZ = 20,
	OPC_JGZ = 21,
	OPC_JLZ = 22,
	OPC_JE1 = 23,
	OPC_JCF = 24,
	OPC_JNEZ = 28,
	OPC_JNGZ = 29,
	OPC_JNLZ = 30,
	OPC_JNE1 = 31,
	OPC_JNCF = 32,

	OPC_JDIR = 40,

	OPC_STR,
	OPC_GET,

	OPC_STRL,
	OPC_GETL,

	OPC_INT,

	OPC_IMM,
	OPC_LIMM,

	OPC_NOP,

	OPC_LABEL,
} OPCode;

typedef struct BasicInstruction {
	OPCode code = OPC_NULL;
} BasicInstruction;

typedef struct ALUInstruction : BasicInstruction {
	unsigned dest : 4;
	unsigned a : 4;
	unsigned b : 4;
} ALUInstruction;

typedef struct MultInstruction : BasicInstruction {
	unsigned destHigh : 4;
	unsigned destLow : 4;
	unsigned a : 4;
	unsigned b : 4;
} MultInstruction;

typedef struct JMPInstruction : BasicInstruction {
	bool userMode;
	unsigned offset : 4;
	unsigned check : 4;
} JMPInstruction;

typedef struct L_JMPInstruction : BasicInstruction {
	bool userMode;
	unsigned high : 4;
	unsigned low : 4;
} L_JMPInstruction;

typedef struct RAMInstruction : BasicInstruction {
	unsigned reg : 4;
} RAMInstruction;

typedef struct INTInstruction : BasicInstruction {
	unsigned intID : 8;
} INTInstruction;

typedef struct IMMInstruction : BasicInstruction {
	unsigned reg : 4;
	Uint16 value;
} IMMInstruction;

typedef struct UnparsedInstruction {
	std::string name = ""s;
	std::vector<Token> arguments;
	bool label = false;
	bool eof = false;
} UnparsedInstruction;

class Lexer {
private:
	Tokenizer* t;
public:
	std::map<std::string, unsigned> consts;

	Lexer(Tokenizer* tok);

	//YOU must delete result
	BasicInstruction* lex(const UnparsedInstruction& inst);
	void scanLabels();
	UnparsedInstruction nextInstruction();
	
};

inline std::ostream& operator<<(std::ostream& s, const UnparsedInstruction& i) {
	s << i.name << " ";
	for (auto t : i.arguments) {
		s << t << ", ";
	}
	return s;
}