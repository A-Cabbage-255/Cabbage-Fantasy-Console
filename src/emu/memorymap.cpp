#include "memory.h"

/*
MemoryRegion mapAddress(Uint32 a, Uint32* index_OUT = nullptr) {
	if (a < MEMADDR_GENERALEND) {
		if (index_OUT != nullptr) *index_OUT = a;
		return MemoryRegion::General;
	} else {

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
	switch (region) {
    case MemoryRegion::General:
    	return [this](Uint32 i, Uint8 v) -> void { generalRAM[i] = v; };

    case MemoryRegion::IVT:
      	return [this](Uint32 i, Uint8 v) -> void { IVT[i] = v; };

    case MemoryRegion::Sprites:
      	return [this](Uint32 i, Uint8 v) -> void { SpriteMem[i] = v; spriteDataUpdated(i, false); };

    case MemoryRegion::SpriteFlags:
      	return [this](Uint32 i, Uint8 v) -> void { SpriteFlags[i] = v; };

    case MemoryRegion::SpriteData:
      	return [this](Uint32 i, Uint8 v) -> void { SpriteData[i] = v; };

    case MemoryRegion::Palette:
      	return [this](Uint32 i, Uint8 v) -> void { Palette[i] = v; paletteUpdated(); };

    default:
      assert(false);
    }

	return nullptr;
}

std::function<void(Uint32, Uint16)> Memory::setter16(MemoryRegion region) {
	switch (region) {
	case MemoryRegion::General:
		return [this](Uint32 i, Uint16 v) -> void {
			generalRAM[i] = v >> 8;
			generalRAM[i + 1] = v & 0xFF;
		};

	case MemoryRegion::IVT:
		return [this](Uint32 i, Uint16 v) -> void {
			IVT[i] = v >> 8;
			IVT[i + 1] = v & 0xFF;
		};

	case MemoryRegion::Sprites:
		return [this](Uint32 i, Uint16 v) -> void {
			SpriteMem[i] = v >> 8;
			SpriteMem[i + 1] = v & 0xFF;
			spriteDataUpdated(i, true);
		};

	case MemoryRegion::SpriteFlags:
		return [this](Uint32 i, Uint16 v) -> void {
			SpriteFlags[i] = v >> 8;
			SpriteFlags[i + 1] = v & 0xFF;
		};

	case MemoryRegion::SpriteData:
		return [this](Uint32 i, Uint16 v) -> void {
			SpriteData[i] = v >> 8;
			SpriteData[i + 1] = v & 0xFF;
		};

	case MemoryRegion::Palette:
		return [this](Uint32 i, Uint16 v) -> void {
			Palette[i] = v >> 8;
			Palette[i + 1] = v & 0xFF;
			paletteUpdated();
		};

	default:
		assert(false);
	}

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
	default:
		break;
	}
}

void Memory::setInterrupt(Uint8 ind, Uint32 val) {
	setter16()((1u << 28u) | ((Uint32)ind << 2), val >> 16u);
	setter16()((1u << 28u) | ((Uint32)ind << 2) + 2, val & 0xFFFFu);
}

Uint32 Memory::getInterrupt(Uint8 ind) {
	return ((Uint32)getter16()((1u << 28u) | ((Uint32)ind << 2)) << 16u) | (Uint32)getter16()(((1u << 28u) | ((Uint32)ind << 2)) + 2);
}
*/

Uint8 Memory::getter8(Uint32 addr) {
	size_t baseIndex = 0;
	for (int i = 0; i < std::to_underlying(MemoryRegion::MAX); i++) {
		if (addr >= MEM_REG_START[i] && addr < MEM_REG_END[i]) {
			return data[baseIndex + (addr - MEM_REG_START[i])];
		}
		baseIndex += MEM_REG_SIZES[i];
	}

	assert(false);
	return 0xff;
}

