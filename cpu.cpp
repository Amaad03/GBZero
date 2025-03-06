#include "cpu.h"
#include "opcode_handlers.h"
#include <iostream>

CPU::CPU() {
    A = B = C = F = D = E = H = L = 0;  // Initialize 8-bit registers and Flag
    SP = 0xFFFE;  // Stack Pointer
    PC = 0x0100;
    cycleCount = 0;
    interruptsEnabled = false;

    reset();
    initOpcodeTable();
}

void CPU::initOpcodeTable() {
    std::fill(std::begin(opcodeTable), std::end(opcodeTable), &UNIMPLEMENTED);  

    opcodeTable[0x00] = NOP;
    opcodeTable[0x01] = LD_BC_d16;
    opcodeTable[0x02] = &LD_BC_A;
    opcodeTable[0x03] = &INC_BC;
    opcodeTable[0x04] = &INC_B;
    opcodeTable[0x05] = &DEC_B;
    opcodeTable[0x06] = &LD_B_d8;
    opcodeTable[0x07] = &RLCA;
    opcodeTable[0x08] = &LD_a16_SP;
    opcodeTable[0x09] = &ADD_HL_BC;
    opcodeTable[0x0A] = &LD_A_BC;
    opcodeTable[0x0B] = &DEC_BC;
    opcodeTable[0x0C] = &INC_C;
    opcodeTable[0x0D] = &DEC_C;
    opcodeTable[0x0E] = &LD_C_n8;
    opcodeTable[0x0F] = &RRCA;
    opcodeTable[0xC3] = &JP_a16;
    opcodeTable[0xD9] = &RETI;
    opcodeTable[0x10] = &STOP_n8; //0x10
    opcodeTable[0x11] = LD_DE_n16; //0x11
    opcodeTable[0x12] = LD_DE_A;//0x12
    opcodeTable[0x13] = INC_DE; //0x13
    opcodeTable[0x14] = INC_D; //0x14
    opcodeTable[0x15] = DEC_D; //0x15
    opcodeTable[0x16] = LD_D_n8; //0x16
    opcodeTable[0x17] = RLA; //0x17
    opcodeTable[0x18] = JR_e8; //0x18
    opcodeTable[0x19] = ADD_HL_DE; //0x19
    opcodeTable[0x1A] = LD_A_DE; //0x1A
    opcodeTable[0x1B] = DEC_DE; //0x1B
    opcodeTable[0x1C] = INC_E; //0x1C
    opcodeTable[0x1D] = DEC_E; //0x1D
    opcodeTable[0x1E] = LD_E_n8; // 0x1E
    opcodeTable[0x1F] = RRA; //0x1F

    //----------------------------
    opcodeTable[0x20] = JR_NZ_e8;
    opcodeTable[0x21] = LD_HL_n16;
    opcodeTable[0x22] = LD_HLplus_A;
    opcodeTable[0x23] = INC_HL;
    opcodeTable[0x24] = INC_H;
    opcodeTable[0x25] = DEC_H;
    opcodeTable[0x26] = LD_H_n8;
    opcodeTable[0x27] = DAA;
    opcodeTable[0x28] = JRZ_e8;
    opcodeTable[0x29] = ADD_HL_HL;
    opcodeTable[0x2A] = LD_A_HLplus;
    opcodeTable[0x2B] =DEC_HL;
    opcodeTable[0x2C] = INC_L;
    opcodeTable[0x2D] = DEC_L;
    opcodeTable[0x2E] = LD_L_n8;
    opcodeTable[0x2F] = CPL;
    //------------------------
    opcodeTable[0x30] = JR_NZ_e8;   // 0x30 - JR NZ, e8
    opcodeTable[0x31] = LD_SP_n16;  // 0x31 - LD SP, n16
    opcodeTable[0x32] = LD_HL_minus_A; // 0x32 - LD (HL-), A
    opcodeTable[0x33] = INC_SP;     // 0x33 - INC SP
    opcodeTable[0x34] = INC_HL_mem;     // 0x34 - INC HL
    opcodeTable[0x35] = DEC_HL_mem;     // 0x35 - DEC HL
    opcodeTable[0x36] = LD_HL_n8;   // 0x36 - LD HL, n8
    opcodeTable[0x37] = SCF;        // 0x37 - SCF
    opcodeTable[0x38] = JR_C_e8;    // 0x38 - JR C, e8
    opcodeTable[0x39] = ADD_HL_SP;  // 0x39 - ADD HL, SP
    opcodeTable[0x3A] = LD_A_HL_minus; // 0x3A - LD A, (HL-)
    opcodeTable[0x3B] = DEC_SP;     // 0x3B - DEC SP
    opcodeTable[0x3C] = INC_A;      // 0x3C - INC A
    opcodeTable[0x3D] = DEC_A;      // 0x3D - DEC A
    opcodeTable[0x3E] = LD_A_n8;    // 0x3E - LD A, n8
    opcodeTable[0x3F] = CCF;        // 0x3F - CCF
    //-------------------- 
    opcodeTable[0x40] = LD_B_B;
    opcodeTable[0x41] = LD_B_C;
    opcodeTable[0x42] = LD_B_D;
    opcodeTable[0x43] = LD_B_E;
    opcodeTable[0x44] = LD_B_H;
    opcodeTable[0x45] = LD_B_L;
    opcodeTable[0x46] = LD_B_HL;
    opcodeTable[0x47] = LD_B_A;

    opcodeTable[0x48] = LD_C_B;
    opcodeTable[0x49] = LD_C_C;
    opcodeTable[0x4A] = LD_C_D;
    opcodeTable[0x4B] = LD_C_E;
    opcodeTable[0x4C] = LD_C_H;
    opcodeTable[0x4D] = LD_C_L;
    opcodeTable[0x4E] = LD_C_HL;
    opcodeTable[0x4F] = LD_C_A;

    opcodeTable[0x50] = LD_D_B;
    opcodeTable[0x51] = LD_D_C;
    opcodeTable[0x52] = LD_D_D;
    opcodeTable[0x53] = LD_D_E;
    opcodeTable[0x54] = LD_D_H;
    opcodeTable[0x55] = LD_D_L;
    opcodeTable[0x56] = LD_D_HL;
    opcodeTable[0x57] = LD_D_A;

    // LD E, B to LD E, A
    opcodeTable[0x58] = LD_E_B;
    opcodeTable[0x59] = LD_E_C;
    opcodeTable[0x5A] = LD_E_D;
    opcodeTable[0x5B] = LD_E_E;
    opcodeTable[0x5C] = LD_E_H;
    opcodeTable[0x5D] = LD_E_L;
    opcodeTable[0x5E] = LD_E_HL;
    opcodeTable[0x5F] = LD_E_A;

    opcodeTable[0x60] = LD_H_B;
    opcodeTable[0x61] = LD_H_C;
    opcodeTable[0x62] = LD_H_D;
    opcodeTable[0x63] = LD_H_E;
    opcodeTable[0x64] = LD_H_H;
    opcodeTable[0x65] = LD_H_L;
    opcodeTable[0x66] = LD_H_HL;
    opcodeTable[0x67] = LD_H_A;

    // LD L, B to LD L, A
    opcodeTable[0x68] = LD_L_B;
    opcodeTable[0x69] = LD_L_C;
    opcodeTable[0x6A] = LD_L_D;
    opcodeTable[0x6B] = LD_L_E;
    opcodeTable[0x6C] = LD_L_H;
    opcodeTable[0x6D] = LD_L_L;
    opcodeTable[0x6E] = LD_L_HL;
    opcodeTable[0x6F] = LD_L_A;

    opcodeTable[0x70] = LD_HL_B;
    opcodeTable[0x71] = LD_HL_C;
    opcodeTable[0x72] = LD_HL_D;
    opcodeTable[0x73] = LD_HL_E;
    opcodeTable[0x74] = LD_HL_H;
    opcodeTable[0x75] = LD_HL_L;
    opcodeTable[0x76] = HALT;  // HALT instruction
    opcodeTable[0x77] = LD_HL_A;
    opcodeTable[0x78] = LD_A_B;
    opcodeTable[0x79] = LD_A_C;
    opcodeTable[0x7A] = LD_A_D;
    opcodeTable[0x7B] = LD_A_E;
    opcodeTable[0x7C] = LD_A_H;
    opcodeTable[0x7D] = LD_A_L;
    opcodeTable[0x7E] = LD_A_HL;
    opcodeTable[0x7F] = LD_A_A;


   
    //8x row
    opcodeTable[0x80] = ADD_A_B;  
    opcodeTable[0x81] = ADD_A_C; 
    opcodeTable[0x82] = ADD_A_D;    
    opcodeTable[0x83] = ADD_A_E;    
    opcodeTable[0x84] = ADD_A_H;
    opcodeTable[0x85] = ADD_A_L;   
    opcodeTable[0x86] = ADD_A_HL;  
    opcodeTable[0x87] = ADD_A_A;   
    opcodeTable[0x88] = ADC_A_B;  
    opcodeTable[0x89] = ADC_A_C ;   
    opcodeTable[0x8A] = ADC_A_D  ; 
    opcodeTable[0x8B] = ADC_A_E  ;
    opcodeTable[0x8C] = ADC_A_H  ; 
    opcodeTable[0x8D] = ADC_A_L   ; 
    opcodeTable[0x8E] = ADC_A_HL  ; 
    opcodeTable[0x8F] = ADC_A_A   ;





    //9x row
    opcodeTable[0x90] = &SUB_A_B;
    opcodeTable[0x91] = SUB_A_C;
    opcodeTable[0x92] = SUB_A_D;
    opcodeTable[0x93] =SUB_A_E;
    opcodeTable[0x94] = SUB_A_H;
    opcodeTable[0x95] = SUB_A_L;
    opcodeTable[0x96] = SUB_A_memHL;
    opcodeTable[0x97] = SUB_A_A;
    opcodeTable[0x98] = SBC_A_B;
    opcodeTable[0x99] = SBC_A_C;
    opcodeTable[0x9A] =SBC_A_D;
    opcodeTable[0x9B] =SBC_A_E;
    opcodeTable[0x9C] = SBC_A_H;
    opcodeTable[0x9D] = SBC_A_L;
    opcodeTable[0x9E] = SBC_A_memHL;
    opcodeTable[0x9F] =SBC_A_A;

    //Ax row

    opcodeTable[0xA0] = AND_A_B;
    opcodeTable[0xA1] = AND_A_C;
    opcodeTable[0xA2] = AND_A_D;
    opcodeTable[0xA3] = AND_A_E;
    opcodeTable[0xA4] = AND_A_H;
    opcodeTable[0xA5] = AND_A_L;
    opcodeTable[0xA6] = AND_A_memHL;
    opcodeTable[0xA7] = AND_A_A;
    opcodeTable[0xA8] =XOR_A_B;
    opcodeTable[0xA9] =XOR_A_C;
    opcodeTable[0xAA] = XOR_A_D;
    opcodeTable[0xAB] = XOR_A_E;
    opcodeTable[0xAC] = XOR_A_H;
    opcodeTable[0xAD] = XOR_A_L;
    opcodeTable[0xAE] = XOR_A_memHL;
    opcodeTable[0xAF] = XOR_A_A;
    

    opcodeTable[0xB0] = OR_A_B;
    opcodeTable[0xB1] = OR_A_C;
    opcodeTable[0xB2] = OR_A_D;
    opcodeTable[0xB3] = OR_A_E;
    opcodeTable[0xB4] = OR_A_H;
    opcodeTable[0xB5] =OR_A_L;
    opcodeTable[0xB6] = OR_A_memHL;
    opcodeTable[0xB7] =OR_A_A;
    opcodeTable[0xB8] =CP_A_B;
    opcodeTable[0xB9] = CP_A_C;
    opcodeTable[0xBA] =CP_A_D;
    opcodeTable[0xBB] =CP_A_E;
    opcodeTable[0xBC] =CP_A_H;
    opcodeTable[0xBD] = CP_A_L;
    opcodeTable[0xBE] = CP_A_memHL;
    opcodeTable[0xBF] =CP_A_A;


    opcodeTable[0xC0] =RET_NZ;
    opcodeTable[0xC1] =POP_BC;
    opcodeTable[0xC2]=JP_NZ_a16;
    opcodeTable[0xC3] =JP_a16;
    opcodeTable[0xC4] = CALL_NZ_a16;
    opcodeTable[0xC5] = PUSH_BC;
    opcodeTable[0xC6] = ADD_A_n8;
    opcodeTable[0xC7] = RST_00;
    opcodeTable[0xC8] = RET_Z;
    opcodeTable[0xC9] = RET;
    opcodeTable[0xCA] = JP_Z_a16;
    opcodeTable[0xCB] = PREFIX;
    opcodeTable[0xCC] =CALL_Z_a16;
  
    opcodeTable[0xCD] = CALL_a16;
    opcodeTable[0xCE] = ADC_A_n8;
    opcodeTable[0xCF] = RST_08;

    opcodeTable[0xD0] = RET_NC;
    opcodeTable[0xD1] = POP_DE;
    opcodeTable[0xD2] = JP_NC_a16;
    opcodeTable[0xD4] = CALL_NC_a16;
    opcodeTable[0xD5] = PUSH_DE;
    opcodeTable[0xD6] = SUB_A_n8;
    opcodeTable[0xD7] = RST_10;
    opcodeTable[0xD8] = RET_C;
    opcodeTable[0xD9] = RETI;
    opcodeTable[0xDC] = JP_C_a16;
    opcodeTable[0xDD] = CALL_C_a16;
    opcodeTable[0xDE] = SBC_A_n8;
    opcodeTable[0xDF] = RST_18;

    opcodeTable[0xE0] = LDH_a8_A;
    opcodeTable[0xE1] = POP_HL;
    opcodeTable[0xE2] = LDH_C_A;
    opcodeTable[0xE5] = PUSH_HL;
    opcodeTable[0xE6] = AND_A_n8;
    opcodeTable[0xE7] = RST_20;
    opcodeTable[0xE8] = ADD_SP_e8;
    opcodeTable[0xE9] = JP_HL;
    opcodeTable[0xEA] = LD_a16_A;
    opcodeTable[0xEE] = XOR_A_n8;
    opcodeTable[0xEF] = RST_28;
    opcodeTable[0xED] = no_opcode;

    opcodeTable[0xF0] = LDH_A_a8;
    opcodeTable[0xF1] = POP_AF;
    opcodeTable[0xE2] = LDH_A_C;
    opcodeTable[0xF3] = DI;
    opcodeTable[0xF5] = PUSH_AF;
    opcodeTable[0xF6] = OR_A_n8;
    opcodeTable[0xF7] = RST_30;
    opcodeTable[0xF8] = LD_HL_SP_e8;
    opcodeTable[0xF9] = LD_SP_HL;
    opcodeTable[0xFA] = LD_A_a16;
    opcodeTable[0xFB] = EI;
    opcodeTable[0xFE] = CP_A_n8;
    opcodeTable[0xFF] = RST_38;
    // Add more opcodes as needed...
}

