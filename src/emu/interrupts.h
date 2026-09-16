#pragma once
#include "../common.h"

enum class IntID : Uint8 {
	Power = 0x00,
	Exeption = 0x01,
	Timer = 0x02,
	Breakpoint = 0x03,

	Connect_Keyboard = 0x10,

	Disconnect_Keyboard = 0x20,

	Input_Keyboard = 0x30,

	Addr_Usr_Mem_Lower = 0xE0,
	Addr_Usr_Mem_Higher = 0xE1,
	Addr_Keyb_Input = 0xE2,

	Ill_Interrupt = 0xFD,
	Draw = 0xFE,
	Ill_Command = 0xFF
};

inline bool operator==(IntID a, Uint8 b) {
	return std::to_underlying(a) == b;
}