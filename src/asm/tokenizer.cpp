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
	case '~':
		return token(TOKEN_TILDE);
	case '.':
		return token(TOKEN_PERIOD);
	case '$':
		return token(TOKEN_DOLLAR);
	}

	if (char_isAlphabeticalEx(c)) {
		std::string s(1, toupper(c));
		while (char_isAlphaNumEx(peek()) && !eof()) {
			c = get();
			s += toupper(c);
		}

		if (s == "R0"s || s == "ZR"s) {
			return {TOKEN_REGISTER, 0, ""};
		} else if (s == "R1"s) {
			return {TOKEN_REGISTER, 1, ""};
		} else if (s == "R2"s) {
			return {TOKEN_REGISTER, 2, ""};
		} else if (s == "R3"s) {
			return {TOKEN_REGISTER, 3, ""};
		} else if (s == "R4"s) {
			return {TOKEN_REGISTER, 4, ""};
		} else if (s == "R5"s) {
			return {TOKEN_REGISTER, 5, ""};
		} else if (s == "R6"s) {
			return {TOKEN_REGISTER, 6, ""};
		} else if (s == "R7"s) {
			return {TOKEN_REGISTER, 7, ""};
		} else if (s == "R8"s) {
			return {TOKEN_REGISTER, 8, ""};
		} else if (s == "R9"s) {
			return {TOKEN_REGISTER, 9, ""};
		} else if (s == "R10"s || s == "USP"s) {
			return {TOKEN_REGISTER, 10, ""};
		} else if (s == "R11"s || s == "LSP"s) {
			return {TOKEN_REGISTER, 11, ""};
		} else if (s == "R12"s || s == "URA"s) {
			return {TOKEN_REGISTER, 12, ""};
		} else if (s == "R13"s || s == "LRA"s) {
			return {TOKEN_REGISTER, 13, ""};
		} else if (s == "R14"s || s == "UMA"s) {
			return {TOKEN_REGISTER, 14, ""};
		} else if (s == "R15"s || s == "LMA"s) {
			return {TOKEN_REGISTER, 15, ""};
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
	if (c == '-') {
		assert(char_isNumeric(peek()));

		unsigned ret = 0;

		while (char_isNumeric(peek())) {
			c = get();
			ret *= 10;
			ret += (unsigned)c - (unsigned)'0';

			if (eof()) break;
		}
		return {TOKEN_NUMBER, (0xFFFF ^ ret) + 1, ""};
	}
	if (c == '"') {
		std::string s = "";
		while ((c = get()) != '"' && !eof()) {
			s += c;
		}
		return {TOKEN_STRING, 0, s};
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