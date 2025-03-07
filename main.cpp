#include "cpu.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
// Function to load ROM into CPU memory
int main() {
    CPU cpu;
    cpu.memory.loadROM("Pokemon_Red.gb");


    while (true) {
        
        cpu.run();
    }


    return 0;
}