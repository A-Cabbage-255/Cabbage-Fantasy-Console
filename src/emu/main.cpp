#include "tests/tests.h"
#include "screen.h"
#include "memory.h"
#include "CPU.h"
#include <SDL3/SDL.h>

Memory* m;
CPU* c;
Screen* w;
bool windowQuit = false;

void updWin() {
    windowQuit = !w->tick();
}

int main(int argc, char* argv[]) {
    //TEST_ALL();

    m = new Memory();
    c = new CPU(m, updWin);
    w = new Screen(m->getter8(MemoryRegion::Sprites), m->getter8(MemoryRegion::SpriteFlags), m->getter8(MemoryRegion::SpriteData), m->getter8(MemoryRegion::Palette));
    //w->setIcon("assets/icon.png");

    auto file = SDL_IOFromFile("assets/rom.bin", "rb");

    auto s = m->setter16(MemoryRegion::General);

    for (int i = 0; i < 200; i++) {
        Uint16 v = 0;
        SDL_ReadU16BE(file, &v);
        s(i * 2, v);
    }

    while (!c->finished && !windowQuit) {
        c->tick();
    }

    delete m;
    delete c;
    delete w;

    return 0;
}