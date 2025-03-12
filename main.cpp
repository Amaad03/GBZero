
#include <iostream>
#include "cpu.h"
#include "memory.h"


int main() {
    Memory memory;
    CPU cpu(memory); 
    cpu.reset();

    cpu.memory.loadROM("cpu_instrs.gb");
    while (true) {

        cpu.executeNextInstruction();  

    }

    return 0;
}
