#include "cpu.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
// Function to load ROM into CPU memory
int main() {
    CPU cpu;
    cpu.memory.loadROM("Red.gb");
    cpu.dumpROMHeader();


    while (true) {
        
        cpu.executeNextInstruction();
    }
        return 0;
}