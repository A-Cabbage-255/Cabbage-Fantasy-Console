#include <SDL3/SDL.h>
#include "../common.h"
#include "asmparser.h"

int main(int argc, char* argv[]) {
	Assembler a("assets/asm.asm");

	std::cout << "\tSTART\n";

	while (!a.eof) {
		a.parseLine();
	}

	std::cout << "\tEND\n";

	std::cout << "Pointers:\n";

	for (auto it = a.pointers.cbegin(); it != a.pointers.cend(); ++it) {
		std::cout << "\t{" << it->first << ", " << it->second << "}\n";
	}

	return 0;
}