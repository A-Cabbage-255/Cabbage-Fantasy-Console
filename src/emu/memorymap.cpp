#include "memory.h"

MemoryRegion mapAddress(Uint32 a, Uint32* index_OUT = nullptr) {
	if (a >> 28 == 0) {
		if (index_OUT != nullptr) *index_OUT = a & 0x0FFFFFFF;
		return MemoryRegion::General;
	} else {
		if (index_OUT != nullptr) *index_OUT = a & 0x00FFFFFF;
	}
	
	if (a >> 28 == 1) {
		return MemoryRegion::IVT;
	} else if (a >> 28 == 2) {
		return MemoryRegion::Sprites;
	} else if (a >> 24 == 0x30) {
		return MemoryRegion::SpriteFlags;
	} else if (a >> 24 == 0x31) {
		return MemoryRegion::SpriteData;
	} else if (a >> 24 == 0x32) {
		return MemoryRegion::Palette;
	} else {
		return MemoryRegion::INVALID;
	}
}

std::function<Uint8(Uint32)> Memory::getter8() {
	return [this](Uint32 i) -> Uint8 {
		MemoryRegion reg = mapAddress(i, &i);
		return getter8(reg)(i);
	};
}
std::function<Uint16(Uint32)> Memory::getter16() {
	return [this](Uint32 i) -> Uint16 {
		MemoryRegion reg = mapAddress(i, &i);
		return getter16(reg)(i);
	};
}
std::function<void(Uint32, Uint8)> Memory::setter8() {
	return [this](Uint32 i, Uint8 v) -> void {
		MemoryRegion reg = mapAddress(i, &i);
		return setter8(reg)(i, v);
	};
}
std::function<void(Uint32, Uint16)> Memory::setter16() {
	return [this](Uint32 i, Uint16 v) -> void {
		MemoryRegion reg = mapAddress(i, &i);
		return setter16(reg)(i, v);
	};
}

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
	default:
		assert(false);
	}

#undef GETTER8_MAP
	return nullptr;
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
	default:
		assert(false);
	}

#undef GETTER16_MAP
	return nullptr;
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
	default:
		assert(false);
	}

#undef SETTER8_MAP
	return nullptr;
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
	default:
		assert(false);
	}

#undef SETTER16_MAP
	return nullptr;
}

void Memory::copy(const void* src, size_t count, MemoryRegion region) {
	switch (region) {
	case MemoryRegion::General:
		memcpy(generalRAM, src, count);
		break;
	case MemoryRegion::Sprites:
		memcpy(SpriteMem, src, count);
		break;
	case MemoryRegion::Palette:
		memcpy(Palette, src, count);
		break;
	}
}