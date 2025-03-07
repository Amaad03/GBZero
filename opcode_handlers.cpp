
#include "opcode_handlers.h"
#include "cpu.h"  // Include the CPU class to access registers
#include <iostream>
// Define the opcode handler functions
void NOP(CPU& cpu) {
    std::cout << "[DEBUG] NOP executed" << std::endl;
    cpu.PC++; 
    cpu.updateCycles(4);
    
}

void LD_BC_d16(CPU& cpu) { 
    cpu.setBC(cpu.read16(cpu.PC+1));
    cpu.PC += 3;  
    cpu.updateCycles(12);
}
void LD_BC_A(CPU& cpu) {
    cpu.writeMemory(cpu.getBC(), cpu.A);// Store the value of register A at the address
    cpu.PC ++;  // Move the program counter forward by 1 byte (the opcode)
    cpu.updateCycles(8);  // This opcode takes 8 cycles
}


void INC_BC(CPU& cpu) {
    cpu.setBC(cpu.getBC()+1); 
    cpu.PC++;
    cpu.updateCycles(8);
}


void INC_B(CPU& cpu) {
    cpu.increment8(cpu.B);
    cpu.PC++;
    cpu.updateCycles(4);
} 
void DEC_B(CPU& cpu) {
    cpu.decrement8(cpu.B);
    cpu.PC++;  
    cpu.updateCycles(4);  
}


void LD_B_d8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    cpu.B = value;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RLCA(CPU& cpu) {
    uint8_t carry = (cpu.A & 0x80) >> 7;
    cpu.A = (cpu.A << 1) | carry; 
    cpu.setCarryFlag(carry);
    cpu.setZeroFlag(false);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}


void LD_a16_SP(CPU& cpu) {  // 0x08
    uint16_t addr =  cpu.read16(cpu.PC+1); 
    cpu.write16(addr, cpu.SP);
    cpu.PC += 3;
    cpu.updateCycles(20);

}
void ADD_HL_BC(CPU& cpu) {  // 0x09
    uint16_t hl = cpu.getHL();
    uint16_t bc = cpu.getBC();
    uint16_t result = hl + bc;
    cpu.setHL(result);
    cpu.setSubtractFlag(false);  
    cpu.setHalfCarryFlag((hl & 0xFFF) + (bc & 0xFFF) > 0xFFF);  
    cpu.setCarryFlag(result < hl);  
    cpu.PC++; 
    cpu.updateCycles(8);  
}

void LD_A_BC(CPU& cpu) { //0x0A
    cpu.A = cpu.read8(cpu.getBC());
    cpu.PC += 1;
    cpu.updateCycles(8);
}

void DEC_BC(CPU& cpu) { //0x0B
    cpu.setBC(cpu.getBC() - 1);
    cpu.PC++;
    cpu.updateCycles(8);

}

void INC_C(CPU& cpu) { //0x0C
    cpu.increment8(cpu.C);
    cpu.PC++;
    cpu.updateCycles(4);
}

