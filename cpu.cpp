#include "cpu.h"
#include "opcode_handlers.h"
#include <iostream>
#include <iomanip>
#include "ppu.h"
CPU::CPU(Memory& mem) 
    : A(0), F(0), B(0), C(0), D(0), E(0), H(0), L(0), 
      PC(0x0000), SP(0xFFFE), memory(mem),
      cycleCount(0), interruptsEnabled(false), isStopped(false), interruptsEnableNextInstructions(false) ,  interruptFlags(0x00), interruptEnable(0x00){
    reset();
   
    initOpcodeTable();
    initPreOpcodeTable();
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
    opcodeTable[0x16] = LD_D_d8; //0x16
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
    opcodeTable[0x26] = LD_H_d8;
    opcodeTable[0x27] = DAA;
    opcodeTable[0x28] = JR_Z_e8;
    opcodeTable[0x29] = ADD_HL_HL;
    opcodeTable[0x2A] = LD_A_HLplus;
    opcodeTable[0x2B] =DEC_HL;
    opcodeTable[0x2C] = INC_L;
    opcodeTable[0x2D] = DEC_L;
    opcodeTable[0x2E] = LD_L_n8;
    opcodeTable[0x2F] = CPL;
    //------------------------
    opcodeTable[0x30] = JR_NC_e8;   // 0x30 - JR NZ, e8
    opcodeTable[0x31] = LD_SP_n16;  // 0x31 - LD SP, n16
    opcodeTable[0x32] = LD_HL_minus_A; // 0x32 - LD (HL-), A
    opcodeTable[0x33] = INC_SP;     // 0x33 - INC SP
    opcodeTable[0x34] = INC_HL_mem;     // 0x34 - INC HL
    opcodeTable[0x35] = DEC_HL_mem;     // 0x35 - DEC HL
    opcodeTable[0x36] = LD_HL_d8;   // 0x36 - LD HL, n8
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
    opcodeTable[0x061] = XOR_A_A;
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
    opcodeTable[0xDA] = JP_C_a16;
    opcodeTable[0xDD] = no_opcode;
    opcodeTable[0xDC] = CALL_C_a16;
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
    //opcodeTable[0xED] = no_opcode

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
    opcodeTable[0xFB] = Elephant_I;
    opcodeTable[0xFE] = CP_A_n8;
    opcodeTable[0xFF] = RST_38;


    // Add more opcodes as needed...
}


