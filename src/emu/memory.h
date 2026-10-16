#pragma once
#include "../common.h"

constexpr size_t MEM_RAM_SIZE = 0x10000000;
constexpr size_t MEM_IVT_SIZE = 0x400;
constexpr size_t MEM_SPRMEM_SIZE = 0x1000000;
constexpr size_t MEM_SPRFLAG_SIZE = 0x2000;
constexpr size_t MEM_SPRDAT_SIZE = 0x8000;
constexpr size_t MEM_PAL_SIZE = 0x300;

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
	std::function<void(void)> paletteUpdated;
	std::function<void(unsigned, bool)> spriteDataUpdated;

	Memory() {
		generalRAM = new Uint8[MEM_RAM_SIZE];
		IVT = new Uint8[MEM_IVT_SIZE];
		SpriteMem = new Uint8[MEM_SPRMEM_SIZE];
		SpriteFlags = new Uint8[MEM_SPRFLAG_SIZE];
		SpriteData = new Uint8[MEM_SPRDAT_SIZE];
		Palette = new Uint8[MEM_PAL_SIZE];
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

	void copy(const void* src, size_t count, MemoryRegion region);

	void setInterrupt(Uint8 ind, Uint32 val);
	Uint32 getInterrupt(Uint8 ind);
};