void CPU::reset() {
    SP = 0xFFFE;  // Stack Pointer
    PC = 0x0100;  // Program Counter starts at 0x0100
    cycleCount = 0;
}

void UNIMPLEMENTED(CPU& cpu) {
    std::cerr << "Unimplemented opcode: 0x" 
              << std::hex << static_cast<int>(cpu.readMemory(cpu.PC)) << std::endl;
    exit(1);
}

void CPU::executeOpcode(uint8_t opcode) {
    std::cout << "PC: 0x" << std::hex << PC 
              << " | Opcode: 0x" << static_cast<int>(opcode) << std::endl;

    if (opcodeTable[opcode]) {
        opcodeTable[opcode](*this);  // Execute opcode
    } else {
        std::cerr << "Unknown opcode: 0x" << std::hex << static_cast<int>(opcode) << std::endl;
        exit(1);
    }
}
void CPU::executeNextInstruction() {
    uint8_t opcode = readMemory(PC);  // Fetch the opcode from memory
    PC++;  // Increment the program counter to point to the next instruction
    executeOpcode(opcode);  // Execute the opcode
    updateCycles(4);  // Update the cycle count (adjust the cycle count as needed based on the opcode)
}
uint8_t CPU::readMemory(uint16_t addr) {
    return memory.read(addr);
}

