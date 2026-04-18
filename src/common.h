#pragma once
#include <iostream>
#include <functional>

#include <string>
#include <cstring>
#include <cassert>
#include <cstdint>

typedef std::uint8_t Uint8;
typedef std::uint16_t Uint16;
typedef std::uint32_t Uint32;
typedef std::uint64_t Uint64;
typedef std::int8_t int8;
typedef std::int16_t int16;
typedef std::int32_t int32;
typedef std::int64_t int64;

using namespace std::literals::string_literals;

struct IntRect {
	int x;
	int y;
	int w;
	int h;
};

enum class ColorChannel {
	red,
	green,
	blue
};

inline
int colorChannelToOffset(ColorChannel c) {
	switch (c) {
	case ColorChannel::red:
		return 0;
	case ColorChannel::green:
		return 1;
	case ColorChannel::blue:
		return 2;
	}
	return -1;
}

inline
void CONS_Log(std::string s) {
	std::clog << s;
}

inline
void CONS_LogPercent(long v, long max) {
	double fraction = (double)v / (double)max;
	std::string line = "\r[";
	for (unsigned i = 0; i < 75; i++) {
		if (i < fraction * 75) {
			line += "@";
		} else {
			line += "-";
		}
	}
	line += "]";
	std::clog << line << std::flush;
}

inline
void CONS_FinishPercent() {
	std::clog << std::endl;
}

inline
void OUT_Log(std::string s) {
	std::cout << s;
}

inline bool char_isWhitespace(char c) {
	return std::isspace((unsigned char)c);
}

inline bool char_isAlphabeticalEx(char c) {
	return std::isalpha((unsigned char)c) || c == '_' || c == '.';
}
inline bool char_isAlphaNumEx(char c) {
	return std::isalnum((unsigned char)c) || c == '_' || c == '.';
}
inline bool char_isNumeric(char c) {
	return std::isdigit((unsigned char)c);
}