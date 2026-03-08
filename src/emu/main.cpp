#include <iostream>
#include "tests/tests.h"
#include "graphics.h"
#include <ctime>

int main(int argc, char* argv[]) {
    srand(time(0));

    TEST_ALL();

    Window window("TEST", 1600, 900, 800, 450);
    ModifiableTexture t(&window, 800, 450);

    t.modify({0, 0, 800, 450},
        [](int x, int y) {return (x & 0xF) * 16;},
        [](int x, int y) {return (y & 0xF) * 16;},
        [](int x, int y) {return (Uint8)(rand() & 0xFF);}
    );

    while (window.tick()) {
        t.modify({33, 130, 32, 32},
            [](int x, int y) {return (x & 0xF) * 16;},
            [](int x, int y) {return (y & 0xF) * 16;},
            [](int x, int y) {return (Uint8)(rand() & 0xFF);}
        );

        window.clear();
        t.draw(0, 0);
    }

    return 0;
}