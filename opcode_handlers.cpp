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


void STOP_n8(CPU& cpu) {



    cpu.PC += 2;
    cpu.updateCycles(4);
}


void LD_DE_n16(CPU& cpu) {
    uint16_t value = cpu.read16(cpu.PC + 1);
    cpu.D = (value >> 8) & 0xFF;
    cpu.E = value & 0xFF;



    cpu.PC += 3;
    cpu.updateCycles(12);
}

void LD_DE_A(CPU& cpu) {
    uint16_t address = (cpu.D << 8) | cpu.E;
    cpu.writeMemory(address, cpu.A);
    cpu.PC++;
    cpu.updateCycles(8);
}

void INC_DE(CPU& cpu) {
    uint16_t de = (cpu.D << 8) | cpu.E;
    de++;
    cpu.D = (de >> 8) & 0xFF;
    cpu.E = de & 0xFF;
    cpu.PC++;
    cpu.updateCycles(8);
}

void INC_D(CPU& cpu) {
    cpu.D++;
    cpu.F = (cpu.D == 0) ? (cpu.F | 0x80) : (cpu.F & ~0x80);
    cpu.F &= ~0x40; 
    cpu.F = ((cpu.D & 0xF) + 1 > 0xF) ? (cpu.F | 0x20) : (cpu.F & ~0x20);
    cpu.PC++;
    cpu.updateCycles(8);
}

void DEC_D(CPU& cpu) {
    cpu.D--;
    cpu.F = (cpu.B == 0) ? (cpu.F | 0x80) : (cpu.F & ~0x80);
    cpu.F = ((cpu.B & 0x0F) == 0x0) ? (cpu.F | 0x20) : (cpu.F & ~0x20);
    cpu.F |= 0x40;  
    cpu.PC++;
    cpu.updateCycles(4);

}


