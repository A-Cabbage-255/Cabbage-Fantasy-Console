#include "tests/tests.h"
#include "screen.h"
#include "memory.h"
#include "CPU.h"

Memory* m;
CPU* c;

void printNumber() {
    std::cout << c->registers[15] << "\n";
}

int main(int argc, char* argv[]) {
    //TEST_ALL();

    m = new Memory();
    c = new CPU(m, printNumber);
    
    uint16_t prog[] = {
        0b1110010110110101,
        0b1110010000001000,
        0b0110010001010100,
        0b1110010100100000,
        0b0000010001010100,
        0b1110000100000000,
        0b1110001000000001,
        0b0000111100000001,
        0b1100100011111110,
        0b0000001100010010,
        0b0000000100100000,
        0b0000001000110000,
        0b1110011000001110,
        0b1110011100000100,
        0b0011100000010100,
        0b1000000010000111,
        0b1010000000000110,
        0b0000111100000001,
        0b1100100011111110,
        0b1100100000000000,
    };

    auto s = m->setter16(MemoryRegion::General);

    for (int i = 0; i < 20; i++) {
        s(i * 2, prog[i]);
    }

    while (!c->finished) {
        c->tick();
    }

    return 0;
}