void DEC_C(CPU& cpu) { //0x0D
    cpu.decrement8(cpu.C);
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_C_n8(CPU& cpu) { //0x0E
    cpu.C = cpu.read8(cpu.PC + 1);
    cpu.PC += 2;
    cpu.updateCycles(8);

}

void RRCA(CPU& cpu) { //0x0F
    bool carryBit = cpu.F & 0x10;  
    cpu.setCarryFlag(false);
    cpu.setCarryFlag((cpu.A & 0x01) != 0);
    cpu.A = (cpu.A >> 1) | (carryBit ? 0x80 : 0); 
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setHalfCarryFlag(false);  
    cpu.setSubtractFlag(false); 
    cpu.PC++; 
    cpu.updateCycles(4);  
}

void STOP_n8(CPU& cpu) {
    cpu.PC += 2;
    cpu.updateCycles(4);
    cpu.isStopped = true;
}


void LD_DE_n16(CPU& cpu) {   // this is complete
    uint16_t value = cpu.read16(cpu.PC + 1);
    cpu.setDE(value);
    cpu.PC += 3;
    cpu.updateCycles(12);
}

void LD_DE_A(CPU& cpu) {
    cpu.writeMemory(cpu.getDE(), cpu.A); // complete
    cpu.PC++;
    cpu.updateCycles(8);
}

void INC_DE(CPU& cpu) { // complete
    uint16_t value = ((cpu.D << 8) + cpu.E) + 1;
    cpu.D = value >> 8;
    cpu.E = value &0x00FF;
    cpu.PC++;  
    cpu.updateCycles(8); 
}

void INC_D(CPU& cpu) {  // both good 
    cpu.increment8(cpu.D);
    cpu.PC++;                      
    cpu.updateCycles(4);           
}

void DEC_D(CPU& cpu) { // both good 
    cpu.decrement8(cpu.D);         
    cpu.PC++;                      
    cpu.updateCycles(4);          
}



void LD_D_n8(CPU& cpu) { // this is good
    uint8_t value = cpu.read8(cpu.PC + 1);
    cpu.D = value;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RLA(CPU& cpu) { //double check
    uint8_t result = (cpu.A << 1) + ((cpu.F & ((1 << cpu.getCarryFlag()) != 0 )));
    cpu.setZeroFlag(false);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(cpu.A & 0x80);  
    cpu.A = result;
    cpu.PC++;
    cpu.updateCycles(4);
}

void JR_e8(CPU& cpu) {
    int8_t offset = cpu.read8(cpu.PC + 1);
    cpu.PC += 2 + offset; 
    cpu.updateCycles(12);
}

void ADD_HL_DE(CPU& cpu) {
    uint16_t hl = cpu.getHL();
    uint16_t de = cpu.getDE();
    uint16_t result = hl + de ;  
    cpu.setHalfCarryFlag((hl & 0x0FFF) + (hl & 0x0FFF) > 0x0FFF);
    cpu.setCarryFlag(result > 0xFFFF); 
    cpu.setZeroFlag(false);  
    cpu.setSubtractFlag(false);  
    cpu.setHL(result);
    cpu.PC++;
    cpu.updateCycles(8);
}

void LD_A_DE(CPU& cpu) {
    uint16_t de = cpu.getDE(); 
    cpu.A = cpu.read8(de);
    cpu.PC++;
    cpu.updateCycles(8);
}

void DEC_DE(CPU& cpu) {
    cpu.decrement16(cpu.D, cpu.E); 
    cpu.PC++;
    cpu.updateCycles(8);
}

void INC_E(CPU& cpu) {
    cpu.increment8(cpu.E);
    cpu.PC++;
    cpu.updateCycles(8);
}

void DEC_E(CPU& cpu) {
    cpu.decrement8(cpu.E);
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_E_n8(CPU& cpu) {
    cpu.E = cpu.read8(cpu.PC + 1);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RRA(CPU& cpu) {
    bool carry = cpu.F & 0x10;  
    uint8_t oldA = cpu.A;
    cpu.A = (cpu.A >> 1) | (carry << 7);  
    if (oldA & 0x01) {
        cpu.setCarryFlag(true); 
    } else {
        cpu.setCarryFlag(false);
    }
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void JR_NZ_e8(CPU& cpu) {
    cpu.PC += 2;
    if ((cpu.F & (1 << cpu.getZeroFlag())) == 0) {
        int8_t offset = cpu.read8(cpu.PC + 1);  
        cpu.PC += offset;  
        cpu.updateCycles(12);  
    } else {
        cpu.updateCycles(8);  
    }
}

void LD_HL_n16(CPU& cpu) { 
    cpu.setHL(cpu.read16(cpu.PC+1));
    cpu.PC += 3; 
    cpu.updateCycles(12); 
}

void LD_HLplus_A(CPU& cpu) {
    uint16_t hl = cpu.getHL();
    cpu.writeMemory(hl, cpu.A);        
    hl++;
    cpu.setHL(hl);
    cpu.PC++;  
    cpu.updateCycles(8);  
}

void INC_HL(CPU& cpu) {
    cpu.increment16(cpu.H, cpu.L);
    cpu.PC++;
    cpu.updateCycles(8);
}

void INC_H(CPU& cpu) {
    cpu.increment8(cpu.H);
    cpu.PC++;  
    cpu.updateCycles(4); 
}

void DEC_H(CPU& cpu) {
    cpu.decrement8(cpu.H);
    cpu.PC++;  
    cpu.updateCycles(4); 
}

void LD_H_n8(CPU& cpu) {
    cpu.H = cpu.read8(cpu.PC + 1);  // Load immediate 8-bit value into H
    cpu.PC += 2;  // Move program counter to the next instruction
    cpu.updateCycles(8);  // 8 cycles for this instruction
}
void DAA(CPU& cpu) {
    if ((cpu.F & 0x40)  == 0) {  
        if ((cpu.A & 0x0F) > 9 || (cpu.F & 0x20)) { 
            cpu.A += 0x06;
        }
        if (cpu.A > 0x99 || (cpu.F & 0x10)) { 
            cpu.A += 0x60;
            cpu.setCarryFlag(true);  
        }
    } else {  
        if ((cpu.A & 0x0F) < 6) {  
            cpu.A -= 0x06;
        }
        if (cpu.A < 0x60) { 
            cpu.A -= 0x60;
            cpu.setCarryFlag(true); 
        }
    }
    cpu.setHalfCarryFlag(false);
    cpu.setZeroFlag(cpu.A == 0);  
    cpu.PC++;
    cpu.updateCycles(4);  
}

void JRZ_e8(CPU& cpu) {
    cpu.PC += 2;
    if((cpu.F & (1 << cpu.getZeroFlag())) != 0) {  
        int8_t offset = cpu.read8(cpu.PC + 1); 
        cpu.PC += offset; 
        cpu.updateCycles(12);  
    } else {
        cpu.updateCycles(8);  
    }
}
void ADD_HL_HL(CPU& cpu) {
    uint16_t hl = cpu.getHL();

    uint16_t result = hl + hl;           
    if (result > 0xFFFF) {
        cpu.F |= 0x10;  
    } else {
        cpu.F &= ~0x10;  
    }

    if ((hl & 0x0FFF) + (hl & 0x0FFF) > 0x0FFF) {
        cpu.F |= 0x20;  
    } else {
        cpu.F &= ~0x20;  
    }
    cpu.setHL(result);
    cpu.PC++;  
    cpu.updateCycles(8); 
}
void LD_A_HLplus(CPU& cpu) {
    uint16_t hl = cpu.getHL();
    cpu.A = cpu.read8(hl); 
    hl++;
    cpu.setHL(hl);      
    cpu.PC++;  
    cpu.updateCycles(8); 
}
void DEC_HL(CPU& cpu) {
    uint16_t hl = cpu.getHL();  
    hl--;  
    cpu.setHL(hl); 
    cpu.PC++;  
    cpu.updateCycles(8); 
}
void INC_L(CPU& cpu) {
    cpu.increment8(cpu.L);
    cpu.PC++; 
    cpu.updateCycles(4); 
}

void DEC_L(CPU& cpu) {
    cpu.decrement8(cpu.L);
    cpu.PC++;  
    cpu.updateCycles(4);  
}

void LD_L_n8(CPU& cpu) {
    cpu.L = cpu.read8(cpu.PC + 1);  
    cpu.PC += 2;  
    cpu.updateCycles(8);  
}

void CPL(CPU& cpu) {
    cpu.A = ~cpu.A;  
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag(true);
    cpu.PC++;
    cpu.updateCycles(4);
}

void LD_SP_n16(CPU& cpu) {
    uint16_t value = cpu.read16(cpu.PC+1);  
    cpu.SP = value; 
    cpu.PC += 3;  
    cpu.updateCycles(12);  
}

void LD_HL_minus_A(CPU& cpu) {
    uint16_t hl = cpu.getHL();  
    cpu.writeMemory(hl, cpu.A);          
    hl--;
    cpu.setHL(hl);          
    cpu.PC++;                                 
    cpu.updateCycles(8);                       
}

void INC_SP(CPU& cpu) {
    uint16_t t = cpu.SP + 1;
    cpu.SP = t;
    cpu.PC++;  
    cpu.updateCycles(8);  
}

void INC_HL_mem(CPU& cpu) {
    uint16_t address = cpu.getHL();  
    uint8_t value = cpu.read8(address);                     
    uint8_t result = value + 1;    
    cpu.updateCycles(4); 
    cpu.writeMemory(address, result);       
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag((value & 0x0F) + 1 > 0x0F);
    cpu.PC++;                          
    cpu.updateCycles(8);               
}
void DEC_HL_mem(CPU& cpu) {
    uint16_t address = cpu.getHL();  
    uint8_t value = cpu.read8(address);   
    uint8_t result = value - 1;   
    cpu.updateCycles(4);       
    cpu.writeMemory(address, result);   

    cpu.setZeroFlag(result==0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((value & 0x0F) == 0); 
    cpu.PC++;  
    cpu.updateCycles(8);    
}   



void LD_HL_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);  
    cpu.writeMemory(cpu.getHL(), value);               
    cpu.PC += 2;  
    cpu.updateCycles(8);  
}

void SCF(CPU& cpu) {
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false); 
    cpu.setCarryFlag(true);   
    cpu.PC++;     
    cpu.updateCycles(4);  
}

void JR_C_e8(CPU& cpu) {
    cpu.PC += 2;  
    if ((cpu.F & (1 << cpu.getCarryFlag())) != 0) { 
        int8_t offset = cpu.read8(cpu.PC + 1); 
        cpu.PC += offset;  
        cpu.updateCycles(12);  
    } else {
       
        cpu.updateCycles(8);  
    }
}

void ADD_HL_SP(CPU& cpu) {
    uint16_t hl = cpu.getHL();  
    uint16_t result = hl + cpu.SP;       
    if ((hl & 0x0FFF) + (cpu.SP & 0x0FFF) > 0x0FFF) {
        cpu.F |= 0x20;  
    } else {
        cpu.F &= ~0x20; 
    }
    if (result > 0xFFFF) {
        cpu.F |= 0x10;  
    } else {
        cpu.F &= ~0x10;  
    }
    cpu.setHL(result);
    cpu.PC++;  
    cpu.updateCycles(8);  
}

void LD_A_HL_minus(CPU& cpu) {
    uint16_t hl = cpu.getHL(); 
    cpu.A = cpu.read8(hl);        
    hl--;        
    cpu.setHL(hl);
    cpu.PC++;                               
    cpu.updateCycles(8);                     
}

void DEC_SP(CPU& cpu) {
    uint16_t t = cpu.SP - 1;
    cpu.SP = t;
    cpu.PC++;  
    cpu.updateCycles(8);  
}

void INC_A(CPU& cpu) {
    cpu.increment8(cpu.A);
    cpu.PC++;  
    cpu.updateCycles(4);  
}

void DEC_A(CPU& cpu) {
    cpu.increment8(cpu.A);
    cpu.PC++;  
    cpu.updateCycles(4);  
}


void LD_A_n8(CPU& cpu) {
    cpu.A = cpu.read8(cpu.PC + 1);
    cpu.PC += 2;  
    cpu.updateCycles(8);  
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
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL register pair
    cpu.B = cpu.read8(hl);  // Copy value at HL into B
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
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL register pair
    cpu.C = cpu.read8(hl);  // Copy value at HL into C
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
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.D = cpu.read8(hl);  // Copy value from memory at HL into D
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
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.E = cpu.read8(hl);  // Copy value from memory at HL into E
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
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.H = cpu.read8(hl);  // Copy value from memory at HL into H
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
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.L = cpu.read8(hl);  // Copy value from memory at HL into L
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_L_A(CPU& cpu) {
    cpu.L = cpu.A;  // Copy value of A into L
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register transfer
}

void LD_HL_B(CPU& cpu) {
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.B);  // Store value of B into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_C(CPU& cpu) {
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.C);  // Store value of C into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_D(CPU& cpu) {
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.D);  // Store value of D into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_E(CPU& cpu) {
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.E);  // Store value of E into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_H(CPU& cpu) {
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.H);  // Store value of H into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void LD_HL_L(CPU& cpu) {
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.writeMemory(hl, cpu.L);  // Store value of L into memory at address HL
    cpu.PC++;
    cpu.updateCycles(8);  // 8 cycles for memory access
}

