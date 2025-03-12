
#include <iostream>
#include "cpu.h"

#include "memory.h"
// Assume the PPU class and other code are already included.

int main() {
    Memory memory;
    CPU cpu(memory); // Your CPU object, assuming you have a CPU class
    cpu.reset();
    //memory.loadBootROM("dmg_boot.bin");
    cpu.memory.loadROM("cpu_instrs.gb");
    
    // Initialize SDL and create window, renderer, etc.

    

    while (true) {
        // Handle window events (like close button press)
        
    
        // Execute one CPU instruction cycle (or step, if you have such a function)
        cpu.executeNextInstruction();  // Or cpu.step() or whatever function you use for the CPU

    }

    return 0;
}
