#include <SDL3/SDL.h>
#include "../common.h"
#include "asmparser.h"

int main(int argc, char* argv[]) {
	Assembler a("");

	a.parseLine();

	return 0;
}