void HALT(CPU& cpu) {
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
    uint16_t hl = cpu.getHL();  // Combine H and L to form HL
    cpu.A = cpu.read8(hl);  // Load value from memory at HL into A
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
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) + 1 > 0x0F);
    cpu.A = result & 0xFF;  // Store the lower byte of the result in A
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for register-to-register addition
}

void ADD_A_C(CPU& cpu) {
    uint16_t result = cpu.A + cpu.C;
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.C & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_D(CPU& cpu) {
    uint16_t result = cpu.A + cpu.D;
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.D & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_E(CPU& cpu) {
    uint16_t result = cpu.A + cpu.E;
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.E & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_H(CPU& cpu) {
    uint16_t result = cpu.A + cpu.H;
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.H & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_L(CPU& cpu) {
    uint16_t result = cpu.A + cpu.L;
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.L & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADD_A_HL(CPU& cpu) {
    uint8_t valueFromHL = cpu.read8(cpu.getHL());
    uint16_t result = cpu.A + valueFromHL + (cpu.getCarryFlag() ? 1 : 0); 
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (valueFromHL & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(8);  // Memory access takes 8 cycles
}

void ADD_A_A(CPU& cpu) {
    uint16_t result = cpu.A + cpu.A;
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.A & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADC_A_B(CPU& cpu) {
    uint16_t result = cpu.A + cpu.B + (cpu.getCarryFlag() ? 1 : 0);
   
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.B & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}
void ADC_A_C(CPU& cpu) {
    uint16_t result = cpu.A + cpu.C + (cpu.getCarryFlag() ? 1 : 0);
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.C & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}
void ADC_A_D(CPU& cpu) {
    uint16_t result = cpu.A + cpu.D + (cpu.getCarryFlag() ? 1 : 0);
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.D & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}

void ADC_A_E(CPU& cpu) {
    uint16_t result = cpu.A + cpu.E + (cpu.getCarryFlag() ? 1 : 0);
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.E & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}
void ADC_A_H(CPU& cpu) {
    uint16_t result = cpu.A + cpu.H+ (cpu.getCarryFlag() ? 1 : 0);
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.H & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
    
}


void ADC_A_L(CPU& cpu) {
    uint16_t result = cpu.A + cpu.L+ (cpu.getCarryFlag() ? 1 : 0);
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.L & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
    
}
void ADC_A_HL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL()); 
    uint16_t result = cpu.A + value + (cpu.getCarryFlag() ? 1 : 0); 
    cpu.A = result & 0xFF; 
    cpu.setZeroFlag((result & 0xFF) == 0); 
    cpu.setSubtractFlag(false);  
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (value & 0x0F) + (cpu.getCarryFlag() ? 1 : 0)) > 0x0F);  
    cpu.setCarryFlag(result > 0xFF);  
    cpu.PC++; 
    cpu.updateCycles(4); 
}
void ADC_A_A(CPU& cpu) {
    uint16_t result = cpu.A + cpu.A+ (cpu.getCarryFlag() ? 1 : 0);
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false); 
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (cpu.A & 0x0F)) > 0x0F); 
    cpu.setCarryFlag(result > 0xFF); 
    cpu.A = result & 0xFF;
    cpu.PC++;
    cpu.updateCycles(4);
}