void CPU::initPreOpcodeTable() {
    std::fill(std::begin(prefixedOpcodeTable), std::end(prefixedOpcodeTable), &UNIMPLEMENTED);
    prefixedOpcodeTable[0x00] = RLC_B;
    prefixedOpcodeTable[0x01] = RLC_C;
    prefixedOpcodeTable[0x02] = RLC_D;
    prefixedOpcodeTable[0x03] = RLC_E;
    prefixedOpcodeTable[0x04] = RLC_H;
    prefixedOpcodeTable[0x05] = RLC_L;
    prefixedOpcodeTable[0x06] = RLC_bracket_HL;
    prefixedOpcodeTable[0x07] = RLC_A;
    prefixedOpcodeTable[0x08] = RRC_B;
    prefixedOpcodeTable[0x09] = RRC_C;
    prefixedOpcodeTable[0x0A] = RRC_D;
    prefixedOpcodeTable[0x0B] = RRC_E;
    prefixedOpcodeTable[0x0C] = RRC_H;
    prefixedOpcodeTable[0x0D] = RRC_L;
    prefixedOpcodeTable[0x0E] = RRC_bracket_HL;
    prefixedOpcodeTable[0x0F] = RRC_A;

    prefixedOpcodeTable[0x10] = RL_B;
    prefixedOpcodeTable[0x11] = RL_C;
    prefixedOpcodeTable[0x12] = RL_D;
    prefixedOpcodeTable[0x13] = RL_E;
    prefixedOpcodeTable[0x14] = RL_H;
    prefixedOpcodeTable[0x15] = RL_L;
    prefixedOpcodeTable[0x16] = RL_bracket_HL;
    prefixedOpcodeTable[0x17] = RL_A;
    prefixedOpcodeTable[0x18] = RR_B;
    prefixedOpcodeTable[0x19] = RR_C;
    prefixedOpcodeTable[0x1A] = RR_D;
    prefixedOpcodeTable[0x1B] = RR_E;
    prefixedOpcodeTable[0x1C]  = RR_H;
    prefixedOpcodeTable[0x1D] = RR_L;
    prefixedOpcodeTable[0x1E] = RR_bracket_HL;
    prefixedOpcodeTable[0x1F] = RR_A;

    prefixedOpcodeTable[0x20] = SLA_B;
    prefixedOpcodeTable[0x21] = SLA_C;
    prefixedOpcodeTable[0x22] = SLA_D;
    prefixedOpcodeTable[0x23] = SLA_E;
    prefixedOpcodeTable[0x24] = SLA_H;
    prefixedOpcodeTable[0x25] = SLA_L;
    prefixedOpcodeTable[0x26] = SLA_bracket_HL;
    prefixedOpcodeTable[0x27] = SLA_A;
    prefixedOpcodeTable[0x28] = SRA_B;
    prefixedOpcodeTable[0x29] = SRA_C;
    prefixedOpcodeTable[0x2A] = SRA_D;
    prefixedOpcodeTable[0x2B] = SRA_E;
    prefixedOpcodeTable[0x2C] = SRA_H;
    prefixedOpcodeTable[0x2D] = SRA_L;
    prefixedOpcodeTable[0x2E] = SRA_bracket_HL;
    prefixedOpcodeTable[0x2F] = SRA_A;


    prefixedOpcodeTable[0x30] = SWAP_B;
    prefixedOpcodeTable[0x31] = SWAP_C;
    prefixedOpcodeTable[0x32] = SWAP_D;
    prefixedOpcodeTable[0x33] = SWAP_E;
    prefixedOpcodeTable[0x34] = SWAP_H;
    prefixedOpcodeTable[0x35] = SWAP_L;
    prefixedOpcodeTable[0x36] = SWAP_bracket_HL;
    prefixedOpcodeTable[0x37] = SWAP_A;
    prefixedOpcodeTable[0x38] = SRL_B;
    prefixedOpcodeTable[0x39] = SRL_C;
    prefixedOpcodeTable[0x3A] = SRL_D;
    prefixedOpcodeTable[0x3B] = SRL_E;
    prefixedOpcodeTable[0x3C] = SRL_H;
    prefixedOpcodeTable[0x3D] = SRL_L;
    prefixedOpcodeTable[0x3E] = SRL_bracket_HL;
    prefixedOpcodeTable[0x3F] = SRL_A;


    prefixedOpcodeTable[0x40] = BIT_0_B;
    prefixedOpcodeTable[0x41] = BIT_0_C;
    prefixedOpcodeTable[0x42] = BIT_0_D;
    prefixedOpcodeTable[0x43] = BIT_0_E;
    prefixedOpcodeTable[0x44] = BIT_0_H;
    prefixedOpcodeTable[0x45] = BIT_0_L;
    prefixedOpcodeTable[0x46] = BIT_0_bracket_HL;
    prefixedOpcodeTable[0x47] = BIT_0_A;
    prefixedOpcodeTable[0x48] = BIT_1_B;
    prefixedOpcodeTable[0x49] = BIT_1_C;
    prefixedOpcodeTable[0x4A] = BIT_1_D;
    prefixedOpcodeTable[0x4B] = BIT_1_E;
    prefixedOpcodeTable[0x4C] = BIT_1_H;
    prefixedOpcodeTable[0x4D] = BIT_1_L;
    prefixedOpcodeTable[0x4E] = BIT_1_bracket_HL;
    prefixedOpcodeTable[0x4F] = BIT_1_A;

    // Prefixed Opcode Table for BIT 2
    prefixedOpcodeTable[0x50] = BIT_2_B;
    prefixedOpcodeTable[0x51] = BIT_2_C;
    prefixedOpcodeTable[0x52] = BIT_2_D;
    prefixedOpcodeTable[0x53] = BIT_2_E;
    prefixedOpcodeTable[0x54] = BIT_2_H;
    prefixedOpcodeTable[0x55] = BIT_2_L;
    prefixedOpcodeTable[0x56] = BIT_2_bracket_HL;
    prefixedOpcodeTable[0x57] = BIT_2_A;
    prefixedOpcodeTable[0x58] = BIT_3_B;
    prefixedOpcodeTable[0x59] = BIT_3_C;
    prefixedOpcodeTable[0x5A] = BIT_3_D;
    prefixedOpcodeTable[0x5B] = BIT_3_E;
    prefixedOpcodeTable[0x5C] = BIT_3_H;
    prefixedOpcodeTable[0x5D] = BIT_3_L;
    prefixedOpcodeTable[0x5E] = BIT_3_bracket_HL;
    prefixedOpcodeTable[0x5F] = BIT_3_A;


    // Prefixed Opcode Table for BIT 4
    prefixedOpcodeTable[0x60] = BIT_4_B;
    prefixedOpcodeTable[0x61] = BIT_4_C;
    prefixedOpcodeTable[0x62] = BIT_4_D;
    prefixedOpcodeTable[0x63] = BIT_4_E;
    prefixedOpcodeTable[0x64] = BIT_4_H;
    prefixedOpcodeTable[0x65] = BIT_4_L;
    prefixedOpcodeTable[0x66] = BIT_4_bracket_HL;
    prefixedOpcodeTable[0x67] = BIT_4_A;
    prefixedOpcodeTable[0x68] = BIT_5_B;
    prefixedOpcodeTable[0x69] = BIT_5_C;
    prefixedOpcodeTable[0x6A] = BIT_5_D;
    prefixedOpcodeTable[0x6B] = BIT_5_E;
    prefixedOpcodeTable[0x6C] = BIT_5_H;
    prefixedOpcodeTable[0x6D] = BIT_5_L;
    prefixedOpcodeTable[0x6E] = BIT_5_bracket_HL;
    prefixedOpcodeTable[0x6F] = BIT_5_A;

    // Prefixed Opcode Table for BIT 6
    prefixedOpcodeTable[0x70] = BIT_6_B;
    prefixedOpcodeTable[0x71] = BIT_6_C;
    prefixedOpcodeTable[0x72] = BIT_6_D;
    prefixedOpcodeTable[0x73] = BIT_6_E;
    prefixedOpcodeTable[0x74] = BIT_6_H;
    prefixedOpcodeTable[0x75] = BIT_6_L;
    prefixedOpcodeTable[0x76] = BIT_6_bracket_HL;
    prefixedOpcodeTable[0x77] = BIT_6_A;
    prefixedOpcodeTable[0x78] = BIT_7_B;
    prefixedOpcodeTable[0x79] = BIT_7_C;
    prefixedOpcodeTable[0x7A] = BIT_7_D;
    prefixedOpcodeTable[0x7B] = BIT_7_E;
    prefixedOpcodeTable[0x7C] = BIT_7_H;
    prefixedOpcodeTable[0x7D] = BIT_7_L;
    prefixedOpcodeTable[0x7E] = BIT_7_bracket_HL;
    prefixedOpcodeTable[0x7F] = BIT_7_A;

    // Prefixed Opcode Table for RES 0
    prefixedOpcodeTable[0x80] = RES_0_B;
    prefixedOpcodeTable[0x81] = RES_0_C;
    prefixedOpcodeTable[0x82] = RES_0_D;
    prefixedOpcodeTable[0x83] = RES_0_E;
    prefixedOpcodeTable[0x84] = RES_0_H;
    prefixedOpcodeTable[0x85] = RES_0_L;
    prefixedOpcodeTable[0x86] = RES_0_bracket_HL;
    prefixedOpcodeTable[0x87] = RES_0_A;
    prefixedOpcodeTable[0x88] = RES_1_B;
    prefixedOpcodeTable[0x89] = RES_1_C;
    prefixedOpcodeTable[0x8A] = RES_1_D;
    prefixedOpcodeTable[0x8B] = RES_1_E;
    prefixedOpcodeTable[0x8C] = RES_1_H;
    prefixedOpcodeTable[0x8D] = RES_1_L;
    prefixedOpcodeTable[0x8E] = RES_1_bracket_HL;
    prefixedOpcodeTable[0x8F] = RES_1_A;



    // Prefixed Opcode Table for RES 2
    prefixedOpcodeTable[0x90] = RES_2_B;
    prefixedOpcodeTable[0x91] = RES_2_C;
    prefixedOpcodeTable[0x92] = RES_2_D;
    prefixedOpcodeTable[0x93] = RES_2_E;
    prefixedOpcodeTable[0x94] = RES_2_H;
    prefixedOpcodeTable[0x95] = RES_2_L;
    prefixedOpcodeTable[0x96] = RES_2_bracket_HL;
    prefixedOpcodeTable[0x97] = RES_2_A;
    prefixedOpcodeTable[0x98] = RES_3_B;
    prefixedOpcodeTable[0x99] = RES_3_C;
    prefixedOpcodeTable[0x9A] = RES_3_D;
    prefixedOpcodeTable[0x9B] = RES_3_E;
    prefixedOpcodeTable[0x9C] = RES_3_H;
    prefixedOpcodeTable[0x9D] = RES_3_L;
    prefixedOpcodeTable[0x9E] = RES_3_bracket_HL;
    prefixedOpcodeTable[0x9F] = RES_3_A;

    // Prefixed Opcode Table for RES 4
    prefixedOpcodeTable[0xA0] = RES_4_B;
    prefixedOpcodeTable[0xA1] = RES_4_C;
    prefixedOpcodeTable[0xA2] = RES_4_D;
    prefixedOpcodeTable[0xA3] = RES_4_E;
    prefixedOpcodeTable[0xA4] = RES_4_H;
    prefixedOpcodeTable[0xA5] = RES_4_L;
    prefixedOpcodeTable[0xA6] = RES_4_bracket_HL;
    prefixedOpcodeTable[0xA7] = RES_4_A;

    // Prefixed Opcode Table for RES 5
    prefixedOpcodeTable[0xA8] = RES_5_B;
    prefixedOpcodeTable[0xA9] = RES_5_C;
    prefixedOpcodeTable[0xAA] = RES_5_D;
    prefixedOpcodeTable[0xAB] = RES_5_E;
    prefixedOpcodeTable[0xAC] = RES_5_H;
    prefixedOpcodeTable[0xAD] = RES_5_L;
    prefixedOpcodeTable[0xAE] = RES_5_bracket_HL;
    prefixedOpcodeTable[0xAF] = RES_5_A;

    // Prefixed Opcode Table for RES 6
    prefixedOpcodeTable[0xB0] = RES_6_B;
    prefixedOpcodeTable[0xB1] = RES_6_C;
    prefixedOpcodeTable[0xB2] = RES_6_D;
    prefixedOpcodeTable[0xB3] = RES_6_E;
    prefixedOpcodeTable[0xB4] = RES_6_H;
    prefixedOpcodeTable[0xB5] = RES_6_L;
    prefixedOpcodeTable[0xB6] = RES_6_bracket_HL;
    prefixedOpcodeTable[0xB7] = RES_6_A;

    // Prefixed Opcode Table for RES 7
    prefixedOpcodeTable[0xB8] = RES_7_B;
    prefixedOpcodeTable[0xB9] = RES_7_C;
    prefixedOpcodeTable[0xBA] = RES_7_D;
    prefixedOpcodeTable[0xBB] = RES_7_E;
    prefixedOpcodeTable[0xBC] = RES_7_H;
    prefixedOpcodeTable[0xBD] = RES_7_L;
    prefixedOpcodeTable[0xBE] = RES_7_bracket_HL;
    prefixedOpcodeTable[0xBF] = RES_7_A;

    // Prefixed Opcode Table for SET 0
    prefixedOpcodeTable[0xC0] = SET_0_B;
    prefixedOpcodeTable[0xC1] = SET_0_C;
    prefixedOpcodeTable[0xC2] = SET_0_D;
    prefixedOpcodeTable[0xC3] = SET_0_E;
    prefixedOpcodeTable[0xC4] = SET_0_H;
    prefixedOpcodeTable[0xC5] = SET_0_L;
    prefixedOpcodeTable[0xC6] = SET_0_bracket_HL;
    prefixedOpcodeTable[0xC7] = SET_0_A;

    // Prefixed Opcode Table for SET 1
    prefixedOpcodeTable[0xC8] = SET_1_B;
    prefixedOpcodeTable[0xC9] = SET_1_C;
    prefixedOpcodeTable[0xCA] = SET_1_D;
    prefixedOpcodeTable[0xCB] = SET_1_E;
    prefixedOpcodeTable[0xCC] = SET_1_H;
    prefixedOpcodeTable[0xCD] = SET_1_L;
    prefixedOpcodeTable[0xCE] = SET_1_bracket_HL;
    prefixedOpcodeTable[0xCF] = SET_1_A;
        
    // Prefixed Opcode Table for SET 2
    prefixedOpcodeTable[0xD0] = SET_2_B;
    prefixedOpcodeTable[0xD1] = SET_2_C;
    prefixedOpcodeTable[0xD2] = SET_2_D;
    prefixedOpcodeTable[0xD3] = SET_2_E;
    prefixedOpcodeTable[0xD4] = SET_2_H;
    prefixedOpcodeTable[0xD5] = SET_2_L;
    prefixedOpcodeTable[0xD6] = SET_2_bracket_HL;
    prefixedOpcodeTable[0xD7] = SET_2_A;
    prefixedOpcodeTable[0xD8] = SET_3_B;
    prefixedOpcodeTable[0xD9] = SET_3_C;
    prefixedOpcodeTable[0xDA] = SET_3_D;
    prefixedOpcodeTable[0xDB] = SET_3_E;
    prefixedOpcodeTable[0xDC] = SET_3_H;
    prefixedOpcodeTable[0xDD] = SET_3_L;
    prefixedOpcodeTable[0xDE] = SET_3_bracket_HL;
    prefixedOpcodeTable[0xDF] = SET_3_A;


    // Prefixed Opcode Table for SET 4
    prefixedOpcodeTable[0xE0] = SET_4_B;
    prefixedOpcodeTable[0xE1] = SET_4_C;
    prefixedOpcodeTable[0xE2] = SET_4_D;
    prefixedOpcodeTable[0xE3] = SET_4_E;
    prefixedOpcodeTable[0xE4] = SET_4_H;
    prefixedOpcodeTable[0xE5] = SET_4_L;
    prefixedOpcodeTable[0xE6] = SET_4_bracket_HL;
    prefixedOpcodeTable[0xE7] = SET_4_A;

    // Prefixed Opcode Table for SET 5
    prefixedOpcodeTable[0xE8] = SET_5_B;
    prefixedOpcodeTable[0xE9] = SET_5_C;
    prefixedOpcodeTable[0xEA] = SET_5_D;
    prefixedOpcodeTable[0xEB] = SET_5_E;
    prefixedOpcodeTable[0xEC] = SET_5_H;
    prefixedOpcodeTable[0xED] = SET_5_L;
    prefixedOpcodeTable[0xEE] = SET_5_bracket_HL;
    prefixedOpcodeTable[0xEF] = SET_5_A;

    // Prefixed Opcode Table for SET 6
    prefixedOpcodeTable[0xF0] = SET_6_B;
    prefixedOpcodeTable[0xF1] = SET_6_C;
    prefixedOpcodeTable[0xF2] = SET_6_D;
    prefixedOpcodeTable[0xF3] = SET_6_E;
    prefixedOpcodeTable[0xF4] = SET_6_H;
    prefixedOpcodeTable[0xF5] = SET_6_L;
    prefixedOpcodeTable[0xF6] = SET_6_bracket_HL;
    prefixedOpcodeTable[0xF7] = SET_6_A;

    // Prefixed Opcode Table for SET 7
    prefixedOpcodeTable[0xF8] = SET_7_B;
    prefixedOpcodeTable[0xF9] = SET_7_C;
    prefixedOpcodeTable[0xFA] = SET_7_D;
    prefixedOpcodeTable[0xFB] = SET_7_E;
    prefixedOpcodeTable[0xFC] = SET_7_H;
    prefixedOpcodeTable[0xFD] = SET_7_L;
    prefixedOpcodeTable[0xFE] = SET_7_bracket_HL;
    prefixedOpcodeTable[0xFF] = SET_7_A;

    
}


