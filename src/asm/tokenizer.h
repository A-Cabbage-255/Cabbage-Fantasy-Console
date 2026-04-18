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
	//std::vector<Token> tokens;

	Tokenizer(std::string filepath);
	~Tokenizer();

	bool eof() {return (*it == '\0');}
	bool match(char expected);
	char get();
	char peek();
	void skipWhitespace();

	Token parseToken();
};