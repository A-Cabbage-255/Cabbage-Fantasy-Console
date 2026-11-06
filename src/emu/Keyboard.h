#pragma once
#include <string>
#include <cstdint>
#include <deque>

namespace Keyboard {
	void keyDown(uint8_t scancode);
	void keyUp(uint8_t scancode);
	void keyType(std::string unicode);

	void tick();
}