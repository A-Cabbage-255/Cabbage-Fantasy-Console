#include "ALU.h"
#include <cinttypes>

std::variant<Uint16, Uint32> ALU::execute(Operation op, Uint16 a, Uint16 b) {
	std::variant<Uint16,Uint32> ret = (Uint16)0;
	switch (op) {
	case ADD:
		ret = (Uint16)(a + b);
		carry = (a + b > UINT16_MAX); //depends on integer promotion. Is this reliable?
		break;
	case ADD_w_CAR:
		ret = (Uint16)(a + b + carry);
		carry = (a + b + carry > UINT16_MAX);
		break;
	case SUB_w_CAR:
		ret = (Uint16)(a - b - carry);
		carry = (a - b - carry < 0);
		break;
	case SUB:
		ret = (Uint16)(a - b);
		carry = (a - b < 0); //extra-so depends on integer promotion. Is this reliable?
		break;
	case NAND:
		ret = (Uint16)~(a & b);
		carry = true;
		break;
	case MULT:
		ret = (Uint32)a * (Uint32)b;
		carry = std::get<Uint32>(ret) > UINT16_MAX;
		break;
	case SHIFT_L:
		ret = (Uint16)(a << b);
		break;
	case SHIFT_R:
		ret = (Uint16)(a >> b);
		break;
	default: assert(false);
	}
	return ret;
}