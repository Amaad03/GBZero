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
    cycleCount = 0;


    std::fill(std::begin(memory), std::end(memory), 0);

    initOpcodeTable();
 

}

void CPU::initOpcodeTable() {
    std::fill(std::begin(opcodeTable), std::end(opcodeTable),  &UNIMPLEMENTED);  
    opcodeTable[0x00] = &NOP;
    opcodeTable[0x01] = &LD_BC_d16;
    opcodeTable[0x02] = &LD_BC_A;
    opcodeTable[0x03] = &INC_BC;
    opcodeTable[0x04] = &INC_B;
    opcodeTable[0x05] = &DEC_B;
    opcodeTable[0x06] = &LD_B_d8;
    opcodeTable[0x07] = &RLCA;
    opcodeTable[0xFF] = &RST_38;
    opcodeTable[0xC3] = &JP_a16;
    opcodeTable[0xE1] = &POP_HL;
    opcodeTable[0xF1] = &POP_AF;
    opcodeTable[0xD9] = &RETI;
    // Add more opcodes...


}


void UNIMPLEMENTED(CPU& cpu) {
    std::cerr << "Unimplemented opcode: 0x" << std::hex << static_cast<int>(cpu.memory[cpu.PC]) << std::endl;
    exit(1);
}

void CPU::executeOpcode(uint8_t opcode) {
    std::cout << "PC: 0x" << std::hex << PC 
        << " | Opcode: 0x" << static_cast<int>(opcode) << std::endl;

    if (opcodeTable[opcode]) {
        opcodeTable[opcode](*this);  // Pass 'this' to the handler function
    } else {
        std::cout << "Unknown opcode: " << std::hex << int(opcode) << std::endl;
        exit(1);
    }
    
}

uint8_t CPU::read(uint16_t addr) {
    if (addr >= 0x10000) {
        std::cerr << "Invalid memory access at address: 0x" << std::hex << addr << std::endl;
        exit(1);
    }
    return memory[addr];
}

uint16_t CPU::read16(uint16_t addr) {
    if (addr >= 0xFFFF) {
        std::cerr << "Invalid memory access at address: 0x" << std::hex << addr << std::endl;
        exit(1);
    }
    return memory[addr] | (memory[addr + 1] << 8);
}

void CPU::updateCycles(uint32_t cycles) {
    cycleCount += cycles;  // Increment total cycles by the amount specified by the opcode
}

