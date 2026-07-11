#include "tests/tests.h"
#include "graphics.h"
#include "memory.h"
#include "CPU.h"
#include <SDL3/SDL.h>

Memory* m;
CPU* c;
Window* w;

void printNumber() {
    std::cout << c->registers[15] << "\n";
}

int main(int argc, char* argv[]) {
    //TEST_ALL();

    m = new Memory();
    c = new CPU(m, printNumber);
    w = new Window("UNTITLED", 400, 300);
    w->setIcon("assets/icon.png");

    auto file = SDL_IOFromFile("assets/rom.bin", "rb");

    auto s = m->setter16(MemoryRegion::General);

    for (int i = 0; i < 200; i++) {
        Uint16 v = 0;
        SDL_ReadU16BE(file, &v);
        s(i * 2, v);
    }

    bool windowQuit = false;
    while (!c->finished && !windowQuit) {
        c->tick();
    }

    delete m;
    delete c;
    delete w;

    return 0;
}