void CPU::writeMemory(uint16_t addr, uint8_t value) {
    memory.write(addr, value);
}

uint16_t CPU::read16(uint16_t addr) {
    return memory.read(addr) | (memory.read(addr + 1) << 8);
}
void CPU::write16(uint16_t addr, uint16_t value) {
    memory.write(addr, value & 0xFF);
    memory.write(addr + 1, (value >> 8) & 0xFF);
}

void CPU::updateCycles(uint32_t cycles) {
    cycleCount += cycles;
    std::cout << "[DEBUG] Total cycle count: " << std::hex << cycleCount << std::endl;
}

void CPU::enableInterrupts() {
    interruptsEnabled = true;
    std::cout << "[DEBUG] Interrupts Enabled" << std::endl;
}

void CPU::disableInterrupts() {
    interruptsEnabled = false;
    std::cout << "[DEBUG] Interrupts Disabled" << std::endl;
}

void CPU::pushToStack(uint16_t value) {
    if (SP < 0xFF80) {
        std::cerr << "Stack underflow! SP: 0x" << std::hex << SP << std::endl;
        exit(1);
    }
    SP -= 2;
    write16(SP, value);
}

uint16_t CPU::popFromStack() {
    if (SP >= 0xFFFF) {
        std::cerr << "Stack overflow! SP: 0x" << std::hex << SP << std::endl;
        exit(1);
    }

    uint16_t value = read16(SP);
    SP += 2;

    std::cout << "[DEBUG] Popped 0x" << std::hex << value 
              << " from SP: 0x" << SP << std::endl;

    return value;
}


