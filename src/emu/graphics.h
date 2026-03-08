#pragma once
#include "../common.h"

struct IntRect {
	int x;
	int y;
	int w;
	int h;
};

typedef struct IntRect TextureRegion;

class Window {
protected:
	void* win;
	void* rend;
public:
	static unsigned numWindows;

	Window(std::string title, int w, int h);
	Window(std::string title, int w, int h, int logicalw, int logicalh);
	~Window();

	void clear(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);

	bool tick(); //returns true if you should continue runnning

	friend class Texture;
	friend class ModifiableTexture;
};

class Texture {
protected:
	void* tex;
	Window* win;
	int w;
	int h;
public:
	void draw(float x, float y);
};

//Pixel format ABGR 8888??
class ModifiableTexture : public Texture {
public:
	ModifiableTexture(Window* parent, int w, int h);
	~ModifiableTexture();

	Uint8* lockRegion(TextureRegion reg, int* pitch_OUT = nullptr);
	void unlock();

	void modify(TextureRegion reg, std::function<Uint8(int x, int y)> r, std::function<Uint8(int x, int y)> g, std::function<Uint8(int x, int y)> b);
};