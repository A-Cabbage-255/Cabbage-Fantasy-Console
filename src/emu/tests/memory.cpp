#include "memory.h"
#include "../memory.h"
#include <random>
#include <chrono>

std::random_device dev;
std::mt19937 gen(dev());

void TEST_Memory() {
	CONS_Log("Beginning Memory Unit Test...\n");

	Memory* mem = new Memory;

	TEST_Memory_General((void*)mem);
	
	delete mem;

	CONS_Log("Finished Memory Unit Test.\n");
}

void TEST_Memory_General(void* memObj) {
	CONS_Log("Testing General Memory...\n");

	unsigned long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	gen.seed(ms);

	std::uniform_int_distribution<Uint8> randomByte(0, 255);
	std::uniform_int_distribution<unsigned> randomIdx(0, 0xFFFFFFF);

	Memory* mem = (Memory*)memObj;

	for (unsigned i = 0; i < 5000; i++) {
		CONS_LogPercent(i, 10000);
		Uint32 indx = randomIdx(gen);
		Uint8 byte = randomByte(gen);
		mem->setter8()(indx, byte);
	}

	gen.seed(ms);

	for (unsigned i = 0; i < 5000; i++) {
		CONS_LogPercent(i + 5000, 10000);
		Uint32 indx = randomIdx(gen);
		Uint8 byte = randomByte(gen);
		assert(mem->getter8()(indx) == byte);
	}
	CONS_FinishPercent();

	CONS_Log("Done Testing General Memory.\n");
}