void LD_D_n8(CPU& cpu) {
    uint8_t value = cpu.readMemory(cpu.PC + 1);
    cpu.D = value;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RLA(CPU& cpu) {
    uint8_t carry = (cpu.F & 0x10) >> 4;
    uint8_t result = (cpu.A << 1) | carry;
    cpu.F = (cpu.A & 0x80) ? 0x10 : 0x00;  
    cpu.A = result;
    cpu.PC++;
    cpu.updateCycles(4);
}

void JR_e8(CPU& cpu) {
    int8_t offset = static_cast<int8_t>(cpu.readMemory(cpu.PC + 1));
    cpu.PC += 2 + offset; 
    cpu.updateCycles(12);
}

void ADD_HL_DE(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L; 
    uint16_t de = (cpu.D << 8) | cpu.E; 
    uint16_t result = hl + de;
    if ((hl & 0x0FFF) + (de & 0x0FFF) > 0x0FFF) {
        cpu.F |= 0x20;  // Set the H flag (bit 5 in F register)
    } else {
        cpu.F &= ~0x20;  // Clear the H flag
    }

    // Set the Carry flag (C) if there is a carry from bit 15
    if (result > 0xFFFF) {
        cpu.F |= 0x10;  // Set the C flag (bit 4 in F register)
    } else {
        cpu.F &= ~0x10;  // Clear the C flag
    }

    // Store the result in HL
    cpu.H = (result >> 8) & 0xFF;  
    cpu.L = result & 0xFF; 
    // Update the program counter by 1 to skip the opcode
    cpu.PC += 1;

    // Update cycles for the instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}

void LD_A_DE(CPU& cpu) {
    uint16_t address = (cpu.D << 8) | cpu.E;

    cpu.A = cpu.readMemory(address);
    cpu.PC++;
    cpu.updateCycles(8);

}

void DEC_DE(CPU& cpu) {
    uint16_t de = (cpu.D << 8) | cpu.E;
    de--;

    cpu.D = (de >> 8) & 0xFF;
    cpu.E = de & 0xFF;
    cpu.PC++;
    cpu.updateCycles(8);

}

void INC_E(CPU& cpu) {
    cpu.E++;

    cpu.F = (cpu.E == 0) ? (cpu.F | 0x80) : (cpu.F & ~0x80);
    //half carry flag
    cpu.F = ((cpu.E & 0xF) + 1 > 0xF) ? (cpu.F | 0x20) : (cpu.F & ~0x20);
    //Subtract flag (N) 
    cpu.F = ~0x40;// Clear the N flag


    cpu.PC++;
    cpu.updateCycles(8);
}

void DEC_E(CPU& cpu) {
    cpu.E--;
    cpu.F = (cpu.E == 0) ? (cpu.F | 0x80) : (cpu.F & ~0x80);
    cpu.F = ((cpu.E & 0x0F) == 0x0F) ? (cpu.F | 0x20) : (cpu.F & ~0x20);
    cpu.F = cpu.F | 0x40;

    cpu.PC++;
    cpu.updateCycles(4);

}

void LD_E_n8(CPU& cpu) {
    cpu.E = cpu.readMemory(cpu.PC + 1);

    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RRA(CPU& cpu) {
    bool carry = cpu.F & 0x10;  // C flag is bit 4 in the F register
    uint8_t oldA = cpu.A;
    cpu.A = (cpu.A >> 1) | (carry << 7);  // Shift right and place the old carry in the MSB

    if (oldA & 0x01) {
        cpu.F |= 0x10;  // Set the C flag if the least significant bit of A was 1
    } else {
        cpu.F &= ~0x10; // Clear the C flag if the least significant bit of A was 0
    }

    cpu.F &= ~(0x20 | 0x40); // Clear the H and N flags
    cpu.PC++;
    cpu.updateCycles(4);
}


void LD_HL_n16(CPU& cpu) {
    uint16_t value = cpu.read16(cpu.PC + 1);  // Read 16-bit value from memory
    cpu.H = (value >> 8) & 0xFF;  // Set high byte of HL
    cpu.L = value & 0xFF;         // Set low byte of HL
    cpu.PC += 3;  // Move program counter to the next instruction
    cpu.updateCycles(12);  // 12 cycles for this instruction
}
void LD_HLplus_A(CPU& cpu) {
    uint16_t address = (cpu.H << 8) | cpu.L;  // Get the address from HL
    cpu.writeMemory(address, cpu.A);           // Write A to memory at address HL
    cpu.L++;                                  // Increment HL
    if (cpu.L == 0) cpu.H++;                  // Handle overflow from L to H
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}

void INC_H(CPU& cpu) {
    cpu.H++;
    cpu.F = (cpu.H == 0) ? (cpu.F | 0x80) : (cpu.F & ~0x80);
    cpu.F &= ~0x40; 
    cpu.F = ((cpu.H & 0xF) + 1 > 0xF) ? (cpu.F | 0x20) : (cpu.F & ~0x20);
    cpu.PC++;
    cpu.updateCycles(4);
}



void DEC_H(CPU& cpu) {
    cpu.H--;  // Decrement the H register
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(4);  // 4 cycles for this instruction
}
void LD_H_n8(CPU& cpu) {
    cpu.H = cpu.readMemory(cpu.PC + 1);  // Load immediate 8-bit value into H
    cpu.PC += 2;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}
void DAA(CPU& cpu) {
    if (cpu.F & 0x10) {  // If carry flag is set
        if ((cpu.A & 0x0F) > 9) {
            cpu.A += 6;  // Adjust for BCD
        }
    }

    if (cpu.F & 0x80) {  // If zero flag is set
        cpu.A += 6;  // Adjust for BCD
    }

    // Update the Zero, Negative, Half Carry, and Carry flags
    if (cpu.A == 0) {
        cpu.F |= 0x80;  // Set Z flag
    } else {
        cpu.F &= ~0x80;  // Clear Z flag
    }

    if ((cpu.A & 0x10) > 0xF) {
        cpu.F |= 0x10;  // Set the C flag (carry)
    }

    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(4);  // 4 cycles for this instruction
}
void JRZ_e8(CPU& cpu) {
    if (cpu.F & 0x80) {  // Check if the Zero flag (Z) is set
        int8_t offset = cpu.readMemory(cpu.PC + 1);  // Get the signed offset
        cpu.PC += offset;  // Jump by the offset
        cpu.updateCycles(12);  // 12 cycles for this instruction
    } else {
        cpu.PC += 2;  // Skip the offset if Zero flag is not set
        cpu.updateCycles(8);  // 8 cycles for this instruction
    }
}
void ADD_HL_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Get the HL register value
    uint16_t result = hl + hl;           // Add HL to itself

    // Set the carry flag (C) if there is a carry from bit 15
    if (result > 0xFFFF) {
        cpu.F |= 0x10;  // Set carry flag
    } else {
        cpu.F &= ~0x10;  // Clear carry flag
    }

    // Set the half-carry flag (H) if there is a carry from bit 11
    if ((hl & 0x0FFF) + (hl & 0x0FFF) > 0x0FFF) {
        cpu.F |= 0x20;  // Set half-carry flag
    } else {
        cpu.F &= ~0x20;  // Clear half-carry flag
    }

    // Store the result back into HL
    cpu.H = (result >> 8) & 0xFF;
    cpu.L = result & 0xFF;

    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}
void LD_A_HLplus(CPU& cpu) {
    uint16_t address = (cpu.H << 8) | cpu.L;  // Get the address from HL
    cpu.A = cpu.readMemory(address);  // Load memory value at address into A
    cpu.L++;                          // Increment L
    if (cpu.L == 0) cpu.H++;          // Handle overflow from L to H
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}

void INC_L(CPU& cpu) {
    cpu.L++;  // Increment the L register
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(4);  // 4 cycles for this instruction
}
void DEC_L(CPU& cpu) {
    cpu.L--;  // Decrement the L register
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(4);  // 4 cycles for this instruction
}
void LD_L_n8(CPU& cpu) {
    cpu.L = cpu.readMemory(cpu.PC + 1);  // Load immediate 8-bit value into L
    cpu.PC += 2;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}
void CPL(CPU& cpu) {
    cpu.A = ~cpu.A;  // Complement (invert) all bits of A
    cpu.F |= 0x20;   // Set the N flag (negative flag)
    cpu.PC+=2;
    cpu.updateCycles(4);
}

void JR_NZ_e8(CPU& cpu) {
    // Check if Zero flag (Z) is not set
    if (!(cpu.F & 0x80)) {
        int8_t offset = cpu.readMemory(cpu.PC + 1);  // Get the signed 8-bit offset
        cpu.PC += offset;  // Jump to the new address
        cpu.updateCycles(12);  // 12 cycles for this instruction
    } else {
        cpu.PC += 2;  // Skip the offset if Zero flag is set
        cpu.updateCycles(8);  // 8 cycles for this instruction
    }
}

void LD_SP_n16(CPU& cpu) {
    uint16_t value = cpu.read16(cpu.PC + 1);  // Read 16-bit immediate value
    cpu.SP = value;  // Set SP register
    cpu.PC += 3;  // Move program counter to the next instruction
    cpu.updateCycles(12);  // 12 cycles for this instruction
}


void LD_HL_minus_A(CPU& cpu) {
    uint16_t address = (cpu.H << 8) | cpu.L;  // Get the address from HL
    cpu.writeMemory(address, cpu.A);           // Write A to memory at address HL
    cpu.L--;                                  // Decrement HL
    if (cpu.L == 0xFF) cpu.H--;                // Handle overflow from L to H
    cpu.PC++;                                  // Move program counter to the next instruction
    cpu.updateCycles(8);                       // 8 cycles for this instruction
}

void INC_SP(CPU& cpu) {
    cpu.SP++;  // Increment SP
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}
void INC_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L into a 16-bit value
    hl++;                                 // Increment HL
    cpu.H = (hl >> 8) & 0xFF;             // Update H with the high byte of HL
    cpu.L = hl & 0xFF;                    // Update L with the low byte of HL
    cpu.PC++;                              // Move program counter to the next instruction
    cpu.updateCycles(8);                   // 8 cycles for this instruction
}


void DEC_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L into a 16-bit value
    hl--;                                 // Decrement HL
    cpu.H = (hl >> 8) & 0xFF;             // Update H with the high byte of HL
    cpu.L = hl & 0xFF;                    // Update L with the low byte of HL
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}
void LD_HL_n8(CPU& cpu) {
    uint8_t value = cpu.readMemory(cpu.PC + 1);  // Read immediate 8-bit value
    cpu.H = (value >> 4) & 0xF0;                  // Set high nibble of H
    cpu.L = value & 0x0F;                         // Set low nibble of L
    cpu.PC += 2;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}