void CPU::reset() {
    //memory.write(0xFF50, 0x01);
    
    A = 0x01; 
    F = 0xB0;  // Clear Z flag (F = 0x70 means Z = 0, N = 0, H = 0, C = 0)
    B = 0x00;
    C = 0x13;
    D = 0x00;
    E = 0xD8;
    H = 0x01;
    L = 0x4D;
    PC = 0x0100; 
    SP = 0xFFFE;
    
    memory.ie = 0x00;
    memory.write(0xFF44, 0x91);
    cycleCount = 0;
    interruptsEnabled = false;
    isStopped = false;

    std::cout << "[DEBUG] CPU reset. Registers initialized." << std::endl;
}
void UNIMPLEMENTED(CPU& cpu) {
    std::cerr << "Unimplemented opcode: 0x" 
              << std::hex << static_cast<int>(cpu.read8(cpu.PC)) << std::endl;
    exit(1);
}

void CPU::executeOpcode(uint8_t opcode) {
    std::cout << "Executing opcode: 0x" << std::hex << static_cast<int>(opcode) 
              << " at PC: 0x" << std::hex << static_cast<int>(PC) << std::endl;
    std::cout<<"Th SP is: 0x" << std::hex <<static_cast<int>(SP) << std::endl;
    std::cout<<"the hl value is: "<< std::hex <<static_cast<int>(getHL()) << std::endl;
    std::cout<<"the BC value is: "<< std::hex <<static_cast<int>(getBC()) << std::endl;
    std::cout<<"the DE value is: "<< std::hex <<static_cast<int>(getDE()) << std::endl;
    std::cout<<"the AF value is: "<< std::hex <<static_cast<int>(getAF()) << std::endl;
    std::cout<<"the F value is: 0x"<< std::hex << static_cast<int>(F)  << std::endl;
    if (opcode == 0xCB) {
        uint8_t prefixedByte = memory.read(PC+1); 
        std::cout << "Prefixed Opcode: 0xCB 0x" << std::hex << static_cast<int>(prefixedByte) << std::endl;
        
        if (prefixedOpcodeTable[prefixedByte] == nullptr) {
            std::cerr << "[ERROR] Unimplemented prefixed opcode: 0xCB 0x" 
                      << std::hex << static_cast<int>(prefixedByte) 
                      << " at PC: 0x" << std::hex << static_cast<int>(PC) << std::endl;
        } else {
            prefixedOpcodeTable[prefixedByte](*this);  
        }
    } else {

        if (opcodeTable[opcode] == nullptr) {
            std::cerr << "[ERROR] Unimplemented unprefixed opcode: 0x" 
                      << std::hex << static_cast<int>(opcode) 
                      << " at PC: 0x" << std::hex << static_cast<int>(PC) << std::endl;
        } else {
            opcodeTable[opcode](*this);  
        }

    }
}





