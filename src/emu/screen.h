#pragma once
#include "../common.h"

class Screen {
private:
	void* win = nullptr;
	void* screen = nullptr;

	std::function<Uint8(Uint32)> getSpriteColor;
	std::function<Uint8(Uint32)> getSpriteFlags;
	std::function<Uint8(Uint32)> getSpriteData;
	std::function<Uint8(Uint32)> getPalette;
public:
	Screen(std::function<Uint8(Uint32)> colorDat, std::function<Uint8(Uint32)> flagDat, std::function<Uint8(Uint32)> posDat, std::function<Uint8(Uint32)> palette);
	~Screen();

	bool tick();
};