void SUB_A_B(CPU& cpu) {
    uint16_t result = cpu.A - cpu.B;
    cpu.A = result & 0xFF;  // Update A
    cpu.setZeroFlag(cpu.A == 0);  // Zero flag if A becomes 0
    cpu.setSubtractFlag(true);  // Subtraction occurred
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.B & 0x0F));  // Half-carry flag
    cpu.setCarryFlag(result > 0xFF);  // Carry flag is set if result overflows
    cpu.PC++;
    cpu.updateCycles(4);  // 4 cycles for this operation
}

void SUB_A_C(CPU& cpu) {
    uint16_t result = cpu.A - cpu.C;
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.C & 0x0F));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SUB_A_D(CPU& cpu) {
    uint16_t result = cpu.A - cpu.D;
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.D & 0x0F));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SUB_A_E(CPU& cpu) {
    uint16_t result = cpu.A - cpu.E;
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.E & 0x0F));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SUB_A_H(CPU& cpu) {
    uint16_t result = cpu.A - cpu.H;
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.H & 0x0F));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SUB_A_L(CPU& cpu) {
    uint16_t result = cpu.A - cpu.L;
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.L & 0x0F));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SUB_A_memHL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL());
    uint16_t result = cpu.A - value;
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (value & 0x0F));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(8);  // Memory access takes 8 cycles
}