uint8_t CPU::fetch() {
    uint8_t opcode = memory.read(PC);  
    return opcode;
}

void CPU::executeNextInstruction() {

    if (interruptsEnabled) {
        handleInterrupts();
    }

   
    uint8_t opcode = memory.read(PC);
    executeOpcode(opcode);  
   
}
void CPU::setPPU(PPU& ppuRef) {
    ppu = &ppuRef;
}


void CPU::updateCycles(uint32_t cycles) {
    cycleCount += cycles;  
    if (ppu) {  // Ensure PPU is set before using it
        ppu->step(cycles);
    }
}



void CPU::enableInterrupts() {
    interruptsEnabled = true;
    std::cout << "[DEBUG] Interrupts Enabled" << std::endl;
}

void CPU::disableInterrupts() {
    interruptsEnabled = false;
    std::cout << "[DEBUG] Interrupts Disabled" << std::endl;
}


void CPU::setZeroFlag(bool value) {
    if (value) {
        F |= (1 << 7);  
    } else  {
    F &= ~(1 << 7);     
    }
}

void CPU::setSubtractFlag(bool value) {
    if (value) {
        F |= (1 << 6);  
    } else {
        F &= ~(1 << 6);
    }      
}

void CPU::setHalfCarryFlag(bool value) {
    if (value) {
        F |= (1 << 5);
    } else {
        F &= ~(1 << 5);      
    }
}

