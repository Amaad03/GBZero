#include "cpu.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
// Function to load ROM into CPU memory
int main() {
    CPU cpu;
    cpu.memory.loadROM("Pokemon_Red.gb");
    uint16_t prevPC = cpu.PC;
    int cyclesWithoutProgress = 0;

    while (true) {
        
        cpu.executeNextInstruction();
        if (cpu.PC == prevPC) {
                cyclesWithoutProgress++;
                if (cyclesWithoutProgress > 100) {
                    std::cerr << "[ERROR] Infinite loop detected at PC: " << std::hex << cpu.PC << std::endl;
                    break;
                }
            } else {
                cyclesWithoutProgress = 0;
            }

            prevPC = cpu.PC;
        }
        return 0;
}