void SUB_A_A(CPU& cpu) {
    uint16_t result = cpu.A - cpu.A;
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.A & 0x0F));  // This will always be true
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}


void SBC_A_B(CPU& cpu) {
    uint16_t result = cpu.A - cpu.B - (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.B & 0x0F) + (cpu.getCarryFlag() ? 1 : 0));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SBC_A_C(CPU& cpu) {
    uint16_t result = cpu.A - cpu.C - (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.C & 0x0F) + (cpu.getCarryFlag() ? 1 : 0));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SBC_A_D(CPU& cpu) {
    uint16_t result = cpu.A - cpu.D - (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.D & 0x0F) + (cpu.getCarryFlag() ? 1 : 0));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SBC_A_E(CPU& cpu) {
    uint16_t result = cpu.A - cpu.E - (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.E & 0x0F) + (cpu.getCarryFlag() ? 1 : 0));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SBC_A_H(CPU& cpu) {
    uint16_t result = cpu.A - cpu.H - (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.H & 0x0F) + (cpu.getCarryFlag() ? 1 : 0));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SBC_A_L(CPU& cpu) {
    uint16_t result = cpu.A - cpu.L - (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.L & 0x0F) + (cpu.getCarryFlag() ? 1 : 0));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void SBC_A_memHL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL());
    uint16_t result = cpu.A - value - (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (value & 0x0F) + (cpu.getCarryFlag() ? 1 : 0));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(8);  // Memory access takes 8 cycles
}

void SBC_A_A(CPU& cpu) {
    uint16_t result = cpu.A - cpu.A - (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) > (cpu.A & 0x0F) + (cpu.getCarryFlag() ? 1 : 0));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC++;
    cpu.updateCycles(4);
}

void AND_A_B(CPU& cpu) {
    uint8_t  t =  cpu.A &= cpu.B;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}


void AND_A_C(CPU& cpu) {
    uint8_t  t = cpu.A &= cpu.C;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}


void AND_A_D(CPU& cpu) {
    uint8_t  t =  cpu.A &= cpu.D;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}



void AND_A_E(CPU& cpu) {
    uint8_t  t =  cpu.A &= cpu.E;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false); 
    cpu.A = t; 
    cpu.PC++;
    cpu.updateCycles(4);
}

void AND_A_H(CPU& cpu) {
    uint8_t t = cpu.A &= cpu.H;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);
    cpu.A = t; 
    cpu.PC++;
    cpu.updateCycles(4);
}