void CPU::setCarryFlag(bool value) {
    if (value){
        F |= (1 << 4);  
    } else {
        F &= ~(1 << 4);       
    }
}


bool CPU::getZeroFlag() {
    return (F & (1 << 7)) != 0;
}

bool CPU::getSubtractFlag() {
    return (F & (1 << 6)) != 0; 
}

bool CPU::getHalfCarryFlag() {
    return (F & (1 << 5)) != 0;  
}

bool CPU::getCarryFlag() {
    return (F & (1 << 4)) != 0;  
}




void CPU::increment8(uint8_t& reg) {
    uint8_t result = reg + 1;
    setZeroFlag(result == 0);  
    setSubtractFlag(false);   
    setHalfCarryFlag((reg & 0x0F) + 1 > 0x0F); 
    reg = result;
}

void CPU::decrement8(uint8_t& reg) {
    uint8_t result = reg - 1;  
    setZeroFlag(result == 0);  
    setSubtractFlag(true);   
    setHalfCarryFlag((reg & 0x0F) == 0x00);
    reg = result;
}

void CPU::increment16(uint8_t& high, uint8_t& low) {
    uint16_t value = ((high << 8 ) + low) + 1;
    high = value >> 8;
    low = value & 0x00FF;
}

void CPU::decrement16(uint8_t& high, uint8_t& low) {
    uint16_t value = ((high << 8 ) + low) - 1;
    high = value >> 8;
    low = value & 0x00FF;

}

