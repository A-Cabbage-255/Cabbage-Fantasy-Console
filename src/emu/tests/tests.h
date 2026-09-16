#pragma once
#include "memory.h"
#include "cpu.h"
#include "alu.h"

inline void TEST_ALL() {
	TEST_Memory();
	TEST_CPU();
	TEST_ALU();
}