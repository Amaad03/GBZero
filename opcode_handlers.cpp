
#include "opcode_handlers.h"
#include "cpu.h"  
#include <iostream>



void no_opcode(CPU& cpu) {
    cpu.PC++;
}
void NOP(CPU& cpu) {
    std::cout << "[DEBUG] NOP executed" << std::endl;
    cpu.PC++; 
    cpu.updateCycles(4);
    std::cout << "Z Flag: " << (cpu.getZeroFlag() ? "Set" : "Not Set") << std::endl;
  
    
    
}

void LD_BC_d16(CPU& cpu) { 
    uint16_t value = cpu.read16(cpu.PC+1);
    std::cout<<"the value is: "<< std::hex <<int(value) << std::endl;
    cpu.setBC(value);
    cpu.PC += 3;  
    cpu.updateCycles(12);
}
void LD_BC_A(CPU& cpu) {
    cpu.writeMemory(cpu.getBC(), cpu.A);
    cpu.PC ++;  
    cpu.updateCycles(8);  
}


void INC_BC(CPU& cpu) {
    uint16_t bc = cpu.getBC();
    cpu.setBC(bc + 1);
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
void LD_DE_n16(CPU& cpu) {   
    uint16_t value = cpu.read16(cpu.PC + 1);
    cpu.setDE(value);
    cpu.PC += 3;
    cpu.updateCycles(12);
}

void LD_DE_A(CPU& cpu) {
    cpu.writeMemory(cpu.getDE(), cpu.A); 
    cpu.PC++;
    cpu.updateCycles(8);
}

void INC_DE(CPU& cpu) { 
    uint16_t value = ((cpu.D << 8) + cpu.E) + 1;
    cpu.D = value >> 8;
    cpu.E = value &0x00FF;
    cpu.PC++;  
    cpu.updateCycles(8); 
}

void INC_D(CPU& cpu) {  
    cpu.increment8(cpu.D);
    cpu.PC++;                      
    cpu.updateCycles(4);           
}

void DEC_D(CPU& cpu) { 
    cpu.decrement8(cpu.D);         
    cpu.PC++;                      
    cpu.updateCycles(4);          
}



void LD_D_d8(CPU& cpu) {
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
    uint8_t r8 = cpu.read8(cpu.PC + 1);
    int8_t offset = static_cast<int8_t>(r8);  
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
   
    uint8_t r8 = cpu.read8(cpu.PC + 1);
    int8_t offset = static_cast<int8_t>(r8);
    cpu.PC += 2;
    // Print the value of offset
    std::cout << "Offset value: " << static_cast<int>(offset) << std::endl;

    
    
    if (!cpu.getZeroFlag() ) {
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

void LD_H_d8(CPU& cpu) {
    cpu.H = cpu.read8(cpu.PC + 1);  
    cpu.PC += 2;  
    cpu.updateCycles(8);  
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

void JR_Z_e8(CPU& cpu) {
    int8_t r8 = cpu.read8(cpu.PC + 1); 
    int8_t offset = static_cast<int8_t>(r8);  
  
    cpu.PC += 2;  
    std::cout << "Z Flag: " << (cpu.getZeroFlag() ? "Set" : "Not Set") << std::endl;
  
    if(cpu.getZeroFlag()) {
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
void JR_NC_e8(CPU& cpu) {
    uint8_t r8 = cpu.read8(cpu.PC+1);
    int8_t offset = static_cast<int8_t>(r8);
    cpu.PC+=2;
    if(!cpu.getZeroFlag()) {
        cpu.PC+= offset;
        cpu.updateCycles(12);
    } else {
        cpu.updateCycles(8);
    }
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



void LD_HL_d8(CPU& cpu) {
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
    uint8_t r8 = cpu.read8(cpu.PC + 1);
    int8_t offset = static_cast<int8_t>(r8);  
    cpu.PC += 2;  

    if (!cpu.getCarryFlag()) { 
        
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
    uint8_t value = cpu.read8(cpu.PC + 1);
    cpu.A = value;
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
    uint8_t addr = cpu.read8(cpu.B);
    cpu.D = addr;  // Copy value of B into D
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
    cpu.L = cpu.read8( cpu.getHL());  // Copy value from memory at HL into L
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
    std::cout<<"executing 095" <<std::endl;
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
    cpu.A &= cpu.B;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.PC++;
    cpu.updateCycles(4);
}


void AND_A_C(CPU& cpu) {
    cpu.A &= cpu.C;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.PC++;
    cpu.updateCycles(4);
}


void AND_A_D(CPU& cpu) {
    cpu.A &= cpu.D;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.PC++;
    cpu.updateCycles(4);
}



void AND_A_E(CPU& cpu) {
    cpu.A &= cpu.E;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.PC++;
    cpu.updateCycles(4);
}

void AND_A_H(CPU& cpu) {
    cpu.A &= cpu.H;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false); 
    cpu.PC++;
    cpu.updateCycles(4);
}


void AND_A_L(CPU& cpu) {
    cpu.A &= cpu.L;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.PC++;
    cpu.updateCycles(4);
}
void AND_A_memHL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL());
    cpu.A &= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(8);
}
void AND_A_A(CPU& cpu) {
    cpu.A &= cpu.A;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);    
    cpu.setCarryFlag(false);  
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_B(CPU& cpu) {
    cpu.A ^=cpu.B;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_C(CPU& cpu) {
    cpu.A ^=cpu.C;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_D(CPU& cpu) {
    cpu.A ^=cpu.D;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_E(CPU& cpu) {
    cpu.A ^=cpu.E;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_H(CPU& cpu) {
    cpu.A ^=cpu.H;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_L(CPU& cpu) {
    cpu.A ^=cpu.L;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_memHL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL());
    cpu.A ^= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void XOR_A_A(CPU& cpu) {
    cpu.A ^=cpu.A;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);    
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}

void OR_A_B(CPU& cpu) {
    cpu.A |= cpu.B;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void OR_A_C(CPU& cpu) {
    cpu.A |= cpu.C;
    std::cout << "Z Flag: " << (cpu.getZeroFlag() ? "Set" : "Not Set") << std::endl;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC++;

    std::cout << "Z Flag: " << (cpu.getZeroFlag() ? "Set" : "Not Set") << std::endl;
  
    cpu.updateCycles(4);
}
void OR_A_D(CPU& cpu) {
    cpu.A |= cpu.D;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void OR_A_E(CPU& cpu) {
    cpu.A |= cpu.E;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}


void OR_A_H(CPU& cpu) {
    cpu.A |= cpu.H;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void OR_A_L(CPU& cpu) {
    cpu.A |= cpu.L;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}
void OR_A_memHL(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.getHL());
    cpu.A |= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC++;
    cpu.updateCycles(4);
}

void OR_A_A(CPU& cpu) {
    cpu.A |= cpu.A;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
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
        // Jump is taken: set PC to the 16-bit address
        uint16_t address = cpu.read16(cpu.PC+1);
        cpu.PC = address;
        cpu.updateCycles(16);  // 16 cycles if jump is taken
    } else {
        // Jump is not taken: skip the 2-byte address
        cpu.PC += 3;
        cpu.updateCycles(12);  // 12 cycles if jump is not taken
    }
}

void JP_a16(CPU& cpu) {
    uint16_t addr= cpu.read16(cpu.PC+1);
    std::cout << "[DEBUG] JP executed: Jumping to " << std::hex << addr << std::endl;
    cpu.PC = addr;
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
    cpu.PC += 1;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0000;
    cpu.updateCycles(16);
}
void RET_Z(CPU& cpu) {
    if (cpu.getZeroFlag()) {
        uint16_t returnAddress = cpu.pop16();
        cpu.PC = returnAddress;
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
    
    uint16_t targetAddress = cpu.read16(cpu.PC + 1);
    uint16_t returnAddress = cpu.PC+3; 

    cpu.push16(returnAddress);
 
    cpu.PC = targetAddress;

    
    // Update cycles: CALL instruction takes 6 cycles
    cpu.updateCycles(24);
    

}
//0b8f
//9800


void ADC_A_n8(CPU& cpu) {
    uint8_t value = cpu.read8(cpu.PC + 1);
    uint8_t carry = cpu.getCarryFlag() ? 1 : 0;
    
    uint16_t result = cpu.A + value + carry;  // Compute full result before modifying A
    
    // Compute flags BEFORE modifying A
    cpu.setZeroFlag((result & 0xFF) == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(((cpu.A & 0x0F) + (value & 0x0F) + carry) > 0x0F);
    cpu.setCarryFlag(result > 0xFF);

    cpu.A = result & 0xFF;  // Update register A with the lower byte

    cpu.PC += 2;
    cpu.updateCycles(8);
}


void RST_08(CPU& cpu) {
    cpu.PC++;
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
    cpu.PC++;
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
    cpu.PC++;
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
    std::cout << "[DEBUG] Returning to PC: " << std::hex << cpu.PC << std::endl;
    cpu.PC = cpu.pop16();
    cpu.handleInterrupts();
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
    cpu.PC += 1;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0018;
    cpu.updateCycles(16);
}
void LDH_a8_A(CPU& cpu) {
    uint8_t addr = cpu.read8(cpu.PC + 1);
    std::cout << "Address Value: " << +addr << std::endl;
    cpu.memory.write(0xFF00 + addr, cpu.A);  // Write A to the address [0xFF00 + addr]
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
    cpu.A &= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(true);  // AND always produces half carry
    cpu.setCarryFlag(false);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RST_20(CPU& cpu) {
    cpu.PC += 1;
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
    cpu.A ^= value;
    cpu.setZeroFlag(cpu.A == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(false);
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
    std::cout<<"the add value is: "<< std::hex <<int(addr) << std::endl;
    uint16_t address = 0xFF00 + addr;
    std::cout<<"the add value is: "<< std::hex <<int(address) << std::endl;
    uint8_t value = cpu.read8(address);
    std::cout << "Value at " << std::hex << int(address) << ": 0x" << int(value) << std::endl;

    cpu.A = value;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void POP_AF(CPU& cpu) {

    uint16_t af = cpu.pop16();
    cpu.A = (af >> 8) & 0xFF;
    cpu.F = af & 0xF0;
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

    uint16_t addr = cpu.read16(cpu.PC+1);
    cpu.A = cpu.read8(addr);
    cpu.PC += 3;
    cpu.updateCycles(16);
}

void Elephant_I(CPU& cpu) {
    std::cout << "[DEBUG] Entering EI, PC: " << std::hex << (int)cpu.PC << std::endl;

    cpu.interruptsEnabled =  true;  // Enable interrupts
    cpu.PC++;
    std::cout << "[DEBUG] EI executed. Interrupts will be enabled after the next instruction." << std::endl;

    cpu.updateCycles(4);
}

void CP_A_n8(CPU& cpu) {
    uint8_t value = cpu.memory.read(cpu.PC + 1);
    
    uint8_t result = cpu.A - value;
    std::cout << "Z Flag: " << (cpu.getZeroFlag() ? "Set" : "Not Set") << std::endl;
    // Set flags based on the result of the subtraction
    cpu.setZeroFlag(result == 0);  // Set Zero Flag if result is 0
    cpu.setSubtractFlag(true);  // Subtraction sets the N flag
    cpu.setHalfCarryFlag((cpu.A & 0x0F) < (value & 0x0F));  // Half-carry occurs if lower nibbles subtract with borrow
    cpu.setCarryFlag(cpu.A < value);  // Carry occurs if A is less than value (borrow)

    // Debugging output for flags and result
    std::cout << "Z Flag: " << (cpu.getZeroFlag() ? "Set" : "Not Set") << std::endl;
    std::cout << "Z flag after CP: " << cpu.getZeroFlag() << std::endl;
    std::cout << "[DEBUG] CP: A = 0x" << std::hex << static_cast<int>(cpu.A)
              << ", value = 0x" << static_cast<int>(value)
              << ", result = 0x" << static_cast<int>(result)
              << ", F = 0x" << static_cast<int>(cpu.F) << std::endl;

    // Update the program counter and cycles
    cpu.PC += 2;  // CP instruction is 2 bytes
    cpu.updateCycles(8);  // CP instruction takes 8 cycles
}


void RST_38(CPU& cpu) {
    //std::cout << "[DEBUG] RST 38H: Pushing PC = 0x" << std::hex << cpu.PC << " onto stack at SP = 0x" << cpu.SP << std::endl;
    cpu.PC++;
    cpu.push16(cpu.PC);
    cpu.PC = 0x0038;
    cpu.updateCycles(16);
        
}




void RLC_B(CPU& cpu) {
    uint8_t value = cpu.B;
    bool carryout = value & 0x80;
    value = (value << 1) | (carryout ? 1 : 0);
    cpu.B = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}

void RLC_C(CPU& cpu) {
    uint8_t value = cpu.C;
    bool carryout = value & 0x80;
    value = (value << 1) | (carryout ? 1 : 0);
    cpu.C = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}



void RLC_D(CPU& cpu) {
    uint8_t value = cpu.D;
    bool carryout = value & 0x80;
    value = (value << 1) | (carryout ? 1 : 0);
    cpu.D = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}

void RLC_E(CPU& cpu) {
    uint8_t value = cpu.E;
    bool carryout = value & 0x80;
    value = (value << 1) | (carryout ? 1 : 0);
    cpu.E = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}

void RLC_H(CPU& cpu) {
    uint8_t value = cpu.H;
    bool carryout = value & 0x80;
    value = (value << 1) | (carryout ? 1 : 0);
    cpu.H = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}   



void RLC_L(CPU& cpu) {
    uint8_t value = cpu.L;
    bool carryout = value & 0x80;
    value = (value << 1) | (carryout ? 1 : 0);
    cpu.L = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}


void RLC_bracket_HL(CPU& cpu) {

    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);

    bool carryout = value & 0x80;
    value = (value << 1) | (carryout ? 1 : 0);
    cpu.memory.write(address, value);
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);


    cpu.PC+= 2;
    cpu.updateCycles(16);
}



void RLC_A(CPU& cpu) {
    uint8_t value = cpu.A;
    bool carryout = value & 0x80;
    value = (value << 1) | (carryout ? 1 : 0);
    cpu.A = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}


void RRC_B(CPU& cpu) {
    uint8_t value = cpu.B;
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryout ? 0x80 : 0);
    cpu.B = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}


void RRC_C(CPU& cpu) {
    uint8_t value = cpu.C;
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryout ? 0x80 : 0);
    cpu.C = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}
void RRC_D(CPU& cpu) {
    uint8_t value = cpu.D;
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryout ? 0x80 : 0);
    cpu.D = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}



void RRC_E(CPU& cpu) {
    uint8_t value = cpu.E;
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryout ? 0x80 : 0);
    cpu.E = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}
void RRC_H(CPU& cpu) {
    uint8_t value = cpu.H;
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryout ? 0x80 : 0);
    cpu.H = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}
void RRC_L(CPU& cpu) {
    uint8_t value = cpu.L;
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryout ? 0x80 : 0);
    cpu.L = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}
void RRC_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryout ? 0x80 : 0);
    cpu.memory.write(address, value);
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(16);
}
void RRC_A(CPU& cpu){
    uint8_t value = cpu.A;
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryout ? 0x80 : 0);
    cpu.A = value;
    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    cpu.PC+= 2;
    cpu.updateCycles(8);
}


void RL_B(CPU& cpu) {
    uint8_t value = cpu.B;
    bool carryout = value & 0x80;  // Get the most significant bit (MSB) before shifting
    value = (value << 1) | (cpu.getCarryFlag() ? 1 : 0);  // Shift left and add the carry flag to the LSB
    cpu.B = value;

    cpu.setZeroFlag(value == 0);  // Set ZeroFlag if the result is zero
    cpu.setSubtractFlag(false);  // RL operation does not affect the subtract flag
    cpu.setHalfCarryFlag(false);  // RL operation does not affect the half carry flag
    cpu.setCarryFlag(carryout);  // Set CarryFlag to the value of the old MSB

    cpu.PC += 2;  // Increment PC by 2 for 8-bit instructions (opcode + operand)
    cpu.updateCycles(8);  // 8 cycles for this operation
}

void RL_C(CPU& cpu) {
    uint8_t value = cpu.C;
    bool carryout = value & 0x80; 
    value = (value << 1) | (cpu.getCarryFlag() ? 1 : 0);  
    cpu.C = value;

    cpu.setZeroFlag(value == 0); 
    cpu.setSubtractFlag(false);  
    cpu.setHalfCarryFlag(false);  
    cpu.setCarryFlag(carryout);  

    cpu.PC += 2; 
    cpu.updateCycles(8);  
}

void RL_D(CPU& cpu) {
    uint8_t value = cpu.D;
    bool carryout = value & 0x80; 
    value = (value << 1) | (cpu.getCarryFlag() ? 1 : 0);  
    cpu.D= value;

    cpu.setZeroFlag(value == 0); 
    cpu.setSubtractFlag(false);  
    cpu.setHalfCarryFlag(false);  
    cpu.setCarryFlag(carryout);  

    cpu.PC += 2; 
    cpu.updateCycles(8);  
}

void RL_E(CPU& cpu) {
    uint8_t value = cpu.E;
    bool carryout = value & 0x80; 
    value = (value << 1) | (cpu.getCarryFlag() ? 1 : 0);  
    cpu.E = value;

    cpu.setZeroFlag(value == 0); 
    cpu.setSubtractFlag(false);  
    cpu.setHalfCarryFlag(false);  
    cpu.setCarryFlag(carryout);  

    cpu.PC += 2; 
    cpu.updateCycles(8);  
}

void RL_H(CPU& cpu) {
    uint8_t value = cpu.H;
    bool carryout = value & 0x80; 
    value = (value << 1) | (cpu.getCarryFlag() ? 1 : 0);  
    cpu.H = value;

    cpu.setZeroFlag(value == 0); 
    cpu.setSubtractFlag(false);  
    cpu.setHalfCarryFlag(false);  
    cpu.setCarryFlag(carryout);  

    cpu.PC += 2; 
    cpu.updateCycles(8);  
}

void RL_L(CPU& cpu) {
    uint8_t value = cpu.L;
    bool carryout = value & 0x80; 
    value = (value << 1) | (cpu.getCarryFlag() ? 1 : 0);  
    cpu.L = value;

    cpu.setZeroFlag(value == 0); 
    cpu.setSubtractFlag(false);  
    cpu.setHalfCarryFlag(false);  
    cpu.setCarryFlag(carryout);  

    cpu.PC += 2; 
    cpu.updateCycles(8);  
}
void RL_bracket_HL(CPU& cpu) {
    uint8_t value = cpu.getHL();  // Read the value from the address [HL]
    bool carryout = value & 0x80;
    value = (value << 1) | (cpu.getCarryFlag() ? 1 : 0);
    cpu.setHL(value);  // Write the result back to the memory at [HL]

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(16);  // [HL] takes 16 cycles
}

void RL_A(CPU& cpu) {
    uint8_t value = cpu.A;
    bool carryout = value & 0x80;
    value = (value << 1) | (cpu.getCarryFlag() ? 1 : 0);
    cpu.A = value;

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(8);
}








void RR_B(CPU& cpu) {
    uint8_t value = cpu.B;
    bool carryin = cpu.getCarryFlag();
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryin ? 0x80 : 0);
    cpu.B = value;

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RR_C(CPU& cpu) {
    uint8_t value = cpu.C;
    bool carryin = cpu.getCarryFlag();
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryin ? 0x80 : 0);
    cpu.C = value;

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RR_D(CPU& cpu) {
    uint8_t value = cpu.D;
    bool carryin = cpu.getCarryFlag();
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryin ? 0x80 : 0);
    cpu.D = value;

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RR_E(CPU& cpu) {
    uint8_t value = cpu.E;
    bool carryin = cpu.getCarryFlag();
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryin ? 0x80 : 0);
    cpu.E = value;

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RR_H(CPU& cpu) {
    uint8_t value = cpu.H;
    bool carryin = cpu.getCarryFlag();
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryin ? 0x80 : 0);
    cpu.H = value;

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RR_L(CPU& cpu) {
    uint8_t value = cpu.L;
    bool carryin = cpu.getCarryFlag();
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryin ? 0x80 : 0);
    cpu.L = value;

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void RR_bracket_HL(CPU& cpu) {
    uint8_t value = cpu.getHL();  // Read the value from the address [HL]
    bool carryin = cpu.getCarryFlag();
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryin ? 0x80 : 0);
    cpu.setHL(value);  // Write the result back to the memory at [HL]

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(16);  // [HL] takes 16 cycles
}

void RR_A(CPU& cpu) {
    uint8_t value = cpu.A;
    bool carryin = cpu.getCarryFlag();
    bool carryout = value & 0x01;
    value = (value >> 1) | (carryin ? 0x80 : 0);
    cpu.A = value;

    cpu.setZeroFlag(value == 0);
    cpu.setSubtractFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setCarryFlag(carryout);
    
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SLA_B(CPU& cpu) {
    uint8_t value = cpu.B;  // Get the current value of register B
    uint8_t result = value << 1;
    cpu.setCarryFlag(value & 0x80);  // Set carry if MSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.B = result;
    cpu.PC+= 2;
    cpu.updateCycles(8);

}

void SLA_C(CPU& cpu) {
    uint8_t value = cpu.C;  // Get the current value of register B
    uint8_t result = value << 1;
    cpu.setCarryFlag(value & 0x80);  // Set carry if MSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.C = result;
    cpu.PC+= 2;
    cpu.updateCycles(8);

}
void SLA_D(CPU& cpu) {
    uint8_t value = cpu.D;  // Get the current value of register B
    uint8_t result = value << 1;
    cpu.setCarryFlag(value & 0x80);  // Set carry if MSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.D = result;
    cpu.PC+= 2;
    cpu.updateCycles(8);

}
void SLA_E(CPU& cpu) {
    uint8_t value = cpu.E;  // Get the current value of register B
    uint8_t result = value << 1;
    cpu.setCarryFlag(value & 0x80);  // Set carry if MSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.E = result;
    cpu.PC+= 2;
    cpu.updateCycles(8);

}
void SLA_H(CPU& cpu) {
    uint8_t value = cpu.H;  // Get the current value of register H
    uint8_t result = value << 1;
    cpu.setCarryFlag(value & 0x80);  // Set carry if MSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.H = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SLA_L(CPU& cpu) {
    uint8_t value = cpu.L;  // Get the current value of register L
    uint8_t result = value << 1;
    cpu.setCarryFlag(value & 0x80);  // Set carry if MSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.L = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SLA_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();  // Get memory address from HL
    uint8_t value = cpu.read8(address);  // Read value from memory
    uint8_t result = value << 1;
    cpu.setCarryFlag(value & 0x80);  // Set carry if MSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.writeMemory(address, result);  // Write updated value back to memory
    cpu.PC += 2;
    cpu.updateCycles(16);  // Takes longer to access memory
}

void SLA_A(CPU& cpu) {
    uint8_t value = cpu.A;  // Get the current value of register A
    uint8_t result = value << 1;
    cpu.setCarryFlag(value & 0x80);  // Set carry if MSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.A = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}
void SRA_B(CPU& cpu) {
    uint8_t value = cpu.B;  // Get the current value of register B
    uint8_t result = value >> 1;  // Perform an arithmetic shift right
    cpu.setCarryFlag(value & 0x01);  // Set carry if the LSB is 1
    cpu.setZeroFlag(result == 0);  // Set zero if the result is 0
    cpu.setHalfCarryFlag(false);  // No effect on half-carry
    cpu.setSubtractFlag(false);  // No effect on subtract flag
    cpu.B = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRA_C(CPU& cpu) {
    uint8_t value = cpu.C;  // Get the current value of register C
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);  // Set carry if the LSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.C = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRA_D(CPU& cpu) {
    uint8_t value = cpu.D;  // Get the current value of register D
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);  // Set carry if the LSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.D = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRA_E(CPU& cpu) {
    uint8_t value = cpu.E;  // Get the current value of register E
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);  // Set carry if the LSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.E = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRA_H(CPU& cpu) {
    uint8_t value = cpu.H;  // Get the current value of register H
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);  // Set carry if the LSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.H = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRA_L(CPU& cpu) {
    uint8_t value = cpu.L;  // Get the current value of register L
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);  // Set carry if the LSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.L = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRA_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();  // Get memory address from HL
    uint8_t value = cpu.read8(address);  // Read value from memory
    uint8_t result = value >> 1;  // Perform arithmetic shift right
    cpu.setCarryFlag(value & 0x01);  // Set carry if the LSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.writeMemory(address, result);  // Write the result back to memory
    cpu.PC += 2;
    cpu.updateCycles(16);  // Takes longer to access memory
}

void SRA_A(CPU& cpu) {
    uint8_t value = cpu.A;  // Get the current value of register A
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);  // Set carry if the LSB is 1
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.A = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}
void SWAP_B(CPU& cpu) {
    uint8_t value = cpu.B;
    uint8_t result = (value << 4) | (value >> 4);  // Swap the nibbles
    cpu.setZeroFlag(result == 0);  // Set zero flag if the result is 0
    cpu.setCarryFlag(false);  // No carry generated
    cpu.setHalfCarryFlag(false);  // No half-carry
    cpu.setSubtractFlag(false);  // No subtraction
    cpu.B = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SWAP_C(CPU& cpu) {
    uint8_t value = cpu.C;
    uint8_t result = (value << 4) | (value >> 4);  // Swap the nibbles
    cpu.setZeroFlag(result == 0);
    cpu.setCarryFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.C = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SWAP_D(CPU& cpu) {
    uint8_t value = cpu.D;
    uint8_t result = (value << 4) | (value >> 4);  // Swap the nibbles
    cpu.setZeroFlag(result == 0);
    cpu.setCarryFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.D = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SWAP_E(CPU& cpu) {
    uint8_t value = cpu.E;
    uint8_t result = (value << 4) | (value >> 4);  // Swap the nibbles
    cpu.setZeroFlag(result == 0);
    cpu.setCarryFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.E = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SWAP_H(CPU& cpu) {
    uint8_t value = cpu.H;
    uint8_t result = (value << 4) | (value >> 4);  // Swap the nibbles
    cpu.setZeroFlag(result == 0);
    cpu.setCarryFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.H = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SWAP_L(CPU& cpu) {
    uint8_t value = cpu.L;
    uint8_t result = (value << 4) | (value >> 4);  // Swap the nibbles
    cpu.setZeroFlag(result == 0);
    cpu.setCarryFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.L = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SWAP_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();  // Get memory address from HL
    uint8_t value = cpu.read8(address);  // Read value from memory
    uint8_t result = (value << 4) | (value >> 4);  // Swap the nibbles
    cpu.setZeroFlag(result == 0);
    cpu.setCarryFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.writeMemory(address, result);  // Write the result back to memory
    cpu.PC += 2;
    cpu.updateCycles(16);  // Takes longer to access memory
}

void SWAP_A(CPU& cpu) {
    uint8_t value = cpu.A;
    uint8_t result = (value << 4) | (value >> 4);  // Swap the nibbles
    cpu.setZeroFlag(result == 0);
    cpu.setCarryFlag(false);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.A = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}
void SRL_B(CPU& cpu) {
    uint8_t value = cpu.B;
    uint8_t result = value >> 1;  // Perform logical shift right
    cpu.setCarryFlag(value & 0x01);  // Set carry if the LSB is 1
    cpu.setZeroFlag(result == 0);  // Set zero flag if the result is 0
    cpu.setHalfCarryFlag(false);  // No effect on half-carry
    cpu.setSubtractFlag(false);  // No effect on subtract flag
    cpu.B = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRL_C(CPU& cpu) {
    uint8_t value = cpu.C;
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.C = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRL_D(CPU& cpu) {
    uint8_t value = cpu.D;
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.D = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRL_E(CPU& cpu) {
    uint8_t value = cpu.E;
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.E = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRL_H(CPU& cpu) {
    uint8_t value = cpu.H;
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.H = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRL_L(CPU& cpu) {
    uint8_t value = cpu.L;
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.L = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void SRL_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);  // Read value from memory
    uint8_t result = value >> 1;  // Perform logical shift right
    cpu.setCarryFlag(value & 0x01);
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.writeMemory(address, result);  // Write the result back to memory
    cpu.PC += 2;
    cpu.updateCycles(16);  // Takes longer to access memory
}

void SRL_A(CPU& cpu) {
    uint8_t value = cpu.A;
    uint8_t result = value >> 1;
    cpu.setCarryFlag(value & 0x01);
    cpu.setZeroFlag(result == 0);
    cpu.setHalfCarryFlag(false);
    cpu.setSubtractFlag(false);
    cpu.A = result;
    cpu.PC += 2;
    cpu.updateCycles(8);
}
void BIT_0_B(CPU& cpu) {
    uint8_t value = cpu.B;
    bool bitSet = (value & 0x01) != 0;  // Check if bit 0 is set
    cpu.setZeroFlag(!bitSet);  // Set zero flag if bit 0 is 0
    cpu.setHalfCarryFlag(true);  // Always set Half Carry flag for BIT instructions
    cpu.setSubtractFlag(false);  // No subtraction
    cpu.setCarryFlag(false);  // Carry flag is unaffected
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_0_C(CPU& cpu) {
    uint8_t value = cpu.C;
    bool bitSet = (value & 0x01) != 0;
    cpu.setZeroFlag(!bitSet);
    cpu.setHalfCarryFlag(true);
    cpu.setSubtractFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_0_D(CPU& cpu) {
    uint8_t value = cpu.D;
    bool bitSet = (value & 0x01) != 0;
    cpu.setZeroFlag(!bitSet);
    cpu.setHalfCarryFlag(true);
    cpu.setSubtractFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_0_E(CPU& cpu) {
    uint8_t value = cpu.E;
    bool bitSet = (value & 0x01) != 0;
    cpu.setZeroFlag(!bitSet);
    cpu.setHalfCarryFlag(true);
    cpu.setSubtractFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_0_H(CPU& cpu) {
    uint8_t value = cpu.H;
    bool bitSet = (value & 0x01) != 0;
    cpu.setZeroFlag(!bitSet);
    cpu.setHalfCarryFlag(true);
    cpu.setSubtractFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_0_L(CPU& cpu) {
    uint8_t value = cpu.L;
    bool bitSet = (value & 0x01) != 0;
    cpu.setZeroFlag(!bitSet);
    cpu.setHalfCarryFlag(true);
    cpu.setSubtractFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_0_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);  // Read value from memory
    bool bitSet = (value & 0x01) != 0;
    cpu.setZeroFlag(!bitSet);
    cpu.setHalfCarryFlag(true);
    cpu.setSubtractFlag(false);
    cpu.setCarryFlag(false);
    cpu.PC += 2;
    cpu.updateCycles(12);  // Takes longer due to memory access
}

void BIT_0_A(CPU& cpu) {
    cpu.BIT(0, cpu.A);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

// Repeat the same for BIT 1 to 7

void BIT_1_B(CPU& cpu) {
    cpu.BIT(1, cpu.B);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_1_C(CPU& cpu) {
    cpu.BIT(1, cpu.C);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_1_D(CPU& cpu) {
    cpu.BIT(1, cpu.D);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_1_E(CPU& cpu) {
    cpu.BIT(1, cpu.E);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_1_H(CPU& cpu) {
    cpu.BIT(1, cpu.H);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_1_L(CPU& cpu) {
    cpu.BIT(1, cpu.L);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void BIT_1_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);
    bool bitSet = (value & 0x02) != 0;
    cpu.setZeroFlag(!bitSet);
    cpu.setHalfCarryFlag(true);
    cpu.setSubtractFlag(false);

    cpu.PC += 2;
    cpu.updateCycles(12);  // Memory access takes longer
}

void BIT_1_A(CPU& cpu) {
    cpu.BIT(1, cpu.A);
    cpu.PC += 2;
    cpu.updateCycles(8);
}
void BIT_2_B(CPU& cpu) {
    cpu.BIT(2, cpu.B);
    cpu.PC+=2;
    cpu.updateCycles(8);


}
void BIT_2_C(CPU& cpu) {
    cpu.BIT(2, cpu.C);
    cpu.PC+=2;
    cpu.updateCycles(8);


}
void BIT_2_D(CPU& cpu) {
    cpu.BIT(2, cpu.D);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_2_E(CPU& cpu) {
    cpu.BIT(2, cpu.E);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_2_H(CPU& cpu) {
    cpu.BIT(2, cpu.H);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_2_L(CPU& cpu) {
    cpu.BIT(2, cpu.L);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_2_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);
    cpu.BIT(2, value);
    cpu.PC += 2;
    cpu.updateCycles(12);

    
}
void BIT_2_A(CPU& cpu) {
    cpu.BIT(2, cpu.A);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_3_B(CPU& cpu) {
    cpu.BIT(3, cpu.B);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_3_C(CPU& cpu) {
    cpu.BIT(3, cpu.C);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_3_D(CPU& cpu) {
    cpu.BIT(3, cpu.D);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_3_E(CPU& cpu) {
    cpu.BIT(3, cpu.E);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_3_H(CPU& cpu) {
    cpu.BIT(3, cpu.H);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_3_L(CPU& cpu) {
    cpu.BIT(3, cpu.L);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_3_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);
    cpu.BIT(3, value);
    cpu.PC += 2;
    cpu.updateCycles(12);

    
}
void BIT_3_A(CPU& cpu) {
    cpu.BIT(3, cpu.A);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_4_B(CPU& cpu) {
    cpu.BIT(4, cpu.B);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_4_C(CPU& cpu) {
    cpu.BIT(4, cpu.C);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_4_D(CPU& cpu) {
    cpu.BIT(4, cpu.D);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}

void BIT_4_E(CPU& cpu) {
    cpu.BIT(4, cpu.E);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_4_H(CPU& cpu) {
    cpu.BIT(4, cpu.H);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_4_L(CPU& cpu) {
    cpu.BIT(4, cpu.L);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_4_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);
    cpu.BIT(4, value);
    cpu.PC+=2;
    cpu.updateCycles(12);

    
}
void BIT_4_A(CPU& cpu) {
    cpu.BIT(4, cpu.A);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_5_B(CPU& cpu) {
    cpu.BIT(5, cpu.B);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_5_C(CPU& cpu) {
    cpu.BIT(5, cpu.C);
    cpu.PC+=2;
    cpu.updateCycles(8);

    
}
void BIT_5_D(CPU& cpu) {
    cpu.BIT(5, cpu.D);
    cpu.PC+=2;
    cpu.updateCycles(8);
    
}

void BIT_5_E(CPU& cpu) {
    cpu.BIT(5, cpu.E);
    cpu.PC+=2;
    cpu.updateCycles(8);
    
}
void BIT_5_H(CPU& cpu) {
    cpu.BIT(5, cpu.L);
    cpu.PC+=2;
    cpu.updateCycles(8);
    
}
void BIT_5_L(CPU& cpu) {
    cpu.BIT(5, cpu.L);
    cpu.PC+=2;
    cpu.updateCycles(8);
    
}

void BIT_5_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);
    cpu.BIT(5, value);
    cpu.PC+=2;
    cpu.updateCycles(12);
    
}
void BIT_5_A(CPU& cpu) {
    cpu.BIT(5, cpu.A);
    cpu.PC+=2;
    cpu.updateCycles(8);
    
}


void BIT_6_B(CPU& cpu) {
    cpu.BIT(6, cpu.B);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_6_C(CPU& cpu) {
    cpu.BIT(6, cpu.C);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_6_D(CPU& cpu) {
    cpu.BIT(6, cpu.D);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_6_E(CPU& cpu) {
    cpu.BIT(6, cpu.E);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_6_H(CPU& cpu) {
    cpu.BIT(6, cpu.H);
    cpu.PC+=2;
    cpu.updateCycles(8);
}

void BIT_6_L(CPU& cpu) {
    cpu.BIT(6, cpu.L);
    cpu.PC+=2;
    cpu.updateCycles(8);
}

void BIT_6_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);
    cpu.BIT(6, value);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_6_A(CPU& cpu) {
    cpu.BIT(6, cpu.A);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_7_B(CPU& cpu) {
    cpu.BIT(7, cpu.B);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_7_C(CPU& cpu) {
    cpu.BIT(7, cpu.C);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_7_D(CPU& cpu) {
    cpu.BIT(7, cpu.D);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_7_E(CPU& cpu) {
    cpu.BIT(7, cpu.E);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_7_H(CPU& cpu) {
    cpu.BIT(7, cpu.H);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_7_L(CPU& cpu) {
    cpu.BIT(7, cpu.L);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_7_bracket_HL(CPU& cpu) {
    uint16_t address = cpu.getHL();
    uint8_t value = cpu.read8(address);
    cpu.BIT(7, value);
    cpu.PC+=2;
    cpu.updateCycles(8);
}
void BIT_7_A(CPU& cpu) {
    cpu.BIT(7, cpu.A);
    cpu.PC+=2;
    cpu.updateCycles(8);
}




void RES_0_B(CPU& cpu) { cpu.RES_n_X(cpu, 0, cpu.B); }
void RES_0_C(CPU& cpu) { cpu.RES_n_X(cpu, 0, cpu.C); }
void RES_0_D(CPU& cpu) { cpu.RES_n_X(cpu, 0, cpu.D); }
void RES_0_E(CPU& cpu) { cpu.RES_n_X(cpu, 0, cpu.E); }
void RES_0_H(CPU& cpu) { cpu.RES_n_X(cpu, 0, cpu.L); }
void RES_0_L(CPU& cpu) { cpu.RES_n_X(cpu, 0, cpu.L); }
void RES_0_bracket_HL(CPU& cpu) { cpu.RES_n_HL(cpu, 0); }
void RES_0_A(CPU& cpu) { cpu.RES_n_X(cpu, 0, cpu.A); }

void RES_1_B(CPU& cpu) { cpu.RES_n_X(cpu, 1, cpu.B); }                                          
void RES_1_C(CPU& cpu) { cpu.RES_n_X(cpu, 1, cpu.C); }
void RES_1_D(CPU& cpu) { cpu.RES_n_X(cpu, 1, cpu.D); }
void RES_1_E(CPU& cpu) { cpu.RES_n_X(cpu, 1, cpu.E); }
void RES_1_H(CPU& cpu) { cpu.RES_n_X(cpu, 1, cpu.H); }
void RES_1_L(CPU& cpu) { cpu.RES_n_X(cpu, 1, cpu.L); }
void RES_1_bracket_HL(CPU& cpu) { cpu.RES_n_HL(cpu, 1); }
void RES_1_A(CPU& cpu) { cpu.RES_n_X(cpu, 1, cpu.A); }

void RES_2_B(CPU& cpu) { cpu.RES_n_X(cpu, 2, cpu.B); }
void RES_2_C(CPU& cpu) { cpu.RES_n_X(cpu, 2, cpu.C); }
void RES_2_D(CPU& cpu) { cpu.RES_n_X(cpu, 2, cpu.D); }
void RES_2_E(CPU& cpu) { cpu.RES_n_X(cpu, 2, cpu.E); }
void RES_2_H(CPU& cpu) { cpu.RES_n_X(cpu, 2, cpu.H); }
void RES_2_L(CPU& cpu) { cpu.RES_n_X(cpu, 2, cpu.L); }
void RES_2_bracket_HL(CPU& cpu) { cpu.RES_n_HL(cpu, 2); }
void RES_2_A(CPU& cpu) {cpu.RES_n_X(cpu, 2, cpu.A); }


void RES_3_B(CPU& cpu) { cpu.RES_n_X(cpu, 3, cpu.B); }
void RES_3_C(CPU& cpu) { cpu.RES_n_X(cpu, 3, cpu.C); }
void RES_3_D(CPU& cpu) { cpu.RES_n_X(cpu, 3, cpu.D); }
void RES_3_E(CPU& cpu) { cpu.RES_n_X(cpu, 3, cpu.E); }
void RES_3_H(CPU& cpu) { cpu.RES_n_X(cpu, 3, cpu.H); }
void RES_3_L(CPU& cpu) { cpu.RES_n_X(cpu, 3, cpu.L); }
void RES_3_bracket_HL(CPU& cpu) { cpu.RES_n_HL(cpu, 3); }
void RES_3_A(CPU& cpu) { cpu.RES_n_X(cpu, 3, cpu.A); }

void RES_4_B(CPU& cpu) { cpu.RES_n_X(cpu, 4, cpu.B); }
void RES_4_C(CPU& cpu) { cpu.RES_n_X(cpu, 4, cpu.C); }
void RES_4_D(CPU& cpu) { cpu.RES_n_X(cpu, 4, cpu.D); }
void RES_4_E(CPU& cpu) { cpu.RES_n_X(cpu, 4, cpu.E); }
void RES_4_H(CPU& cpu) { cpu.RES_n_X(cpu, 4, cpu.H); }
void RES_4_L(CPU& cpu) { cpu.RES_n_X(cpu, 4, cpu.L); }
void RES_4_bracket_HL(CPU& cpu) { cpu.RES_n_HL(cpu, 4); }
void RES_4_A(CPU& cpu) { cpu.RES_n_X(cpu, 5, cpu.A); }

void RES_5_B(CPU& cpu) { cpu.RES_n_X(cpu, 5, cpu.B); }
void RES_5_C(CPU& cpu) { cpu.RES_n_X(cpu, 5, cpu.C); }
void RES_5_D(CPU& cpu) { cpu.RES_n_X(cpu, 5, cpu.D); }
void RES_5_E(CPU& cpu) { cpu.RES_n_X(cpu, 5, cpu.E); }
void RES_5_H(CPU& cpu) { cpu.RES_n_X(cpu, 5, cpu.H); }
void RES_5_L(CPU& cpu) { cpu.RES_n_X(cpu, 5, cpu.L); }
void RES_5_bracket_HL(CPU& cpu) { cpu.RES_n_HL(cpu, 5); }
void RES_5_A(CPU& cpu) { cpu.RES_n_X(cpu, 5, cpu.A); }

void RES_6_B(CPU& cpu) { cpu.RES_n_X(cpu, 6, cpu.B); }
void RES_6_C(CPU& cpu) { cpu.RES_n_X(cpu, 6, cpu.C); }
void RES_6_D(CPU& cpu) { cpu.RES_n_X(cpu, 6, cpu.D); }
void RES_6_E(CPU& cpu) { cpu.RES_n_X(cpu, 6, cpu.E); }
void RES_6_H(CPU& cpu) { cpu.RES_n_X(cpu, 6, cpu.H); }
void RES_6_L(CPU& cpu) { cpu.RES_n_X(cpu, 6, cpu.L); }
void RES_6_bracket_HL(CPU& cpu) { cpu.RES_n_HL(cpu, 6); }
void RES_6_A(CPU& cpu) { cpu.RES_n_X(cpu, 6, cpu.A); }

void RES_7_B(CPU& cpu) { cpu.RES_n_X(cpu, 7, cpu.B); }
void RES_7_C(CPU& cpu) { cpu.RES_n_X(cpu, 7, cpu.C); }
void RES_7_D(CPU& cpu) { cpu.RES_n_X(cpu, 7, cpu.D); }
void RES_7_E(CPU& cpu) { cpu.RES_n_X(cpu, 7, cpu.E); }
void RES_7_H(CPU& cpu) { cpu.RES_n_X(cpu, 7, cpu.H); }
void RES_7_L(CPU& cpu) { cpu.RES_n_X(cpu, 7, cpu.L); }
void RES_7_bracket_HL(CPU& cpu) { cpu.RES_n_HL(cpu, 7); }
void RES_7_A(CPU& cpu) { cpu.RES_n_X(cpu, 7, cpu.A); }


void SET_0_B(CPU& cpu) { cpu.SET_n_X(cpu, 0, cpu.B); }
void SET_0_C(CPU& cpu) { cpu.SET_n_X(cpu, 0, cpu.C); }
void SET_0_D(CPU& cpu) { cpu.SET_n_X(cpu, 0, cpu.D); }
void SET_0_E(CPU& cpu) { cpu.SET_n_X(cpu, 0, cpu.E); }
void SET_0_H(CPU& cpu) { cpu.SET_n_X(cpu, 0, cpu.H); }
void SET_0_L(CPU& cpu) { cpu.SET_n_X(cpu, 0, cpu.L); }
void SET_0_bracket_HL(CPU& cpu) { cpu.SET_n_HL(cpu, 0); }
void SET_0_A(CPU& cpu) { cpu.SET_n_X(cpu, 0, cpu.A); }

void SET_1_B(CPU& cpu) { cpu.SET_n_X(cpu, 1, cpu.B); }
void SET_1_C(CPU& cpu) { cpu.SET_n_X(cpu, 1, cpu.C); }
void SET_1_D(CPU& cpu) { cpu.SET_n_X(cpu, 1, cpu.D); }
void SET_1_E(CPU& cpu) { cpu.SET_n_X(cpu, 1, cpu.E); }
void SET_1_H(CPU& cpu) { cpu.SET_n_X(cpu, 1, cpu.H); }
void SET_1_L(CPU& cpu) { cpu.SET_n_X(cpu, 1, cpu.L); }
void SET_1_bracket_HL(CPU& cpu) { cpu.SET_n_HL(cpu, 1); }
void SET_1_A(CPU& cpu) { cpu.SET_n_X(cpu, 1, cpu.A); }


void SET_2_B(CPU& cpu) { cpu.SET_n_X(cpu, 2, cpu.B); }
void SET_2_C(CPU& cpu) { cpu.SET_n_X(cpu, 2, cpu.C); }
void SET_2_D(CPU& cpu) { cpu.SET_n_X(cpu, 2, cpu.D); }
void SET_2_E(CPU& cpu) { cpu.SET_n_X(cpu, 2, cpu.E); }
void SET_2_H(CPU& cpu) { cpu.SET_n_X(cpu, 2, cpu.H); }
void SET_2_L(CPU& cpu) { cpu.SET_n_X(cpu, 2, cpu.L); }
void SET_2_bracket_HL(CPU& cpu) { cpu.SET_n_HL(cpu, 2); }
void SET_2_A(CPU& cpu) { cpu.SET_n_X(cpu, 2, cpu.A); }

void SET_3_B(CPU& cpu) { cpu.SET_n_X(cpu, 3, cpu.B); }
void SET_3_C(CPU& cpu) { cpu.SET_n_X(cpu, 3, cpu.C); }
void SET_3_D(CPU& cpu) { cpu.SET_n_X(cpu, 3, cpu.D); }
void SET_3_E(CPU& cpu) { cpu.SET_n_X(cpu, 3, cpu.E); }
void SET_3_H(CPU& cpu) { cpu.SET_n_X(cpu, 3, cpu.H); }
void SET_3_L(CPU& cpu) { cpu.SET_n_X(cpu, 3, cpu.L); }
void SET_3_bracket_HL(CPU& cpu) { cpu.SET_n_HL(cpu, 3); }
void SET_3_A(CPU& cpu) { cpu.SET_n_X(cpu, 3, cpu.A); }


void SET_4_B(CPU& cpu) { cpu.SET_n_X(cpu, 4, cpu.B); }
void SET_4_C(CPU& cpu) { cpu.SET_n_X(cpu, 4, cpu.C); }
void SET_4_D(CPU& cpu) { cpu.SET_n_X(cpu, 4, cpu.D); }
void SET_4_E(CPU& cpu) { cpu.SET_n_X(cpu, 4, cpu.E); }
void SET_4_H(CPU& cpu) { cpu.SET_n_X(cpu, 4, cpu.H); }
void SET_4_L(CPU& cpu) { cpu.SET_n_X(cpu, 4, cpu.L); }
void SET_4_bracket_HL(CPU& cpu) { cpu.SET_n_HL(cpu, 4); }
void SET_4_A(CPU& cpu) { cpu.SET_n_X(cpu, 4, cpu.A); }

void SET_5_B(CPU& cpu) { cpu.SET_n_X(cpu, 5, cpu.B); }
void SET_5_C(CPU& cpu) { cpu.SET_n_X(cpu, 5, cpu.C); }
void SET_5_D(CPU& cpu) { cpu.SET_n_X(cpu, 5, cpu.D); }
void SET_5_E(CPU& cpu) { cpu.SET_n_X(cpu, 5, cpu.E); }
void SET_5_H(CPU& cpu) { cpu.SET_n_X(cpu, 5, cpu.H); }
void SET_5_L(CPU& cpu) { cpu.SET_n_X(cpu, 5, cpu.L); }
void SET_5_bracket_HL(CPU& cpu) { cpu.SET_n_HL(cpu, 5); }
void SET_5_A(CPU& cpu) { cpu.SET_n_X(cpu, 5, cpu.A); }


void SET_6_B(CPU& cpu) { cpu.SET_n_X(cpu, 6, cpu.B); }
void SET_6_C(CPU& cpu) { cpu.SET_n_X(cpu, 6, cpu.C); }
void SET_6_D(CPU& cpu) { cpu.SET_n_X(cpu, 6, cpu.D); }
void SET_6_E(CPU& cpu) { cpu.SET_n_X(cpu, 6, cpu.E); }
void SET_6_H(CPU& cpu) { cpu.SET_n_X(cpu, 6, cpu.H); }
void SET_6_L(CPU& cpu) { cpu.SET_n_X(cpu, 6, cpu.L); }
void SET_6_bracket_HL(CPU& cpu) { cpu.SET_n_HL(cpu, 6); }
void SET_6_A(CPU& cpu) { cpu.SET_n_X(cpu, 6, cpu.A); }

void SET_7_B(CPU& cpu) { cpu.SET_n_X(cpu, 7, cpu.B); }
void SET_7_C(CPU& cpu) { cpu.SET_n_X(cpu, 7, cpu.C); }
void SET_7_D(CPU& cpu) { cpu.SET_n_X(cpu, 7, cpu.D); }
void SET_7_E(CPU& cpu) { cpu.SET_n_X(cpu, 7, cpu.E); }
void SET_7_H(CPU& cpu) { cpu.SET_n_X(cpu, 7, cpu.H); }
void SET_7_L(CPU& cpu) { cpu.SET_n_X(cpu, 7, cpu.L); }
void SET_7_bracket_HL(CPU& cpu) { cpu.SET_n_HL(cpu, 7); }
void SET_7_A(CPU& cpu) { cpu.SET_n_X(cpu, 7, cpu.A); }