void AND_A_L(CPU& cpu) {
    uint8_t t = cpu.A &= cpu.L;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void AND_A_memHL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL());
    uint8_t t = cpu.A &= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(8);
}
void AND_A_A(CPU& cpu) {
    uint8_t t = cpu.A &= cpu.A;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_B(CPU& cpu) {
    uint8_t t = cpu.A ^=cpu.B;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_C(CPU& cpu) {
    uint8_t t = cpu.A ^=cpu.C;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_D(CPU& cpu) {
    uint8_t t = cpu.A ^=cpu.D;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_E(CPU& cpu) {
    uint8_t t = cpu.A ^=cpu.E;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_H(CPU& cpu) {
    uint8_t t = cpu.A ^=cpu.H;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_L(CPU& cpu) {
    uint8_t t = cpu.A ^=cpu.L;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_memHL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL());
    uint8_t t =  cpu.A ^= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_A(CPU& cpu) {
    uint8_t t = cpu.A ^=cpu.A;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}

void OR_A_B(CPU& cpu) {
    uint8_t t = cpu.A |= cpu.B;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void OR_A_C(CPU& cpu) {
    uint8_t t = cpu.A |= cpu.C;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void OR_A_D(CPU& cpu) {
    uint8_t t = cpu.A |= cpu.D;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void OR_A_E(CPU& cpu) {
    uint8_t t = cpu.A |= cpu.E;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}


void OR_A_H(CPU& cpu) {
    uint8_t t = cpu.A |= cpu.H;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void OR_A_L(CPU& cpu) {
    uint8_t t = cpu.A |= cpu.L;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}
void OR_A_memHL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL());
    uint8_t t  = cpu.A |= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}

void OR_A_A(CPU& cpu) {
    uint8_t t = cpu.A |= cpu.A;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC++;
    cpu.updateCycles(4);
}





void CP_A_B(CPU& cpu) {
    uint16_t result = cpu.A - cpu.B;
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (cpu.B & 0x0F));
    cpu.setCarryFlag(cpu.A < cpu.B);
    cpu.PC++;
    cpu.updateCycles(4);
}

void CP_A_C(CPU& cpu) {
    uint16_t result = cpu.A - cpu.C;
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (cpu.C & 0x0F));
    cpu.setCarryFlag(cpu.A < cpu.C);
    cpu.PC++;
    cpu.updateCycles(4);
}

void CP_A_D(CPU& cpu) {
    uint16_t result = cpu.A - cpu.D;
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (cpu.D & 0x0F));
    cpu.setCarryFlag(cpu.A < cpu.D);
    cpu.PC++;
    cpu.updateCycles(4);
}

void CP_A_E(CPU& cpu) {
    uint16_t result = cpu.A - cpu.E;
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (cpu.E & 0x0F));
    cpu.setCarryFlag(cpu.A < cpu.E);
    cpu.PC++;
    cpu.updateCycles(4);
}

void CP_A_H(CPU& cpu) {
    uint16_t result = cpu.A - cpu.H;
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (cpu.L & 0x0F));
    cpu.setCarryFlag(cpu.A < cpu.L);
    cpu.PC++;
    cpu.updateCycles(4);
}

void CP_A_L(CPU& cpu) {
    uint16_t result = cpu.A - cpu.L;
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (cpu.L & 0x0F));
    cpu.setCarryFlag(cpu.A < cpu.L);
    cpu.PC++;
    cpu.updateCycles(4);
}

void CP_A_memHL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL());
    uint16_t result = cpu.A - value;
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (value & 0x0F));
    cpu.setCarryFlag(cpu.A < value);
    cpu.PC++;
    cpu.updateCycles(4);
}

void CP_A_A(CPU& cpu) {
    uint16_t result = cpu.A - cpu.A;
    cpu.setZeroFlag(result == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void RET_NZ(CPU& cpu) {
    if (!cpu.getZeroFlag()) {
        cpu.PC = cpu.pop16();  
        cpu.updateCycles(20);
    } else {
        cpu.PC++;
    }
    cpu.updateCycles(8);  // 8 cycles if taken, 12 cycles otherwise
}
void POP_BC(CPU& cpu) {
    uint16_t bc = cpu.pop16();
    cpu.setBC(bc);
    cpu.PC += 1;
    cpu.updateCycles(12);
}

void JP_NZ_a16(CPU& cpu) {
    if (!cpu.getZeroFlag()) {
        cpu.PC = cpu.read16(cpu.PC+1);
        cpu.updateCycles(16);  // or 12 cycles if not taken
    } else {
        cpu.PC += 3;
        cpu.updateCycles(12);
    }
}

void JP_a16(CPU& cpu) {
    cpu.PC = cpu.read16(cpu.PC+1);
    cpu.updateCycles(16);
}
void CALL_NZ_a16(CPU& cpu) {
    
    if (!cpu.getZeroFlag()) { 
        uint16_t address =cpu.PC+3; 
        cpu.push16(address);                     
        cpu.PC =  cpu.read16(cpu.PC + 1);                      
        cpu.updateCycles(24);               
    } else {
        cpu.PC += 3;
        cpu.updateCycles(12);                   
    }
}

void PUSH_BC(CPU& cpu) {
    uint16_t bc = cpu.getBC();   
    cpu.push16(bc);          
    cpu.PC++;                 
    cpu.updateCycles(16);  
}
void ADD_A_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    uint16_t result = cpu.A + value;
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (value & 0x0F)) > 0x0F);
    cpu.setCarryFlag(result > 0xFF);
    cpu.A = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}
void RST_00(CPU& cpu) {
    //cpu.PC += 1;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0000;
    cpu.updateCycles(16);
}
void RET_Z(CPU& cpu) {
    if (cpu.getZeroFlag()) {
        cpu.PC = cpu.pop16();
        cpu.updateCycles(20);
    } else {
        cpu.PC++;
        cpu.updateCycles(8); 
    }

}
void RET(CPU& cpu) {


    uint16_t returnAddress = cpu.pop16();
    cpu.PC = returnAddress;
    cpu.updateCycles(16);
}

void JP_Z_a16(CPU& cpu) {
    if (cpu.getZeroFlag()) {
        cpu.PC = cpu.read16(cpu.PC+1);
        cpu.updateCycles(16);
    } else {
        cpu.PC += 3;
        cpu.updateCycles(12);
    }
}

