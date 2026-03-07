#pragma once
#include "../common.h"
#include "graphics.h"

class Screen {
private:
	Window* win = nullptr;
	
	std::function<Uint8(Uint32)> getSpriteData = nullptr;
	std::function<Uint8(Uint32)> getOAM1 = nullptr;
	std::function<Uint8(Uint32)> getOAM2 = nullptr;
	std::function<Uint8(Uint32)> getPalette = nullptr;
public:
	static unsigned numScreens;

	Screen(std::function<Uint8(Uint32)> spriteMem, std::function<Uint8(Uint32)> spriteFlags, std::function<Uint8(Uint32)> spriteData, std::function<Uint8(Uint32)> palette);
	~Screen();

	void update();
};