void CPU::updateAF() { AF = (A << 8) | F; }

void CPU::updateBC() { BC = (B << 8) | C; }

void CPU::updateDE() { DE = (D << 8) | E; }

void CPU::updateHL() { HL = (H << 8) | L; }


void CPU::updateA_F() { A = (AF >> 8) & 0xFF; F = AF & 0xFF; }

void CPU::updateB_C() { B = (BC >> 8) & 0xFF; C = BC & 0xFF; }

void CPU::updateD_E() { D = (DE >> 8) & 0xFF; E = DE & 0xFF; }

void CPU::updateH_L() { H = (HL >> 8) & 0xFF; L = HL & 0xFF; }

void CPU::setZeroFlag(bool value) {
    if (value) F |= (1 << 7);  // Set bit 7 (Zero Flag)
    else F &= ~(1 << 7);       // Clear bit 7
}

void CPU::setSubtractFlag(bool value) {
    if (value) F |= (1 << 6);  // Set bit 6 (Subtract Flag)
    else F &= ~(1 << 6);       // Clear bit 6
}

void CPU::setHalfCarryFlag(bool value) {
    if (value) F |= (1 << 5);  // Set bit 5 (Half Carry Flag)
    else F &= ~(1 << 5);       // Clear bit 5
}

void CPU::setCarryFlag(bool value) {
    if (value) F |= (1 << 4);  // Set bit 4 (Carry Flag)
    else F &= ~(1 << 4);       // Clear bit 4
}



