#pragma once
#include "../common.h"

typedef struct VideoCTX VideoCTX;

class Screen {
private:
	VideoCTX* context = nullptr;
	std::function<Uint8(Uint32)> getSpriteData = nullptr;
	std::function<Uint8(Uint32)> getOAM1 = nullptr;
	std::function<Uint8(Uint32)> getOAM2 = nullptr;
	std::function<Uint8(Uint32)> getPalette = nullptr;
public:
	Screen(std::function<Uint8(Uint32)> spriteMem, std::function<Uint8(Uint32)> spriteFlags, std::function<Uint8(Uint32)> spriteData, std::function<Uint8(Uint32)> palette);
	~Screen();
};