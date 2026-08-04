#include "tests/tests.h"
#include "screen.h"
#include "memory.h"
#include "CPU.h"
#include <SDL3/SDL.h>

//#define STEPTHROUGH

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

    m->spriteDataUpdated = [&](unsigned addr, bool l){
        w->reloadSpriteColor((0b11 << 22) & addr, {(int)(addr & 2047), (int)((addr & (2047 << 11)) >> 11), l ? 2 : 1, 1});
    };
    m->paletteUpdated = [&]() {w->reloadPalette();};

    auto file = SDL_IOFromFile("assets/rom.bin", "rb");

    //auto s = m->setter16(MemoryRegion::General);

    unsigned idx = 0;
    while (SDL_ReadU32BE(file, &idx)) {
        unsigned count = 0;
        SDL_ReadU32BE(file, &count);
        count += idx;

        for (; idx < count; idx+=2) {
            Uint16 v;
            SDL_ReadU16BE(file, &v);
            m->setter16()(idx, v);
        }
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
                std::cout << "CF -> " << ((c->carryFlag) ? "True"s : "False"s) << "\n";
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