void SCF(CPU& cpu) {
    cpu.F |= 0x10;  // Set the Carry flag (C)
    cpu.F &= ~0x20; // Clear the Half Carry flag (H)
    cpu.F &= ~0x40; // Clear the Negative flag (N)
    cpu.PC++;       // Move program counter to the next instruction
    cpu.updateCycles(4);  // 4 cycles for this instruction
}

void JR_C_e8(CPU& cpu) {
    if (cpu.F & 0x10) {  // Check if Carry flag (C) is set
        int8_t offset = cpu.readMemory(cpu.PC + 1);  // Get the signed 8-bit offset
        cpu.PC += offset;  // Jump to the new address
        cpu.updateCycles(12);  // 12 cycles for this instruction
    } else {
        cpu.PC += 2;  // Skip the offset if Carry flag is not set
        cpu.updateCycles(8);  // 8 cycles for this instruction
    }
}
void ADD_HL_SP(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Get HL as a 16-bit value
    uint16_t result = hl + cpu.SP;       // Add SP to HL

    // Set the carry and half-carry flags
    if ((hl & 0x0FFF) + (cpu.SP & 0x0FFF) > 0x0FFF) {
        cpu.F |= 0x20;  // Set Half Carry flag
    } else {
        cpu.F &= ~0x20;  // Clear Half Carry flag
    }

    if (result > 0xFFFF) {
        cpu.F |= 0x10;  // Set Carry flag
    } else {
        cpu.F &= ~0x10;  // Clear Carry flag
    }

    // Store the result in HL
    cpu.H = (result >> 8) & 0xFF;
    cpu.L = result & 0xFF;

    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}

