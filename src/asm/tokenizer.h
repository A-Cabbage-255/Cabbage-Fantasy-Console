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
	size_t filesize;

	unsigned index = 0;
public:
	bool eof = false;
	//std::vector<Token> tokens;

	Tokenizer(std::string filepath);
	~Tokenizer();

	bool match(char* c);
	char get();
	char peek();

	Token parseToken();
};