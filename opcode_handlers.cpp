#include "opcode_handlers.h"
#include "cpu.h"  // Include the CPU class to access registers
#include <iostream>
// Define the opcode handler functions
void NOP(CPU& cpu) {
    std::cout << "[DEBUG] NOP executed" << std::endl;
    cpu.PC++;  // No operation; just increment the PC
    cpu.updateCycles(4);
    
}

void LD_BC_d16(CPU& cpu) { 
    uint16_t value = cpu.read16(cpu.PC + 1);// Read 2 bytes and combine them into a 16-bit value
    cpu.B = value >> 8;
    cpu.C = value & 0xFF;
    cpu.PC += 3;  // Move the program counter forward by 3 bytes (1 byte for opcode + 2 bytes for address)
    cpu.updateCycles(12);
}
void LD_BC_A(CPU& cpu) {
    uint16_t address = (cpu.B << 8) | cpu.C;  // Combine B and C to form the BC address
    cpu.memory[address] = cpu.A;  // Store the value of register A at the address
    cpu.PC += 1;  // Move the program counter forward by 1 byte (the opcode)
    cpu.updateCycles(8);  // This opcode takes 8 cycles
}


void INC_BC(CPU& cpu) {
   uint16_t bc = (cpu.B << 8) | cpu.C;
   bc += 1;
   cpu.B = (bc >> 8) & 0xFF; 
   cpu.C = bc & 0xFF;
   cpu.PC += 1;
   cpu.updateCycles(8);

}
void INC_B(CPU& cpu) {
    uint8_t b = cpu.B;
    cpu.B++;

    cpu.F = (cpu.B == 0) ? (cpu.F | 0x80) : (cpu.F & ~0x80);

    cpu.F &= ~0x40; 

    cpu.F = ((b & 0xF) + 1 > 0xF) ? (cpu.F | 0x20) : (cpu.F & ~0x20);
    cpu.PC++;
    cpu.updateCycles(4);
} 
void DEC_B(CPU& cpu) {
    
    cpu.B--;
    cpu.F = (cpu.B == 0) ? (cpu.F | 0x80) : (cpu.F & ~0x80);
    cpu.F = ((cpu.B & 0x0F) == 0x0) ? (cpu.F | 0x20) : (cpu.F & ~0x20);
    cpu.PC++;
    cpu.updateCycles(4);
}
void LD_B_d8(CPU& cpu) {
    uint8_t value = cpu.read(cpu.PC + 1);
    cpu.B = value;
    cpu.PC += 2;
    cpu.updateCycles(8);
}
void RLCA(CPU& cpu) {
    uint8_t carry = (cpu.A & 0x80) >> 7;
    cpu.A <<= 1;
    cpu.A |= carry;

    cpu.F &= 0x10; // Clear the N (subtract) flag
    cpu.F &= 0x20; // Clear the H (half carry) flag
    cpu.F |= (carry << 4); // Set the C (carry) flag to the old MSB value


    cpu.PC += 1;
    cpu.updateCycles(4);
}


void RST_38(CPU& cpu) {
    cpu.memory[cpu.SP - 1] = (cpu.PC >> 8) & 0xFF;
    cpu.memory[cpu.SP - 2] = cpu.PC & 0xFF;
    cpu.SP -= 2;
    cpu.PC = 0x0038;

    cpu.updateCycles(16);

}

void JP_a16(CPU& cpu) {
    uint16_t addr =  cpu.memory[cpu.PC + 1] | (cpu.memory[cpu.PC + 2] << 8);
    std::cout << "JP to address: 0x" << std::hex << addr << std::endl;

    cpu.PC = addr;

    cpu.updateCycles(16);


}

void POP_HL(CPU& cpu) {
    uint8_t low = cpu.memory[cpu.SP];     // Read low byte from stack
    cpu.SP++;  
    uint8_t high = cpu.memory[cpu.SP];    // Read high byte from stack
    cpu.SP++;

    cpu.H = high;  // Store into H register
    cpu.L = low;   // Store into L register

    cpu.updateCycles(12);  // POP HL takes 12 cycles

    cpu.PC += 1;


}

void POP_AF(CPU& cpu) {
    uint8_t low = cpu.memory[cpu.SP];
    cpu.SP++;
    uint8_t high = cpu.memory[cpu.SP];
    cpu.SP++;

    cpu.A = high;
    cpu.F = low & 0xF0;


    cpu.PC++;
    cpu.updateCycles(12);
}


void RETI(CPU& cpu) {



    
}