#pragma once
#include "../common.h"
#include "SDL3/SDL.h"
#include <map>

typedef enum TokenType {
	TOKEN_NULL,
	TOKEN_IDENTIFIER,
	TOKEN_COMMA,
	TOKEN_COLON,
	TOKEN_NUMBER,
	TOKEN_REGISTER,
	TOKEN_STRING,
	TOKEN_EOF,
} TokenType;

typedef struct Token {
	TokenType type = TOKEN_NULL;
	unsigned number;
	std::string str;
} Token;

inline Token token(TokenType t) {
	return {t, 0, ""};
}

inline bool operator==(const Token& a, const Token& b) {
	if (a.type != b.type) return false;

	if (a.type == TOKEN_IDENTIFIER) {
		return a.str == b.str;
	} else if (a.type == TOKEN_STRING) {
		return a.str == b.str;
	} else if (a.type == TOKEN_NUMBER) {
		return a.number == b.number;
	} else if (a.type == TOKEN_REGISTER) {
		return a.number == b.number;
	} else {
		return true;
	}
}

inline std::ostream& operator<<(std::ostream& s, const Token& t) {
	switch (t.type) {
	case TOKEN_NULL:
		s << "Null";
		break;
	case TOKEN_IDENTIFIER:
		s << "Identifier (" << t.str << ")";
		break;
	case TOKEN_COMMA:
		s << "Comma";
		break;
	case TOKEN_COLON:
		s << "Colon";
		break;
	case TOKEN_NUMBER:
		s << "Number (" << t.number << ")";
		break;
	case TOKEN_STRING:
		s << "\"" << t.str << "\"";
		break;
	case TOKEN_REGISTER:
		s << "Register " << t.number;
		break;
	case TOKEN_EOF:
		s << "EOF";
		break;
	}
	return s;
}

class Tokenizer {
private:
	char* file;

	char* it;
public:
	Tokenizer(std::string filepath);
	~Tokenizer();

	bool eof() {return (*it == '\0');}
	bool match(char expected);
	char get();
	char peek();
	void skipWhitespace();

	Token parseToken();
	void expectToken(const Token& t);
	bool isToken(const Token& t);
};