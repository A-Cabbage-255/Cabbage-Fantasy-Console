#include <iostream>
#include "tests/tests.h"
#include "screen.h"
#include "memory.h"

int main(int argc, char* argv[]) {
    TEST_ALL();

    auto clr = [](Uint32 i) {
        Uint8 x = i % 8;
        Uint8 y = i / 8;
        OUT_Log("I: " + std::to_string(i) + "\n");
        OUT_Log("X: " + std::to_string(x) + "\n");
        OUT_Log("Y: " + std::to_string(y) + "\n");
        if ((x == 2 || x == 5) && (y == 1 || y == 2)) return (Uint8)255;
        if (y == 5 && x != 0 && x != 7) return (Uint8)255;
        if (y == 4 && (x == 0 && x == 7)) return (Uint8)255;
        return (Uint8)0;
    };
    auto flg = [](Uint32 i) {
        if (i == 0) {
            return (Uint8)0b10000000;
        }
        return (Uint8)0b00000000;
    };
    auto dat = [](Uint32 i) {
        return (Uint8)0b00000000;
    };
    auto pal = [](Uint32 i) {
        return (Uint8)i;
    };

    Screen scr(clr, flg, dat, pal);

    while (scr.tick()) {
        OUT_Log("--------------TICK\n");
    }

    return 0;
}