uint16_t CPU::pop16() {
    uint8_t low = memory.read(SP);        // Read low byte first
    uint8_t high = memory.read(SP + 1);   // Read high byte next
    SP += 2;  // Increment SP AFTER reading

    uint16_t result = (high << 8) | low;
    std::cout << "[DEBUG] Popped " << std::hex << result << " from stack at SP: " << (SP - 2) << std::endl;

    return result;
}




void CPU::push16(uint16_t value) {
    SP -= 2;  // Decrement SP BEFORE writing to memory
    memory.write(SP, value & 0xFF);         // Low byte at SP
    memory.write(SP + 1, (value >> 8) & 0xFF);  // High byte at SP+1

    std::cout << "[DEBUG] Pushed " << std::hex << value << " to stack at SP: " << SP << std::endl;
}

uint8_t CPU::read8(uint16_t addr) {
    return memory.read(addr);
}

void CPU::writeMemory(uint16_t addr, uint8_t value) {
    memory.write(addr, value);
}

uint16_t CPU::read16(uint16_t addr) {
    uint8_t low = memory.read(addr);
    uint8_t high = memory.read(addr + 1);
    return (high << 8) | low;
}

void CPU::write16(uint16_t addr, uint16_t value) {
    writeMemory(addr, value & 0xFF);
    writeMemory(addr + 1, (value >> 8) & 0xFF);

}


