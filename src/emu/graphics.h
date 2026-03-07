#pragma once
#include "../common.h"

class Window {
private:
	void* win;
	void* rend;
public:
	static unsigned numWindows;

	Window(std::string title, int w, int h);
	Window(std::string title, int w, int h, int logicalw, int logicalh);
	~Window();
};