bool CPU::getZeroFlag() {
    return (F & (1 << 7)) != 0;  // Check if Zero flag (bit 7) is set
}

bool CPU::getSubtractFlag() {
    return (F & (1 << 6)) != 0;  // Check if Subtract flag (bit 6) is set
}

bool CPU::getHalfCarryFlag() {
    return (F & (1 << 5)) != 0;  // Check if Half Carry flag (bit 5) is set
}

bool CPU::getCarryFlag() {
    return (F & (1 << 4)) != 0;  // Check if Carry flag (bit 4) is set
}




void CPU::increment8(uint8_t& reg) {
    reg++;
    setZeroFlag(reg == 0);  
    setSubtractFlag(false);   
    setHalfCarryFlag((reg & 0x0F) == 0x0F);  // Half-carry occurs when lower nibble overflows
}

void CPU::decrement8(uint8_t& reg) {
    reg--;  
    setZeroFlag(reg == 0);  
    setSubtractFlag(true);   
    setHalfCarryFlag((reg & 0x0F) == 0x00); // Half-carry occurs when lower nibble underflows
}

void CPU::increment16(uint16_t& reg, uint8_t& high, uint8_t& low) {
    reg++;
    high = (reg >> 8) & 0xFF;
    low = reg & 0xFF;
}

void CPU::decrement16(uint16_t& reg, uint8_t& high, uint8_t& low) {
    reg--;
    high = (reg >> 8) & 0xFF;
    low = reg & 0xFF;

}

uint16_t CPU::pop16() {
    uint16_t value = readMemory(SP) | (readMemory(SP + 1) << 8);
    SP += 2;  // Stack pointer increases by 2 (16-bit value)
    return value;
}

// PUSH16: Pushes a 16-bit value onto the stack.
void CPU::push16(uint16_t value) {
    writeMemory(SP - 1, value & 0xFF);
    writeMemory(SP - 2, (value >> 8) & 0xFF);
    SP -= 2;  // Stack pointer decreases by 2 (16-bit value)
}