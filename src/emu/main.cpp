#include "tests/tests.h"
#include "screen.h"
#include "memory.h"
#include "CPU.h"

int main(int argc, char* argv[]) {
    TEST_ALL();

    CPU c(nullptr);

    std::cout << "A ";
    unsigned a, b;
    std::cin >> a;
    std::cout << "B ";
    std::cin >> b;
    c.registers[1] = a;
    c.registers[2] = b;
    c.execIns(0b0000001100010010);
    std::cout << "C: " << (unsigned)c.registers[3];

    return 0;
}