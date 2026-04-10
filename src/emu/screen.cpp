#include "screen.h"
#include "graphics.h"

Screen::Screen(std::function<Uint8(Uint32)> colorDat, std::function<Uint8(Uint32)> flagDat, std::function<Uint8(Uint32)> posDat, std::function<Uint8(Uint32)> palette)
: getSpriteColor(colorDat), getSpriteFlags(flagDat), getSpriteData(posDat), getPalette(palette) {
	win = (void*)new Window("", 1600, 900, 800, 450);
	((Window*)win)->setIcon("assets/icon.png");

	screen = (void*)new ModifiableTexture((Window*)win, 800, 450);
}

Screen::~Screen() {
	delete (ModifiableTexture*)screen;
	delete (Window*)win;
}

void Screen::drawSprite(Uint8 tileX, Uint8 tileY, Uint16 destX, Uint16 destY) {
	((ModifiableTexture*)screen)->modify(
	{destX, destY, 8, 8},
	[&](int x, int y, ColorChannel col) {
			Uint32 srcx = tileX * 8 + x;
			Uint32 srcy = tileY * 8 + y;
			Uint8 c = getSpriteColor(srcx + srcy * 2048);

			if (col == ColorChannel::blue) {
				//OUT_Log("I: "s + std::to_string(((Uint32)c) * 3) + " ---- V: "s + std::to_string(getPalette(((Uint32)c) * 3 + colorChannelToOffset(col))) + "\n"s);
			}

			return getPalette(((Uint32)c) * 3 + colorChannelToOffset(col));
		}
	);
}

bool Screen::tick() {
	for (unsigned i = 0; i < 8192; i++) {
		Uint8 flags = getSpriteFlags(i);
		if (flags >> 7 == 0) {
			break;
		}
		Uint8 tileX = getSpriteData(i * 4);
		Uint8 tileY = getSpriteData(i * 4 + 1);

		Uint16 destX = getSpriteData(i * 4 + 2) | ((Uint16)(flags & 0b11) << 8);
		Uint16 destY = getSpriteData(i * 4 + 3) | ((Uint16)((flags >> 6) & 0b1) << 8);
		
		drawSprite(tileX, tileY, destX, destY);
	}

	((Window*)win)->clear(0, 0, 0);

	((ModifiableTexture*)screen)->draw(0,0);

	return ((Window*)win)->tick();
}
