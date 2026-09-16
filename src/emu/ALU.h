#pragma once
#include "../common.h"
#include <variant>

class ALU {
public:
	enum Operation : Uint16 {
		ADD = 0b000,
		ADD_w_CAR = 0b001,
		SUB_w_CAR = 0b010,
		SUB = 0b011,
		NAND = 0b100,
		MULT = 0b101,
		SHIFT_L = 0b110,
		SHIFT_R = 0b111
	};

	bool carry = false;

	std::variant<Uint16, Uint32> execute(Operation op, Uint16 A, Uint16 B);
};