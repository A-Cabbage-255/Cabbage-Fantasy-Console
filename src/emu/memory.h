#pragma once
#include "../common.h"

constexpr size_t MEM_RAM_SIZE = 0x10000000;
constexpr size_t MEM_IVT_SIZE = 0x400;
constexpr size_t MEM_SPRMEM_SIZE = 0x1000000;
constexpr size_t MEM_SPRFLAG_SIZE = 0x2000;
constexpr size_t MEM_SPRDAT_SIZE = 0x8000;
constexpr size_t MEM_PAL_SIZE = 0x300;
constexpr size_t MEM_INFO_SIZE = 0x5;

constexpr size_t MEM_SIZE = MEM_RAM_SIZE + MEM_IVT_SIZE + MEM_SPRMEM_SIZE + MEM_SPRFLAG_SIZE + MEM_SPRDAT_SIZE + MEM_PAL_SIZE + MEM_INFO_SIZE;

constexpr size_t MEM_REG_SIZES[] = {
	MEM_RAM_SIZE,
	MEM_IVT_SIZE,
	MEM_SPRMEM_SIZE,
	MEM_SPRFLAG_SIZE,
	MEM_SPRDAT_SIZE,
	MEM_PAL_SIZE,
	MEM_INFO_SIZE
};

constexpr size_t MEM_REG_INTERNAL_START[] = {
	0x00000000,
	MEM_RAM_SIZE,
	MEM_RAM_SIZE + MEM_IVT_SIZE,
	MEM_RAM_SIZE + MEM_IVT_SIZE + MEM_SPRMEM_SIZE,
	MEM_RAM_SIZE + MEM_IVT_SIZE + MEM_SPRMEM_SIZE + MEM_SPRFLAG_SIZE,
	MEM_RAM_SIZE + MEM_IVT_SIZE + MEM_SPRMEM_SIZE + MEM_SPRFLAG_SIZE + MEM_SPRDAT_SIZE,
	MEM_RAM_SIZE + MEM_IVT_SIZE + MEM_SPRMEM_SIZE + MEM_SPRFLAG_SIZE + MEM_SPRDAT_SIZE + MEM_PAL_SIZE
};

constexpr size_t MEM_REG_START[] = {
	0x00000000,
	0xE0000000,
	0xE1000000,
	0xE2000000,
	0xE3000000,
	0xE4000000,
	0xF0000000
};

constexpr size_t MEM_REG_END[] = {
	0xE0000000,
	0xE0000400,
	0xE2000000,
	0xE2002000,
	0xE3008000,
	0xE4000300,
	0xF0000005
};

enum class MemoryRegion : int {
	General = 0,
	IVT = 1,
	Sprites = 2,
	SpriteFlags = 3,
	SpriteData = 4,
	Palette = 5,
	ReadOnly = 6,
	MAX = 7,
	INVALID = -1
};

class Memory {
private:
	Uint8* data;
public:
	std::function<void(void)> paletteUpdated;
	std::function<void(unsigned, bool)> spriteDataUpdated;

	Memory() {
		data = new Uint8[MEM_SIZE];

		data[MEM_REG_INTERNAL_START[6]+0] = 0x10;
		data[MEM_REG_INTERNAL_START[6]+1] = 0x00;
		data[MEM_REG_INTERNAL_START[6]+2] = 0x00;
		data[MEM_REG_INTERNAL_START[6]+3] = 0x00;
		data[MEM_REG_INTERNAL_START[6]+4] = 0x00;
	}

	~Memory() {
		delete[] data;
	}

	Uint8 getter8(Uint32 addr);
	Uint16 getter16(Uint32 addr);
	void setter8(Uint32 addr, Uint8 val);
	void setter16(Uint32 addr, Uint16 val);

	std::function<Uint8(Uint32)> getter8(MemoryRegion region);
	std::function<Uint16(Uint32)> getter16(MemoryRegion region);
	std::function<void(Uint32, Uint8)> setter8(MemoryRegion region);
	std::function<void(Uint32, Uint16)> setter16(MemoryRegion region);

	//void copy(const void* src, size_t count, MemoryRegion region);

	//void setInterrupt(Uint8 ind, Uint32 val);
	Uint32 getInterrupt(Uint8 ind);
};