Uint16 Memory::getter16(Uint32 addr) {
	size_t baseIndex = 0;
	for (int i = 0; i < std::to_underlying(MemoryRegion::MAX); i++) {
		if (addr >= MEM_REG_START[i] && addr < MEM_REG_END[i] - 1) {
			Uint32 internalAddr = baseIndex + (addr - MEM_REG_START[i]);
			return static_cast<Uint16>(data[internalAddr]) << 8 | data[internalAddr + 1];
		}
		baseIndex += MEM_REG_SIZES[i];
	}

	assert(false);
	return 0xff;
}

void Memory::setter8(Uint32 addr, Uint8 val) {
	size_t baseIndex = 0;
	for (int i = 0; i < std::to_underlying(MemoryRegion::MAX); i++) {
		if (addr >= MEM_REG_START[i] && addr < MEM_REG_END[i]) {
			if (i == std::to_underlying(MemoryRegion::Palette)) paletteUpdated();
			if (i == std::to_underlying(MemoryRegion::Sprites)) spriteDataUpdated(addr, false);
			data[baseIndex + (addr - MEM_REG_START[i])] = val;
			return;
		}
		baseIndex += MEM_REG_SIZES[i];
	}

	assert(false);
}

void Memory::setter16(Uint32 addr, Uint16 val) {
	size_t baseIndex = 0;
	for (int i = 0; i < std::to_underlying(MemoryRegion::MAX); i++) {
		if (addr >= MEM_REG_START[i] && addr < MEM_REG_END[i] - 1) {
			if (i == std::to_underlying(MemoryRegion::Palette)) paletteUpdated();
			if (i == std::to_underlying(MemoryRegion::Sprites)) spriteDataUpdated(addr, true);
			Uint32 internalAddr = baseIndex + (addr - MEM_REG_START[i]);
			data[internalAddr] = val >> 8;
			data[internalAddr+1] = val & 0xff;
			return;
		}
		baseIndex += MEM_REG_SIZES[i];
	}

	assert(false);
}
std::function<Uint8(Uint32)> Memory::getter8(MemoryRegion region) {
	return [this, region] (Uint32 address) -> Uint8 {
		return data[MEM_REG_INTERNAL_START[std::to_underlying(region)] + address];
	};
}

std::function<Uint16(Uint32)> Memory::getter16(MemoryRegion region) {
	return [this, region] (Uint32 address) -> Uint16 {
		size_t internal = MEM_REG_INTERNAL_START[std::to_underlying(region)] + address;
		return static_cast<Uint16>(data[internal]) << 8 | data[internal + 1];
	};
}

std::function<void(Uint32, Uint8)> Memory::setter8(MemoryRegion region) {
	return [this, region] (Uint32 address, Uint8 val) -> void {
		if (region == MemoryRegion::Sprites) spriteDataUpdated(address, false);
		if (region == MemoryRegion::Palette) paletteUpdated();
		data[MEM_REG_INTERNAL_START[std::to_underlying(region)] + address] = val;
	};
}

std::function<void(Uint32, Uint16)> Memory::setter16(MemoryRegion region) {
	return [this, region] (Uint32 address, Uint16 val) -> void {
		if (region == MemoryRegion::Sprites) spriteDataUpdated(address, true);
		if (region == MemoryRegion::Palette) paletteUpdated();
		data[MEM_REG_INTERNAL_START[std::to_underlying(region)] + address] = val >> 8;
		data[MEM_REG_INTERNAL_START[std::to_underlying(region)] + address + 1] = val & 0xff;
	};
}

Uint32 Memory::getInterrupt(Uint8 id) {
	size_t internalAddr = static_cast<Uint32>(id) * 4 + MEM_REG_INTERNAL_START[std::to_underlying(MemoryRegion::IVT)];

	return static_cast<Uint32>(data[internalAddr]) << 24 | static_cast<Uint32>(data[internalAddr + 1]) << 16 | static_cast<Uint32>(data[internalAddr + 2]) << 8 | static_cast<Uint32>(data[internalAddr + 2]);
}