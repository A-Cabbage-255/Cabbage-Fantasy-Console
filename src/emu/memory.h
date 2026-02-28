#pragma once
#include "../common.h"
#include <functional>

enum class MemoryRegion {
	General,
	IVT,
	Sprites,
	SpriteFlags,
	SpriteData,
	Palette,
	INVALID
};

MemoryRegion mapAddress(Uint32 a);

class Memory {
private:
	Uint8* generalRAM = nullptr;
	Uint8* IVT = nullptr;
	Uint8* SpriteMem = nullptr;
	Uint8* SpriteFlags = nullptr;
	Uint8* SpriteData = nullptr;
	Uint8* Palette = nullptr;
public:
	Memory() {
		generalRAM = new Uint8[0x10000000];
		IVT = new Uint8[0x400];
		SpriteMem = new Uint8[0x1000000];
		SpriteFlags = new Uint8[0x2000];
		SpriteData = new Uint8[0x8000];
		Palette = new Uint8[0x100];
	}

	~Memory() {
		delete[] generalRAM;
		delete[] IVT;
		delete[] SpriteMem;
		delete[] SpriteFlags;
		delete[] SpriteData;
		delete[] Palette;
	}

	std::function<Uint8(Uint32)> getter8();
	std::function<Uint16(Uint32)> getter16();
	std::function<void(Uint32, Uint8)> setter8();
	std::function<void(Uint32, Uint16)> setter16();

	std::function<Uint8(Uint32)> getter8(MemoryRegion region);
	std::function<Uint16(Uint32)> getter16(MemoryRegion region);
	std::function<void(Uint32, Uint8)> setter8(MemoryRegion region);
	std::function<void(Uint32, Uint16)> setter16(MemoryRegion region);
};