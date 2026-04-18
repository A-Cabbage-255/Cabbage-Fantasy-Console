#include "tokenizer.h"

Tokenizer::Tokenizer(std::string fp) {
	file = (char*)SDL_LoadFile(fp.c_str(), NULL);
	if (file == NULL) {
		std::cout << "ERROR Loading Assembly File!\n\tSDL: " << SDL_GetError() << "\n" << std::flush;
		assert(false);
	}
	it = file;
}

Tokenizer::~Tokenizer() {
	SDL_free(file);
}

bool Tokenizer::match(char expected) {
	if (eof()) return false;
	if (*it == expected) {
		it++;
		return true;
	}
	return false;
}

char Tokenizer::get() {
	it++;
	return *(it-1);
}

char Tokenizer::peek() {
	return *it;
}

void Tokenizer::skipWhitespace() {
	while (true) {
		if (eof()) break;
		if (char_isWhitespace(peek())) {
			get();
			continue;
		} else if (match(';')) {
			while (peek() != '\n' && peek() != '\r' && !eof()) {
				get();
			}
			continue;
		}
		break;
	}
}

Token Tokenizer::parseToken() {
	skipWhitespace();

	if (eof()) return token(TOKEN_EOF);

	char c = get();

	switch (c) {
	case ':':
		return token(TOKEN_COLON);
	case ',':
		return token(TOKEN_COMMA);
	}

	if (char_isAlphabeticalEx(c)) {
		std::string s(1, c);
		while (char_isAlphaNumEx(peek()) && !eof()) {
			c = get();
			s += c;
		}
		return {TOKEN_IDENTIFIER, 0, s};
	}
	if (char_isNumeric(c)) {
		unsigned ret = 0;
		if (c == '0' && match('x')) {

		}
		ret = (unsigned)c - (unsigned)'0';
		while (char_isNumeric(peek())) {
			c = get();
			ret *= 10;
			ret += (unsigned)c - (unsigned)'0';

			if (eof()) break;
		}
		return {TOKEN_NUMBER, ret, ""};
	}

	return token(TOKEN_NULL);
}