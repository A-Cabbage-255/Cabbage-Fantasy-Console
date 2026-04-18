#include "tokenizer.h"

Tokenizer::Tokenizer(std::string fp) {
	file = (char*)SDL_LoadFile(fp.c_str(), &filesize);
}

Tokenizer::~Tokenizer() {
	SDL_free(file);
}

bool Tokenizer::match(char* c) {
	//TODO
}

char Tokenizer::get() {
	index++;
	if (index >= filesize) {
		eof = true;
	}
	return file[index-1];
}

char Tokenizer::peek() {
	return file[index];
}

Token Tokenizer::parseToken() {
	if (eof) {
		return token(TOKEN_EOF);
	}

	char c = get();
	while (char_isWhitespace(c)) {
		if (eof) {
			return token(TOKEN_EOF);
		}
		c = get();
	}

	if (c == ';') {
		while (c != '\n' && c != '\r') {
			if (eof) {
				return token(TOKEN_EOF);
			}
			c = get();
		}
	}

	while (char_isWhitespace(c)) {
		if (eof) {
			return token(TOKEN_EOF);
		}
		c = get();
	}

	switch (c) {
	case ':':
		return token(TOKEN_COLON);
	case ',':
		return token(TOKEN_COMMA);
	}

	if (char_isAlphabeticalEx(c)) {
		std::string s = "";
		while (char_isAlphaNumEx(c)) { //TODO REPLACE WITH MATCH
			s += c;

			if (eof) {
				break;
			}

			c = get();
		}
		return {TOKEN_IDENTIFIER, 0, s};
	}
	if (char_isNumeric(c)) { //TODO IMPLEMENT
		if (c == '0' && peek() == 'x') {
		} else {

		}
	}

	return token(TOKEN_NULL);
}