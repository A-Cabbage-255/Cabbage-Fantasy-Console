#include "Keyboard.h"

extern bool keyIntLine;
extern uint8_t intDataLine;

std::deque<uint8_t> keybout;

void Keyboard::keyDown(uint8_t scancode) {
	if (scancode <= 0x01 || scancode == 0xff) return;
	keybout.push_back(scancode);
}

void Keyboard::keyUp(uint8_t scancode) {
	if (scancode <= 0x01 || scancode == 0xff) return;
	keybout.push_back(0xFF);
	keybout.push_back(scancode);
}

void Keyboard::keyType(std::string unicode) {
	keybout.push_back(0x01);
	for (int8_t c : unicode) {
		keybout.push_back(c);
	}
}

void Keyboard::tick() {
	if (!keyIntLine && keybout.size() > 0) {
		keybout.pop_front(); //TODO SEND TO CPU??
		keyIntLine = true;
	}
}