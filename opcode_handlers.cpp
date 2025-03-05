/*

left shift operator <<
- shifts the bits of a number to the left by a specified number of postions
    - each shift to the left effectively multiples the number by 2
    - zeros are shifted into the lower bits as the number shifts

5 << 1 will shift the bits to the left by 1 position:
0000 0101 (5)
<< 1
---------
0000 1010 (10)


 Right Shift Operator (>>)

The right shift operator shifts the bits of a number to the right by a specified number of positions.
Each shift to the right effectively divides the number by 2 (with integer division).
For unsigned numbers, zeros are shifted into the higher bits.
For signed numbers (in some programming languages), the sign bit (the leftmost bit) might be shifted in to 
preserve the sign of the number (this is called "arithmetic shift").

20 >> 1 will shift the bits to the right by 1 position:
0001 0100 (20)
>> 1
---------
0000 1010 (10)
*/



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
    cpu.writeMemory(address, cpu.A);// Store the value of register A at the address
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
    cpu.F |= 0x40;  
    cpu.PC++;
    cpu.updateCycles(4);
}
void LD_B_d8(CPU& cpu) {
    uint8_t value = cpu.readMemory(cpu.PC + 1);
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


void LD_a16_SP(CPU& cpu) {  // 0x08
    uint16_t addr =  cpu.read16(cpu.PC + 1); 
    cpu.write16(addr, cpu.SP);
    cpu.PC += 3;
    cpu.updateCycles(20);

}
void ADD_HL_BC(CPU& cpu) { //0x09

    uint16_t bc = (cpu.B << 8) | cpu.C;
    uint16_t hl = (cpu.H << 8) | cpu.L;

    uint16_t result = hl + bc;
    cpu.H = (result >> 8) & 0xFF; // these are the upper bits
    cpu.L = result & 0xFF; // lower bits
    
    cpu.F &= ~0x80;  // Zero flag unaffected
    cpu.F &= ~0x40;  // Subtract flag (N) cleared
    cpu.F = (((hl & 0x0FFF) + (bc & 0x0FFF)) > 0x0FFF) ? (cpu.F | 0x20) : (cpu.F & ~0x20);  // Half carry flag
    cpu.F = (result < hl) ? (cpu.F | 0x10) : (cpu.F & ~0x10);  // Carry flag

    cpu.PC++;
    cpu.updateCycles(8);
}


void LD_A_BC(CPU& cpu) { //0x0A
    uint16_t address = (cpu.B << 8) | cpu.C;
    cpu.A = cpu.readMemory(address);
    cpu.PC += 1;
    cpu.updateCycles(8);


}

void DEC_BC(CPU& cpu) { //0x0B
    uint16_t bc = (cpu.B << 8) | cpu.C; 
    bc--;
    cpu.B = (bc >> 8) & 0xFF;
    cpu.C = bc & 0xFF;
    cpu.PC++;
    cpu.updateCycles(8);

}
void INC_C(CPU& cpu) { //0x0C
    cpu.C++;

    //zero flag
    cpu.F = (cpu.C == 0) ? (cpu.F | 0x80) : (cpu.F & ~0x80);
    //half carry flag
    cpu.F = ((cpu.C & 0xF) + 1 > 0xF) ? (cpu.F | 0x20) : (cpu.F & ~0x20);
    //Subtract flag (N) 
    cpu.F = ~0x40;// Clear the N flag

    cpu.PC++;
    cpu.updateCycles(4);

}
void DEC_C(CPU& cpu) { //0x0D
    cpu.C--;
    
    cpu.F = (cpu.C == 0) ? (cpu.F | 0x80) : (cpu.F & ~0x80);
    cpu.F = ((cpu.C & 0x0F) == 0x0F) ? (cpu.F | 0x20) : (cpu.F & ~0x20);
    cpu.F = cpu.F | 0x40;


    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_C_n8(CPU& cpu) { //0x0E
    cpu.C = cpu.readMemory(cpu.PC + 1);

    cpu.PC += 2;
    cpu.updateCycles(8);

    
}

void RRCA(CPU& cpu) { //0x0F
    bool carryBit = cpu.F & 0x10;  // Get the carry flag from the F register
    cpu.F = (cpu.F & ~0x10);  // Clear the carry flag before we rotate

    // Shift A register right and insert the carry bit in the MSB
    cpu.F |= (cpu.A & 0x01) ? 0x10 : 0;  // Set carry flag if LSB of A is 1
    cpu.A = (cpu.A >> 1) | (carryBit ? 0x80 : 0); // Perform the right shift with the carry bit

    // Zero flag is cleared as it's not affected by RRCA
    cpu.F &= ~0x80;  // Clear zero flag (Z) for this operation

    // Half carry flag is unaffected
    cpu.F &= ~0x20;  // Clear half carry flag (H)

    // Subtract flag is unaffected
    cpu.F &= ~0x40;  // Clear subtract flag (N)

    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(4);  // RRCA takes 4 cycles
}

void RST_38(CPU& cpu) {
    cpu.pushToStack(cpu.PC);
    cpu.PC = 0x0038;
    cpu.updateCycles(16);
}

void JP_a16(CPU& cpu) {
    uint16_t addr = cpu.read16(cpu.PC +1 );
    std::cout << "JP to address: 0x" << std::hex << addr << std::endl;

    cpu.PC = addr;

    cpu.updateCycles(16);


}

void POP_HL(CPU& cpu) {
    uint16_t value = cpu.popFromStack();  // Use the new popFromStack function
    cpu.H = (value >> 8) & 0xFF;         // High byte
    cpu.L = value & 0xFF;                // Low byte
    cpu.PC += 1;                         // Increment program counter
    std::cout << "[DEBUG] POP_HL executed. HL: 0x" << std::hex << value << std::endl;
    cpu.updateCycles(12);
}

void POP_AF(CPU& cpu) {

    uint16_t value = cpu.popFromStack();
    cpu.A = (value >> 8) & 0xFF;
    cpu.F = value & 0xF0;

    cpu.PC++;
    std::cout << "[DEBUG] POP_AF executed. AF: 0x" << std::hex << (uint16_t(cpu.A) << 8 | cpu.F) << std::endl;
    cpu.updateCycles(12);
}


void LD_D_B(CPU& cpu) {
    cpu.D = cpu.B;  // Load the value of register B into register D
    cpu.PC++;
    cpu.updateCycles(4);  // Adjust the cycle count based on the instruction
}

void RETI(CPU& cpu) {
    std::cout << "[DEBUG] Stack before RETI: SP: 0x" << std::hex << cpu.SP << std::endl;
    
    uint16_t returnAddr = cpu.popFromStack();
    cpu.PC = returnAddr;

    cpu.enableInterrupts();  // Enable interrupts

    std::cout << "[DEBUG] RETI executed. Returning to: 0x" << std::hex << cpu.PC << std::endl;

    cpu.updateCycles(16);  // Correct cycle count for RETI
}