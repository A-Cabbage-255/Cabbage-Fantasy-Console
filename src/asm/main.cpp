#include <SDL3/SDL.h>
#include "../common.h"
#include "tokenizer.h"

int main(int argc, char* argv[]) {
	Tokenizer a("assets/asm.asm");

	Token t = token(TOKEN_NULL);
	while (t.type != TOKEN_EOF) {
		t = a.parseToken();
		std::cout << "TOKEN | " << t << "\n";
	}

	return 0;
}