uint16_t CPU::getBC() const { 
    return (B << 8) | C; 
}

uint16_t CPU::getDE() const { 
    return (D << 8) | E; 
}

uint16_t CPU::getHL() const { 
    return (H << 8) | L; 
}

uint16_t CPU::getAF() const { 
    return (A << 8) | F; 
}

void CPU::setBC(uint16_t value) { 
    B = (value >> 8) & 0xFF;
    C = value & 0xFF; 
}

void CPU::setDE(uint16_t value) { 
    D =(value & 0xFF00>> 8); 
    E = value & 0xFF; 
}

void CPU::setHL(uint16_t value) { 
    H = (value >> 8) & 0xFF;  
    L = value & 0xFF; 
}

void CPU::setAF(uint16_t value) { 
    A = (value & 0xFF00>> 8); 
    F = value & 0xFF; 
}

bool CPU::interruptOccurred() {

    uint8_t IF = read8(0xFF0F);  
    uint8_t IE = read8(0xFFFF); 
    return (IF & IE & 0x1F) != 0;  
}

void CPU::handleInterrupts() {
    if (!interruptsEnabled) return; 

    uint8_t IF = memory.read(0xFF0F); 
    uint8_t IE = memory.read(0xFFFF);

    uint8_t activeInterrupts = IF & IE & 0x1F;

    if (activeInterrupts) {
    
        interruptsEnabled = false;
        if (activeInterrupts & 0x01) {
            serviceInterrupt(0x40, 0);  // VBlank Interrupt
        }
        else if (activeInterrupts & 0x02) {
            serviceInterrupt(0x48, 1);  // LCD Stat Interrupt
        }
        else if (activeInterrupts & 0x04) {
            serviceInterrupt(0x50, 2);  // Timer Interrupt
        }
        else if (activeInterrupts & 0x08) {
            serviceInterrupt(0x58, 3);  // Serial Interrupt
        }
        else if (activeInterrupts & 0x10) {
            serviceInterrupt(0x60, 4);  // Joypad Interrupt
        }
    }
}

