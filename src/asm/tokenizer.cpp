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

Token Tokenizer::parseToken() { //TODO PARSE REGISTERS PROPERLY + OFFSETS/COMPILE-TIME MATH
	skipWhitespace();

	if (eof()) return token(TOKEN_EOF);

	char c = get();

	switch (c) {
	case ':':
		return token(TOKEN_COLON);
	case ',':
		return token(TOKEN_COMMA);
	case '~':
		return token(TOKEN_TILDE);
	}

	if (char_isAlphabeticalEx(c)) {
		std::string s(1, toupper(c));
		while (char_isAlphaNumEx(peek()) && !eof()) {
			c = get();
			s += toupper(c);
		}
		return {TOKEN_IDENTIFIER, 0, s};
	}
	if (char_isNumeric(c)) {
		unsigned ret = 0;
		if (c == '0' && match('x')) {
			while (char_isNumericHex(peek())) {
				c = get();
				ret *= 16;
				ret += char_convertHex(c);

				if (eof()) break;
			}
			return {TOKEN_NUMBER, ret, ""};
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

void Tokenizer::expectToken(const Token& t) {
	assert(parseToken() == t);
}

bool Tokenizer::isToken(const Token& t) {
	auto previous = it;
	if (parseToken() == t) {
		return true;
	}
	it = previous;
	return false;
}