void LD_A_HL_minus(CPU& cpu) {
    uint16_t address = (cpu.H << 8) | cpu.L;  // Get the address from HL
    cpu.A = cpu.readMemory(address);           // Load memory value at address into A
    cpu.L--;                                  // Decrement L
    if (cpu.L == 0xFF) cpu.H--;                // Handle overflow from L to H
    cpu.PC++;                                  // Move program counter to the next instruction
    cpu.updateCycles(8);                       // 8 cycles for this instruction
}

void DEC_SP(CPU& cpu) {
    cpu.SP--;  // Decrement SP
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}

void INC_A(CPU& cpu) {
    cpu.A++;  // Increment the A register
    cpu.F &= ~0x80;  // Clear the Zero flag (Z)
    if (cpu.A == 0) {
        cpu.F |= 0x80;  // Set Zero flag (Z) if A is zero
    }
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(4);  // 4 cycles for this instruction
}

void DEC_A(CPU& cpu) {
    cpu.A--;  // Decrement the A register
    cpu.F |= 0x40;  // Set the Negative flag (N)
    if (cpu.A == 0) {
        cpu.F |= 0x80;  // Set Zero flag (Z) if A is zero
    }
    cpu.PC++;  // Move program counter to the next instruction
    cpu.updateCycles(4);  // 4 cycles for this instruction
}

