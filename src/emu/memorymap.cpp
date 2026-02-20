#include "memory.h"


std::function<Uint8(Uint32)> Memory::getter8(MemoryRegion region) {
#define GETTER8_MAP(x,y) case MemoryRegion::x:\
							return [this](Uint32 i) -> Uint8 {return y[i];}

	switch (region) {
		GETTER8_MAP(General,     generalRAM);
		GETTER8_MAP(IVT,         IVT);
		GETTER8_MAP(Sprites,     SpriteMem);
		GETTER8_MAP(SpriteFlags, SpriteFlags);
		GETTER8_MAP(SpriteData,  SpriteData);
		GETTER8_MAP(Palette,     Palette);
	}

#undef GETTER8_MAP
}

std::function<Uint16(Uint32)> Memory::getter16(MemoryRegion region) {
#define GETTER16_MAP(x,y) case MemoryRegion::x:                         \
							return [this](Uint32 i) -> Uint16 {         \
								return ((Uint16)y[i] << 8) | (y[i+1]);  \
							}

	switch (region) {
		GETTER16_MAP(General,     generalRAM);
		GETTER16_MAP(IVT,         IVT);
		GETTER16_MAP(Sprites,     SpriteMem);
		GETTER16_MAP(SpriteFlags, SpriteFlags);
		GETTER16_MAP(SpriteData,  SpriteData);
		GETTER16_MAP(Palette,     Palette);
	}

#undef GETTER16_MAP
}

std::function<void(Uint32, Uint8)> Memory::setter8(MemoryRegion region) {
#define SETTER8_MAP(x,y) case MemoryRegion::x: \
							return [this](Uint32 i, Uint8 v) -> void { \
								y[i] = v; \
							}

	switch (region) {
		SETTER8_MAP(General,     generalRAM);
		SETTER8_MAP(IVT,         IVT);
		SETTER8_MAP(Sprites,     SpriteMem);
		SETTER8_MAP(SpriteFlags, SpriteFlags);
		SETTER8_MAP(SpriteData,  SpriteData);
		SETTER8_MAP(Palette,     Palette);
	}

#undef SETTER8_MAP
}

std::function<void(Uint32, Uint16)> Memory::setter16(MemoryRegion region) {
#define SETTER16_MAP(x,y) case MemoryRegion::x: \
							return [this](Uint32 i, Uint16 v) -> void { \
								y[i] = v >> 8; \
								y[i+1] = v & 0xFF; \
							}

	switch (region) {
		SETTER16_MAP(General,     generalRAM);
		SETTER16_MAP(IVT,         IVT);
		SETTER16_MAP(Sprites,     SpriteMem);
		SETTER16_MAP(SpriteFlags, SpriteFlags);
		SETTER16_MAP(SpriteData,  SpriteData);
		SETTER16_MAP(Palette,     Palette);
	}

#undef SETTER16_MAP
}