#pragma once
#include "tokenizer.h"

typedef enum OPCode {
	OPC_NULL = 0,
	OPC_EOF = 1,

	OPC_ADD = 2,
	OPC_ADC = 3,
	OPC_SWC = 4,
	OPC_SUB = 5,
	OPC_NAND = 6,
	OPC_SHL = 8,
	OPC_SHR = 9,

	OPC_MUL = 7,

	OPC_JEZ,
	OPC_JGZ,
	OPC_JLZ,
	OPC_JE1,
	OPC_JCF,
	OPC_JNEZ,
	OPC_JNGZ,
	OPC_JNLZ,
	OPC_JNE1,
	OPC_JNCF,

	OPC_JDIR,

	OPC_STR,
	OPC_GET,

	OPC_STRL,
	OPC_GETL,

	OPC_INT,

	OPC_IMM,
	OPC_LIMM,

	OPC_NOP,
	OPC_PWR,
	OPC_SCREEN,

	OPC_LABEL,
} OPCode;

typedef struct BasicInstruction {
	OPCode code = OPC_NULL;
} BasicInstruction;

typedef struct LABLMetaInstruction : BasicInstruction {
	std::string name = ""s;
} LABLMetaInstruction;

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