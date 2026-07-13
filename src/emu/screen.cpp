#include "screen.h"
#include "graphics.h"

Screen::Screen(std::function<Uint8(Uint32)> colorDat, std::function<Uint8(Uint32)> flagDat, std::function<Uint8(Uint32)> posDat, std::function<Uint8(Uint32)> palette)
: getSpriteColor(colorDat), getSpriteFlags(flagDat), getSpriteData(posDat), getPalette(palette) {
	win = (void*)new Window("UNTITLED", 800, 450);
	spriteColorTex = (void*)new ModifiablePalettedTexture((Window*)win, 2048, 2048);
	for (int i = 0; i < 256; i++) {
		((ModifiablePalettedTexture*)spriteColorTex)->changePaletteColor(i, getPalette(0 + i * 3), getPalette(1 + i * 3), getPalette(2 + i * 3));
	}
}

Screen::~Screen() {
	delete (ModifiablePalettedTexture*)spriteColorTex;
	delete (Window*)win;
}

void Screen::drawSprite(unsigned tileX, unsigned tileY, unsigned destX, unsigned destY) {
	((ModifiablePalettedTexture*)spriteColorTex)->draw(
		{
			(float)(tileX << 3), (float)(tileY << 3), 8, 8
		}, 
		{
			(float)destX, (float)destY,8, 8
		}
	);
}

bool Screen::tick() {
	for (unsigned i = 0; true; i++) {
		Uint8 flags = getSpriteFlags(i);

		if (!(flags & 0b10000000)) {
			break;
		}

		Uint8 tx = getSpriteData(i * 4);
		Uint8 ty = getSpriteData(i * 4 + 1);
		Uint16 x = getSpriteData(i * 4 + 2) | (((Uint16)flags & 0b11) << 8);
		Uint16 y = getSpriteData(i * 4 + 3) | (((Uint16)flags & 0b01000000) << 2);

		drawSprite(tx, ty, x, y);
	}

	return ((Window*)win)->tick();
}
