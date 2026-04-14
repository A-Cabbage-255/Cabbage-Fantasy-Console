#include "asmparser.h"
#include <algorithm>

/// Code ripped from https://stackoverflow.com/questions/216823/how-can-i-trim-a-stdstring
// Trim from the start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// Trim from the end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// Trim from both ends (in place)
inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

// Trim from the start (copying)
inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// Trim from the end (copying)
inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// Trim from both ends (copying)
inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}
/// End of copied code

Assembler::Assembler(std::string filePath) {
	reading = SDL_IOFromFile(filePath.c_str(), "r");
}
Assembler::~Assembler() {
	SDL_CloseIO(reading);
}

std::string Assembler::readLine() {
	std::string ret = "";

	unsigned char current;
	while (SDL_ReadU8(reading, &current)) {
		if (current == '\n' || current == '\r') {
			break;
		}
		ret += current;
	}
	if (current != '\n' && current != '\r') {
		if (SDL_GetIOStatus(reading) == SDL_IO_STATUS_EOF) {
			eof = true;
		} else {
			std::cout << "SDL ERROR when reading file\n\t" << SDL_GetError() << "\n" << std::flush;
			assert(false);
		}
	} else {
		if (!SDL_ReadU8(reading, &current)) {
			if (SDL_GetIOStatus(reading) == SDL_IO_STATUS_EOF) {
				eof = true;
			} else {
				std::cout << "SDL ERROR when reading file\n\t" << SDL_GetError() << "\n" << std::flush;
				assert(false);
			}
		}
		if (current != '\n' && current != '\r') {
			SDL_SeekIO(reading, -1, SDL_IO_SEEK_CUR);
		}
	}

	return ret;
}

void Assembler::parseLine() {
	switch (pass) {
	case 0: {
		std::string s_with_comment = readLine();
		std::string s = s_with_comment.substr(0, s_with_comment.find(";"));
		trim(s);
		if (s.size() == 0) {
			break;
		}

		if (s.ends_with(":")) {
			pointers.insert({s.substr(0, s.size() - 1), line});
			break;
		}

		line++;
		break;
	}
	case 1: {
		std::string s_with_comment = readLine();
		std::string s = s_with_comment.substr(0, s_with_comment.find(";"));
		trim(s);
		if (s.size() == 0) {
			break;
		}

		std::cout << line << "\t" << s << "\n";

		line++;
		break;
	}
	}

	if (eof && pass < 1) {
		auto res = SDL_SeekIO(reading, 0, SDL_IO_SEEK_SET);
		line = 0;
		pass++;
		eof = false;
		if (res == -1) {
			std::cout << "SDL ERROR when reading file\n\t" << SDL_GetError() << "\n" << std::flush;
			assert(false);
		}
	}
}