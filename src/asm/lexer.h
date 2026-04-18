#pragma once
#include "tokenizer.h"

typedef enum OPCode {
	OPC_NULL,
	OPC_EOF,

	OPC_ADD,
	OPC_ADC,
	OPC_SWC,
	OPC_SUB,
	OPC_NAND,
	OPC_SHL,
	OPC_SHR,

	OPC_MUL,

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
	OPC_SCREEN
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

typedef struct S_RAMInstruction : BasicInstruction {
	bool wr;
	unsigned reg : 4;
} S_RAMInstruction;

typedef struct L_RAMInstruction : BasicInstruction {
	bool wr;
	unsigned lowReg : 4;
	unsigned highReg : 4;
} L_RAMInstruction;

typedef struct INTInstruction : BasicInstruction {
	unsigned intID : 8;
} INTInstruction;

typedef struct IMMInstruction : BasicInstruction {
	unsigned reg : 4;
	Uint16 value;
	bool forcelong = false;
} IMMInstruction;

class Lexer {
private:
	Tokenizer* t;
public:
	Lexer(Tokenizer* tok);
	
	//YOU must delete result
	BasicInstruction* lex();
};