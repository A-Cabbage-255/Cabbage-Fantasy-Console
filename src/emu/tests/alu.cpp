#include "alu.h"
#include "../ALU.h"
#include "../../common.h"
#include <chrono>
#include <random>

// TODO INCOMPLETE 
#define S(x) #x

void TEST_ALU() {
	std::random_device dev;
	std::mt19937 gen(dev());

	unsigned long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	gen.seed(ms);

	std::uniform_int_distribution<Uint16> randOP(0, 0b111);
	std::uniform_int_distribution<Uint16> randArg(0, 65535);
	
	ALU alu;

	std::cout << "\n";
	for (int i = 0; i < 10000; i++) {
		ALU::Operation op = (ALU::Operation)randOP(gen);

		auto a = randArg(gen);
		auto b = randArg(gen);

		auto res = alu.execute(op, a, b);

		switch (op) {
		case ALU::Operation::ADD:
			std::cout << "ADD";
			assert(std::get<Uint16>(res) == (a + b) % 65536);
			break;
		case ALU::Operation::NAND:
			std::cout << "NAND";
			assert(alu.carry);
			break;
		case ALU::Operation::MULT:
			std::cout << "MULT";
			assert(std::holds_alternative<Uint32>(res));
			break;
		default:
			assert(true);
		}
		std::cout << " ";
	}
	std::cout << "\n";
}