#include "memory.h"

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

	return static_cast<Uint32>(data[internalAddr]) << 24 | static_cast<Uint32>(data[internalAddr + 1]) << 16 | static_cast<Uint32>(data[internalAddr + 2]) << 8 | static_cast<Uint32>(data[internalAddr + 3]);
}