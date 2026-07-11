#include "tests/tests.h"
#include "screen.h"
#include "memory.h"
#include "CPU.h"
#include <SDL3/SDL.h>

Memory* m;
CPU* c;

void printNumber() {
    std::cout << c->registers[15] << "\n";
}

int main(int argc, char* argv[]) {
    //TEST_ALL();

    m = new Memory();
    c = new CPU(m, printNumber);

    auto file = SDL_IOFromFile("assets/rom.bin", "rb");

    auto s = m->setter16(MemoryRegion::General);

    for (int i = 0; i < 200; i++) {
        Uint16 v = 0;
        SDL_ReadU16BE(file, &v);
        s(i * 2, v);
    }

    while (!c->finished) {
        c->tick();
    }

    return 0;
}