void CPU::serviceInterrupt(uint16_t address, int bit) {

    uint8_t IF = memory.read(0xFF0F);
    IF &= ~(1 << bit);  
    memory.write(0xFF0F, IF);

    SP -= 2;
    write16(SP, PC);


    PC = address;


    updateCycles(20);
}




void CPU::dumpROMHeader() {
    std::cout << "ROM Header:\n";
    for (int i = 0x100; i < 0x150; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(memory.read(i)) << " ";
        if ((i + 1) % 16 == 0) std::cout << "\n";
    }
}

void CPU::BIT(uint8_t bit, uint8_t reg) {
    bool isBitSet = (reg & (1 << bit)) != 0;


    setZeroFlag(!isBitSet); 
    setSubtractFlag(false);
    setHalfCarryFlag(true);
    


}

void CPU::RES_n_X(CPU& cpu, uint8_t bit, uint8_t& reg) {
    reg &= ~(1 << bit);
    cpu.PC += 2;
    cpu.updateCycles(8);
}

void CPU::RES_n_HL(CPU& cpu, uint8_t bit) {
    uint16_t address = cpu.getHL();    
    uint8_t value = cpu.memory.read(address); 
    value &= ~(1 << bit);                 
    cpu.write16(address, value);       
    cpu.PC += 2;
    cpu.updateCycles(16); 
}

void CPU::SET_n_X(CPU& cpu, uint8_t bit, uint8_t& reg) {
    reg |= (1<<bit);
    cpu.PC+= 2;
    cpu.updateCycles(8);

}

void CPU::SET_n_HL(CPU& cpu, uint8_t bit) {
    uint16_t address = cpu.getHL();        
    uint8_t value = cpu.memory.read(address); 
    cpu.write16(address, value);       
    cpu.PC += 2;
    cpu.updateCycles(16); 

}