void LD_A_n8(CPU& cpu) {
    cpu.A = cpu.readMemory(cpu.PC + 1);  // Load immediate 8-bit value into A
    cpu.PC += 2;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}

void CCF(CPU& cpu) {
    cpu.F ^= 0x10;  // Toggle the Carry flag (C)
    cpu.F &= ~0x20; // Clear the Half Carry flag (H)
    cpu.F &= ~0x40; // Clear the Negative flag (N)
    cpu.PC++;       // Move program counter to the next instruction
    cpu.updateCycles(4);  // 4 cycles for this instruction
}



void LD_B_B(CPU& cpu) {
    cpu.B = cpu.B;  // No change, just copy B to B
    cpu.PC++;
    cpu.updateCycles(4); // LD takes 4 cycles
}

void LD_B_C(CPU& cpu) {
    cpu.B = cpu.C;  // Copy C to B
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_B_D(CPU& cpu) {
    cpu.B = cpu.D;  // Copy D to B
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_B_E(CPU& cpu) {
    cpu.B = cpu.E;  // Copy E to B
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_B_H(CPU& cpu) {
    cpu.B = cpu.H;  // Copy H to B
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_B_L(CPU& cpu) {
    cpu.B = cpu.L;  // Copy L to B
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_B_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL register pair
    cpu.B = cpu.readMemory(hl);  // Copy value at HL into B
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_B_A(CPU& cpu) {
    cpu.B = cpu.A;  // Copy A to B
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_C_B(CPU& cpu) {
    cpu.C = cpu.B;  // Copy B to C
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_C_C(CPU& cpu) {
    cpu.C = cpu.C;  // No change, just copy C to C
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_C_D(CPU& cpu) {
    cpu.C = cpu.D;  // Copy D to C
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_C_E(CPU& cpu) {
    cpu.C = cpu.E;  // Copy E to C
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_C_H(CPU& cpu) {
    cpu.C = cpu.H;  // Copy H to C
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_C_L(CPU& cpu) {
    cpu.C = cpu.L;  // Copy L to C
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_C_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL register pair
    cpu.C = cpu.readMemory(hl);  // Copy value at HL into C
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_C_A(CPU& cpu) {
    cpu.C = cpu.A;  // Copy A to C
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_D_B(CPU& cpu) {
    cpu.D = cpu.B;  // Copy value of B into D
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_D_C(CPU& cpu) {
    cpu.D = cpu.C;  // Copy value of C into D
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_D_D(CPU& cpu) {
    cpu.D = cpu.D;  // Copy value of D into D (no change)
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_D_E(CPU& cpu) {
    cpu.D = cpu.E;  // Copy value of E into D
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_D_H(CPU& cpu) {
    cpu.D = cpu.H;  // Copy value of H into D
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_D_L(CPU& cpu) {
    cpu.D = cpu.L;  // Copy value of L into D
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_D_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.D = cpu.readMemory(hl);  // Copy value from memory at HL into D
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_D_A(CPU& cpu) {
    cpu.D = cpu.A;  // Copy value of A into D
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_E_B(CPU& cpu) {
    cpu.E = cpu.B;  // Copy value of B into E
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_E_C(CPU& cpu) {
    cpu.E = cpu.C;  // Copy value of C into E
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_E_D(CPU& cpu) {
    cpu.E = cpu.D;  // Copy value of D into E
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_E_E(CPU& cpu) {
    cpu.E = cpu.E;  // Copy value of E into E (no change)
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_E_H(CPU& cpu) {
    cpu.E = cpu.H;  // Copy value of H into E
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_E_L(CPU& cpu) {
    cpu.E = cpu.L;  // Copy value of L into E
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_E_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.E = cpu.readMemory(hl);  // Copy value from memory at HL into E
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_E_A(CPU& cpu) {
    cpu.E = cpu.A;  // Copy value of A into E
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_H_B(CPU& cpu) {
    cpu.H = cpu.B;  // Copy value of B into H
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_H_C(CPU& cpu) {
    cpu.H = cpu.C;  // Copy value of C into H
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_H_D(CPU& cpu) {
    cpu.H = cpu.D;  // Copy value of D into H
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_H_E(CPU& cpu) {
    cpu.H = cpu.E;  // Copy value of E into H
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_H_H(CPU& cpu) {
    cpu.H = cpu.H;  // Copy value of H into H (no change)
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_H_L(CPU& cpu) {
    cpu.H = cpu.L;  // Copy value of L into H
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_H_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.H = cpu.readMemory(hl);  // Copy value from memory at HL into H
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_H_A(CPU& cpu) {
    cpu.H = cpu.A;  // Copy value of A into H
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_L_B(CPU& cpu) {
    cpu.L = cpu.B;  // Copy value of B into L
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_L_C(CPU& cpu) {
    cpu.L = cpu.C;  // Copy value of C into L
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_L_D(CPU& cpu) {
    cpu.L = cpu.D;  // Copy value of D into L
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_L_E(CPU& cpu) {
    cpu.L = cpu.E;  // Copy value of E into L
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_L_H(CPU& cpu) {
    cpu.L = cpu.H;  // Copy value of H into L
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_L_L(CPU& cpu) {
    cpu.L = cpu.L;  // Copy value of L into L (no change)
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_L_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.L = cpu.readMemory(hl);  // Copy value from memory at HL into L
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_L_A(CPU& cpu) {
    cpu.L = cpu.A;  // Copy value of A into L
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_HL_B(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.B);  // Store value of B into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_C(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.C);  // Store value of C into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_D(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.D);  // Store value of D into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_E(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.E);  // Store value of E into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_H(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.H);  // Store value of H into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_L(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.L);  // Store value of L into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void HALT(CPU& cpu) {
    // HALT does nothing except stop the CPU from executing instructions
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for HALT
}

void LD_HL_A(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.A);  // Store value of A into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_A_B(CPU& cpu) {
    cpu.A = cpu.B;  // Copy value of B into A
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_A_C(CPU& cpu) {
    cpu.A = cpu.C;  // Copy value of C into A
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_A_D(CPU& cpu) {
    cpu.A = cpu.D;  // Copy value of D into A
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_A_E(CPU& cpu) {
    cpu.A = cpu.E;  // Copy value of E into A
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_A_H(CPU& cpu) {
    cpu.A = cpu.H;  // Copy value of H into A
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_A_L(CPU& cpu) {
    cpu.A = cpu.L;  // Copy value of L into A
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_A_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;  // Combine H and L to form HL
    cpu.A = cpu.readMemory(hl);  // Load value from memory at HL into A
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_A_A(CPU& cpu) {
    cpu.A = cpu.A;  // No change (copy value of A into A)
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void ADD_A_B(CPU& cpu) {
    uint16_t result = cpu.A + cpu.B;
    cpu.Z = (result & 0xFF) == 0;  // Zero flag
    cpu.N = 0;  // Subtraction flag is reset (ADD)
    cpu.H = ((cpu.A & 0xF) + (cpu.B & 0xF)) > 0xF;  // Half-carry flag
    cpu.C = result > 0xFF;  // Carry flag
    cpu.A = result & 0xFF;  // Store the lower byte of the result in A
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register addition
}

void ADD_A_C(CPU& cpu) {
    uint16_t result = cpu.A + cpu.C;
    cpu.Z = (result & 0xFF) == 0;
    cpu.N = 0;
    cpu.H = ((cpu.A & 0xF) + (cpu.C & 0xF)) > 0xF;
    cpu.C = result > 0xFF;
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_D(CPU& cpu) {
    uint16_t result = cpu.A + cpu.D;
    cpu.Z = (result & 0xFF) == 0;
    cpu.N = 0;
    cpu.H = ((cpu.A & 0xF) + (cpu.D & 0xF)) > 0xF;
    cpu.C = result > 0xFF;
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_E(CPU& cpu) {
    uint16_t result = cpu.A + cpu.E;
    cpu.Z = (result & 0xFF) == 0;
    cpu.N = 0;
    cpu.H = ((cpu.A & 0xF) + (cpu.E & 0xF)) > 0xF;
    cpu.C = result > 0xFF;
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_H(CPU& cpu) {
    uint16_t result = cpu.A + cpu.H;
    cpu.Z = (result & 0xFF) == 0;
    cpu.N = 0;
    cpu.H = ((cpu.A & 0xF) + (cpu.H & 0xF)) > 0xF;
    cpu.C = result > 0xFF;
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_L(CPU& cpu) {
    uint16_t result = cpu.A + cpu.L;
    cpu.Z = (result & 0xFF) == 0;
    cpu.N = 0;
    cpu.H = ((cpu.A & 0xF) + (cpu.L & 0xF)) > 0xF;
    cpu.C = result > 0xFF;
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_HL(CPU& cpu) {
    uint16_t hl = (cpu.H << 8) | cpu.L;
    uint16_t result = cpu.A + cpu.readMemory(hl);
    cpu.Z = (result & 0xFF) == 0;
    cpu.N = 0;
    cpu.H = ((cpu.A & 0xF) + (cpu.readMemory(hl) & 0xF)) > 0xF;
    cpu.C = result > 0xFF;
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(8);  // Memory access takes 8 cycles
}

void ADD_A_A(CPU& cpu) {
    uint16_t result = cpu.A + cpu.A;
    cpu.Z = (result & 0xFF) == 0;
    cpu.N = 0;
    cpu.H = ((cpu.A & 0xF) + (cpu.A & 0xF)) > 0xF;
    cpu.C = result > 0xFF;
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
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


void RETI(CPU& cpu) {
    std::cout << "[DEBUG] Stack before RETI: SP: 0x" << std::hex << cpu.SP << std::endl;
    
    uint16_t returnAddr = cpu.popFromStack();
    cpu.PC = returnAddr;

    cpu.enableInterrupts();  // Enable interrupts

    std::cout << "[DEBUG] RETI executed. Returning to: 0x" << std::hex << cpu.PC << std::endl;

    cpu.updateCycles(16);  // Correct cycle count for RETI
}


void ADC_A_n8(CPU& cpu) {
    uint8_t n8 = cpu.readMemory(cpu.PC + 1); // Read the immediate 8-bit value
    uint16_t result = cpu.A + n8 + (cpu.F & 0x10 ? 1 : 0); // Add A + n8 + carry

    // Check for Zero flag
    if (result == 0) {
        cpu.F |= 0x80;  // Set Z flag (bit 7)
    } else {
        cpu.F &= ~0x80; // Clear Z flag
    }

    // Check for Half Carry (carry from bit 3)
    if ((cpu.A & 0xF) + (n8 & 0xF) + (cpu.F & 0x10 ? 1 : 0) > 0xF) {
        cpu.F |= 0x20;  // Set H flag (bit 5)
    } else {
        cpu.F &= ~0x20; // Clear H flag
    }

    // Check for Carry (carry from bit 7)
    if (result > 0xFF) {
        cpu.F |= 0x10;  // Set C flag (bit 4)
    } else {
        cpu.F &= ~0x10; // Clear C flag
    }

    // Store the result in A (only the lower 8 bits)
    cpu.A = static_cast<uint8_t>(result);

    // Update the Program Counter
    cpu.PC += 2;

    // Update cycles
    cpu.updateCycles(8);  // ADC A, n8 takes 8 cycles
}



void no_opcode(CPU& cpu) {
 
}