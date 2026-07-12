#include "screen.h"
#include "graphics.h"

Screen::Screen(std::function<Uint8(Uint32)> colorDat, std::function<Uint8(Uint32)> flagDat, std::function<Uint8(Uint32)> posDat, std::function<Uint8(Uint32)> palette)
: getSpriteColor(colorDat), getSpriteFlags(flagDat), getSpriteData(posDat), getPalette(palette) {
	win = (void*)new Window("UNTITLED", 800, 450);
	spriteColorTex = (void*)new ModifiablePalettedTexture((Window*)win, 2048, 2048);
	for (int i = 0; i < 256; i++) {
		((ModifiablePalettedTexture*)spriteColorTex)->changePaletteColor(i, getPalette(0x32000000 + i * 3), getPalette(0x32000001 + i * 3), getPalette(0x32000002 + i * 3));
	}
}

Screen::~Screen() {
	delete (ModifiablePalettedTexture*)spriteColorTex;
	delete (Window*)win;
}

void Screen::drawSprite(Uint8 tileX, Uint8 tileY, Uint16 destX, Uint16 destY) {
}

bool Screen::tick() {
	((ModifiablePalettedTexture*)spriteColorTex)->draw(0, 0);
	return ((Window*)win)->tick();
}