void PREFIX(CPU& cpu) {
    cpu.PC++;
    cpu.updateCycles(4);
}
void CALL_Z_a16(CPU& cpu) {
    if (cpu.getZeroFlag()) { 
        uint16_t returnAddress = cpu.PC + 3;  
        cpu.push16(returnAddress);  
        cpu.PC = cpu.read16(cpu.PC+1);  
        cpu.updateCycles(24);  
    } else {
        cpu.PC += 3; 
        cpu.updateCycles(12); 
    }
}

void CALL_a16(CPU& cpu) {
    uint16_t returnAddress = cpu.PC + 3; 
    cpu.push16(returnAddress);  
    cpu.PC = cpu.read16(cpu.PC+1);
    cpu.updateCycles(24);
}
void ADC_A_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    uint16_t result = cpu.A + value + (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (value & 0x0F) + (cpu.getCarryFlag() ? 1 : 0)) > 0x0F);
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC += 2;
    cpu.updateCycles(8);
}


void RST_08(CPU& cpu) {
    //cpu.PC++;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0008;
    cpu.updateCycles(16);
}


void RET_NC(CPU& cpu) {
    if (!cpu.getCarryFlag()) {
        cpu.PC = cpu.pop16();
        cpu.updateCycles(20);
    } else {
        cpu.PC++;
        cpu.updateCycles(8);
    }
}

void POP_DE(CPU& cpu) {
    uint16_t de = cpu.pop16();
    cpu.setDE(de);
    cpu.PC++;
    cpu.updateCycles(12);
}

void JP_NC_a16(CPU& cpu) {
    if (!cpu.getCarryFlag()) {
        cpu.PC = cpu.read16(cpu.PC+1);
        cpu.updateCycles(16);
    } else {
        cpu.PC += 3;
        cpu.updateCycles(12);
    }
}

void CALL_NC_a16(CPU& cpu) {
    if (!cpu.getCarryFlag()) {
        uint16_t returnAddress = cpu.PC + 3; 
        cpu.push16(returnAddress);
        cpu.PC = cpu.read16(cpu.PC+1);
        cpu.updateCycles(24);
    } else {
        cpu.PC += 3;
        cpu.updateCycles(12);
    }
}

void PUSH_DE(CPU& cpu) {
    uint16_t de = cpu.getDE();
    cpu.push16(de);
    cpu.updateCycles(16);
}

void SUB_A_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    uint16_t result = cpu.A - value;
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (value & 0x0F));
    cpu.setCarryFlag(result > 0xFF);
    cpu.A = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RST_10(CPU& cpu) {
    //cpu.PC++;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0010;
    cpu.updateCycles(16);
}

void RET_C(CPU& cpu) {
    if (cpu.getCarryFlag()) {
        cpu.PC = cpu.pop16();
        cpu.updateCycles(20); 
    } else {
        cpu.PC++;
        cpu.updateCycles(8); 
    }
     // or 8 cycles if taken
}

void RETI(CPU& cpu) {
    cpu.enableInterrupts();
    cpu.PC = cpu.pop16();
    cpu.updateCycles(16);
}

void JP_C_a16(CPU& cpu) {
    if (cpu.getCarryFlag()) {
        cpu.PC = cpu.read16(cpu.PC+1);
        cpu.updateCycles(16);
    } else {
        cpu.PC += 3;
        cpu.updateCycles(12);
    }
}

void CALL_C_a16(CPU& cpu) {
    if (cpu.getCarryFlag()) {
        uint16_t returnAddress = cpu.PC + 3; 
        cpu.push16(returnAddress);
        cpu.PC = cpu.read16(cpu.PC+1 );
        cpu.updateCycles(24);
    } else {
        cpu.PC += 3;
        cpu.updateCycles(12);
    }
}

void SBC_A_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    uint16_t result = cpu.A - value - (cpu.getCarryFlag() ? 0 : 1);
    cpu.A = result & 0xFF;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(true);
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (value & 0x0F) + (cpu.getCarryFlag() ? 0 : 1));
    cpu.setCarryFlag(result > 0xFF);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RST_18(CPU& cpu) {
    //cpu.PC += 1;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0018;
    cpu.updateCycles(16);
}
void LDH_a8_A(CPU& cpu) {
    uint8_t addr = cpu.read8(cpu.PC + 1);
    cpu.writeMemory(0xFF00 + addr, cpu.A);  // Write A to the address [0xFF00 + addr]
    cpu.PC += 2;
    cpu.updateCycles(12);
}

void POP_HL(CPU& cpu) {
    uint16_t hl = cpu.pop16();
    cpu.setHL(hl);
    cpu.PC += 1;
    cpu.updateCycles(12);
}

void LDH_C_A(CPU& cpu) {
    cpu.writeMemory(0xFF00 + cpu.C, cpu.A);  // Write A to the address [0xFF00 + C]
    cpu.PC++;
    cpu.updateCycles(8);
}

