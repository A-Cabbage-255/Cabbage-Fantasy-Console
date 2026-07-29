#include "tests/tests.h"
#include "screen.h"
#include "memory.h"
#include "CPU.h"
#include <SDL3/SDL.h>

#define STEPTHROUGH

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

    m->spriteDataUpdated = [&](){w->reloadSpriteColor();};
    m->paletteUpdated = [&]() {w->reloadPalette();};

    auto file = SDL_IOFromFile("assets/rom.bin", "rb");

    auto s = m->setter16(MemoryRegion::General);

    for (int i = 0; i < 200; i++) {
        Uint16 v = 0;
        SDL_ReadU16BE(file, &v);
        s(i * 2, v);
    }

    while (!c->finished && !windowQuit) {
#ifdef STEPTHROUGH
        std::cout << std::hex << m->getter16()(c->instPntr) << std::dec  << " :: ";

        std::string input;

        std::cin >> input;
        while (input != "n"s) {
            if (input == "r"s) {
                for (int i = 0; i < 16; i++) {
                    std::cout << "r" << i << " -> " << c->registers[i] << "\n";
                }
            } else if (input == "q"s) {
                windowQuit = true;
                break;
            }

            std::cout << " :: ";
            std::cin >> input;
        }
#endif
        c->tick();
    }

    delete m;
    delete c;
    delete w;

    return 0;
}