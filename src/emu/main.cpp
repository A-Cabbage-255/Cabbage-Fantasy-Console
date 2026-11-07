#include "tests/tests.h"
#include "screen.h"
#include "memory.h"
#include "CPU.h"
#include "PIC.h"
#include <SDL3/SDL.h>
#include "Keyboard.h"

#define STEPTHROUGH

Memory* m;
CPU* c;
Screen* w;
InterruptController* pic;
uint8_t intDataLine = 0xff;
bool keyIntLine = false;
bool windowQuit = false;
#ifdef STEPTHROUGH
    bool cont = false;
#endif

void sendKeyByte_(Uint8 byte) {
    //c->interrupt(IntID::Input_Keyboard);
    keyIntLine = true;
}
std::function<void(Uint8)> sendKeyByte = sendKeyByte_;

void updWin() {
    windowQuit = !w->tick();
}

int main(int argc, char* argv[]) {
    m = new Memory();
    c = new CPU(m, updWin);
    w = new Screen(m->getter8(MemoryRegion::Sprites), m->getter8(MemoryRegion::SpriteFlags), m->getter8(MemoryRegion::SpriteData), m->getter8(MemoryRegion::Palette));
    pic = new InterruptController([&](IntID v) {
        c->interrupt(v);
        #ifdef STEPTHROUGH
        cont = false;
        #endif
    });
    pic->connect(0, &c->stallintline);
    pic->connect(1, &keyIntLine);
    //w->setIcon("assets/icon.png");

    m->spriteDataUpdated = [&](unsigned addr, bool l){
        w->reloadSpriteColor(((0b11 << 22) & addr) >> 22, {(int)(addr & 2047), (int)((addr & (2047 << 11)) >> 11), l ? 2 : 1, 1});
    };
    m->paletteUpdated = [&]() {
        w->reloadPalette();
    };

    auto file = SDL_IOFromFile((argc >= 2) ? argv[1] : "assets/rom.bin", "rb");

    unsigned idx = 0;
    while (SDL_ReadU32BE(file, &idx)) {
        unsigned count = 0;
        SDL_ReadU32BE(file, &count);

        std::cout << std::hex << "idx " << idx << " and count " << count << "\n" << std::dec;

        count += idx;

        for (; idx < count; idx++) {
            Uint8 v;
            SDL_ReadU8(file, &v);
            m->setter8(idx, v);
        }
    }

    while (!c->finished && !windowQuit) {
#ifdef STEPTHROUGH
        if (!cont && argc >= 3 && (std::string)argv[2] == "debug") {
            std::cout << ":" << c->instPntr << " - " << std::hex << m->getter16(c->instPntr) << std::dec  << " :: ";

            std::string input;

            std::cin >> input;
            //std::cout << "\n";
            while (input != "n"s) {
                if (input == "r"s) {
                    for (int i = 0; i < 16; i++) {
                        std::cout << "r" << i << " -> " << c->registers[i] << "\n";
                    }
                } else if (input == "q"s) {
                    windowQuit = true;
                    break;
                } else if (input == "c"s) {
                    cont = true;
                    break;
                }

                std::cout << " :: ";
                std::cin >> input;
            }
        }
#endif
        c->tick();
        pic->tick();
        Keyboard::tick();
    }

    delete pic;
    delete m;
    delete c;
    delete w;

    return 0;
}