void PUSH_HL(CPU& cpu) {
    uint16_t hl = cpu.getHL();
    cpu.push16(hl);
    cpu.PC += 1;
    cpu.updateCycles(16);
}

void AND_A_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    uint64_t t= cpu.A &= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);  // AND always produces half carry
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RST_20(CPU& cpu) {
    //cpu.PC += 1;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0020;
    cpu.updateCycles(16);
}

void ADD_SP_e8(CPU& cpu) {
    int8_t offset = cpu.read8(cpu.PC + 1);
    uint16_t result = cpu.SP + offset;
    cpu.SP = result;
    cpu.setZeroFlag(false);  // ADD SP doesn't affect the Zero flag
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(((cpu.SP & 0xF) + (offset & 0xF)) > 0xF);
    cpu.setCarryFlag((cpu.SP & 0xFF00) != (result & 0xFF00));  // Detect if there was a carry into the next byte
    cpu.PC += 2;
    cpu.updateCycles(16);
}

void JP_HL(CPU& cpu) {
    uint16_t hl = cpu.getHL();
    cpu.PC = hl;
    cpu.updateCycles(4);
}

void LD_a16_A(CPU& cpu) {
    cpu.updateCycles(8);
    uint16_t addr = cpu.read16(cpu.PC+1);
    cpu.writeMemory(addr, cpu.A);
    cpu.PC += 3;
    cpu.updateCycles(8);
}

void XOR_A_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    uint8_t t = cpu.A ^= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A = t;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RST_28(CPU& cpu) {
    //cpu.PC += 1;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0028;
    cpu.updateCycles(16);
}

void LDH_A_a8(CPU& cpu) {
    cpu.updateCycles(4);
    uint8_t addr = cpu.read8(cpu.PC + 1);
    cpu.A = cpu.read8(0xFF00 + addr);  // Load the value from [0xFF00 + addr] into A
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void POP_AF(CPU& cpu) {

    uint16_t af = cpu.pop16();
    cpu.setAF(af);
    cpu.PC += 1;
    cpu.updateCycles(12);
}

void LDH_A_C(CPU& cpu) {
    cpu.A = cpu.read8(0xFF00 + cpu.C);  // Load the value from [0xFF00 + C] into A
    cpu.PC++;
    cpu.updateCycles(8);
}

void DI(CPU& cpu) {
    cpu.interruptsEnabled = false;  // Disable interrupts
    cpu.PC++;
    cpu.updateCycles(4);
}

void PUSH_AF(CPU& cpu) {
    uint16_t af = cpu.getAF();
    cpu.push16(af);
    cpu.PC += 1;
    cpu.updateCycles(16);
}

void OR_A_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    uint8_t t=    cpu.A |= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.A =t;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RST_30(CPU& cpu) {
    //cpu.PC += 1;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0030;
    cpu.updateCycles(16);
}

void LD_HL_SP_e8(CPU& cpu) {
    int8_t offset = cpu.read8(cpu.PC + 1);
    uint16_t result = cpu.SP + offset;
 
    cpu.setHL(result);
    cpu.setZeroFlag(false);  // LD HL, SP + e8 doesn't affect Zero flag
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(((cpu.SP & 0xF) + (offset & 0xF)) > 0xF);
    cpu.setCarryFlag((cpu.SP & 0xFF00) != (result & 0xFF00));  // Carry check
    cpu.PC += 2;
    cpu.updateCycles(12);
}

void LD_SP_HL(CPU& cpu) {
    uint16_t hl = cpu.getHL();
    cpu.SP = hl;
    cpu.PC++;
    cpu.updateCycles(8);
}

void LD_A_a16(CPU& cpu) {
    cpu.updateCycles(8);
    uint16_t addr = cpu.read16(cpu.PC+1);
    cpu.A = cpu.read8(addr);
    cpu.PC += 3;
    cpu.updateCycles(8);
}

void EI(CPU& cpu) {
    cpu.interruptsEnabled = true;  // Enable interrupts
    cpu.PC++;
    cpu.updateCycles(4);
}

void CP_A_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    
    cpu.setZeroFlag(cpu.A == value);  // Set Zero flag if A equals the immediate value
    cpu.setSubtractFlag(true);  // Subtraction occurs in the comparison
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) - (value & 0x0F)) < 0);  // Check for half carry
    cpu.setCarryFlag(cpu.A < value);  // Set Carry flag if there's a borrow
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RST_38(CPU& cpu) {
    //std::cout << "[DEBUG] RST 38H: Pushing PC = 0x" << std::hex << cpu.PC << " onto stack at SP = 0x" << cpu.SP << std::endl;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0038;
    cpu.updateCycles(16);
}

/*
void SET_7_A(CPU& cpu) {
    uint8_t t =  cpu.A | (1<<7);
    cpu.A = t;
    cpu.PC += 2;
    cpu.updateCycles(8);
}




*/






void no_opcode(CPU& cpu) {
 
}