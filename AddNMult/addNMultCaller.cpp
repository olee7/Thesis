#include <cstdint>
#include <iostream>

extern "C" std::int64_t addNMult();

int main() {
    std::cout << addNMult() << '\n';
}