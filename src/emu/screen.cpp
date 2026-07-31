#include "screen.h"
#include "graphics.h"

Screen::Screen(std::function<Uint8(Uint32)> colorDat, std::function<Uint8(Uint32)> flagDat, std::function<Uint8(Uint32)> posDat, std::function<Uint8(Uint32)> palette)
: getSpriteColor(colorDat), getSpriteFlags(flagDat), getSpriteData(posDat), getPalette(palette) {
	win = (void*)new Window("UNTITLED", 800, 450);
	pal = (void*)new Palette();
	for (int i = 0; i < 4; i++) {
		spriteColorTex[i] = (void*)new ModifiablePalettedTexture((Window*)win, 2048, 2048);
		((ModifiablePalettedTexture*)spriteColorTex[i])->setPalette(*(Palette*)pal);
	}
	reloadSpriteColor();
}

Screen::~Screen() {
	delete (Palette*)pal;
	delete (ModifiablePalettedTexture*)spriteColorTex[0];
	delete (ModifiablePalettedTexture*)spriteColorTex[1];
	delete (ModifiablePalettedTexture*)spriteColorTex[2];
	delete (ModifiablePalettedTexture*)spriteColorTex[3];
	delete (Window*)win;
}

void Screen::drawSprite(unsigned tileX, unsigned tileY, unsigned destX, unsigned destY, unsigned p) {
	((ModifiablePalettedTexture**)spriteColorTex)[p]->draw(
		{
			(float)(tileX << 3), (float)(tileY << 3), 8, 8
		}, 
		{
			(float)destX, (float)destY,8, 8
		}
	);
}

bool Screen::tick() {
	((Window*)win)->clear();

	for (unsigned i = 0; true; i++) {
		Uint8 flags = getSpriteFlags(i);

		if (!(flags & 0b10000000)) {
			break;
		}

		Uint8 tx = getSpriteData(i * 4);
		Uint8 ty = getSpriteData(i * 4 + 1);
		Uint16 x = getSpriteData(i * 4 + 2) | (((Uint16)flags & 0b11) << 8);
		Uint16 y = getSpriteData(i * 4 + 3) | (((Uint16)flags & 0b01000000) << 2);
		Uint16 tileset = ((Uint16)flags & 0b00110000) >> 4;

		drawSprite(tx, ty, x, y, tileset);
	}

	return ((Window*)win)->tick();
}

void Screen::reloadSpriteColor() { // TODO OPTIMIZE HEAVILY
	for (int i = 0; i < 4; i++) {
		auto cur = ((ModifiablePalettedTexture**)spriteColorTex)[i];
		cur->modify({0, 0, 2048, 2048}, [this, i](int x, int y) {
			return getSpriteColor(y * 2048 + x + 0x400000 * i);
		});
	}
}

void Screen::reloadPalette() {
	for (int i = 0; i < 256; i++) {\
		((Palette*)pal)->modify(i, getPalette(0 + i * 3), getPalette(1 + i * 3), getPalette(2 + i * 3));
	}
	for (int i = 0; i < 4; i++) {
		((ModifiablePalettedTexture*)spriteColorTex[i])->setPalette(*(Palette*)pal);
	}
}