#include "cpu.h"
#include "opcode_handlers.h"
#include <stdio.h>
#include <iostream>


CPU::CPU() {
    A = B = C = D = E = H = L = 0;  // 8-bit registers
    F = 0;                          // Flag register, initialized to 0 (no flags set)
    
    // Initialize 16-bit registers
    PC = 0x100;                      // Program Counter starts at 0x100 (Game starts here)
    SP = 0xFFFE;
    


    for (int i = 0; i < 0x10000; i++) {
        memory[i] = 0;  // Optionally, you can initialize memory to zero (or some other state).
    }
    memory[0x0000] = 0xFE;  // Low byte of SP
    memory[0x0001] = 0xFF;  // High byte of SP
    initOpcodeTable();
 
    cycleCount = 0;
}

void CPU::initOpcodeTable() {
    std::fill(std::begin(opcodeTable), std::end(opcodeTable), nullptr);  
    opcodeTable[0x00] = NOP;
    opcodeTable[0x01] = LD_BC_d16;
    opcodeTable[0x02] = LD_BC_A;
    opcodeTable[0x03] = INC_BC;
    opcodeTable[0x04] = INC_B;
    opcodeTable[0x05] = DEC_B;
    opcodeTable[0x06] = LD_B_d8;
    opcodeTable[0x07] = RLCA;
    // Add more opcodes...
}

void CPU::executeOpcode(uint8_t opcode) {
    std::cout << "Executing opcode: " << std::hex << int(opcode) << std::endl;
    if (opcodeTable[opcode]) {
        opcodeTable[opcode](*this);  // Pass 'this' to the handler function
    } else {
        std::cout << "Unknown opcode: " << std::hex << int(opcode) << std::endl;
        exit(1);
    }
}

uint8_t CPU::read(uint16_t addr) {
    return memory[addr];  // Return the byte from the memory at the given address
}


uint16_t CPU::read16(uint16_t addr) {
    return memory[addr] | (memory[addr + 1] << 8);
}

void CPU::updateCycles(uint32_t cycles) {
    cycleCount += cycles;  // Increment total cycles by the amount specified by the opcode
}
