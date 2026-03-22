#include "tests/tests.h"
#include "screen.h"
#include "memory.h"

int main(int argc, char* argv[]) {
    TEST_ALL();

    Memory mem;

    Screen scr(
        mem.getter8(MemoryRegion::Sprites),
        mem.getter8(MemoryRegion::SpriteFlags),
        mem.getter8(MemoryRegion::SpriteData),
        mem.getter8(MemoryRegion::Palette)
    );

    while(scr.tick());

    return 0;
}