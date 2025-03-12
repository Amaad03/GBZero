#ifndef OPCODE_HANDLERS_H       
#define OPCODE_HANDLERS_H
class CPU;

// Declare opcode handler functions
void no_opcode(CPU& cpu);
void NOP(CPU& cpu);
void LD_BC_d16(CPU& cpu);
void LD_BC_A(CPU& cpu);
void INC_BC(CPU& cpu);
void INC_B(CPU& cpu);
void DEC_B(CPU& cpu);
void LD_B_d8(CPU& cpu);
void RLCA(CPU& cpu);
void LD_a16_SP(CPU& cpu);
void ADD_HL_BC(CPU& cpu);
void LD_A_BC(CPU& cpu);
void DEC_BC(CPU& cpu);
void INC_C(CPU& cpu);
void DEC_C(CPU& cpu);
void LD_C_n8(CPU& cpu);
void RRCA(CPU& cpu);

//------------------------
void STOP_n8(CPU& cpu); //0x10
void LD_DE_n16(CPU& cpu); //0x11
void LD_DE_A(CPU& cpu); //0x12
void INC_DE(CPU& cpu); //0x13
void INC_D(CPU& cpu); //0x14
void DEC_D(CPU& cpu); //0x15
void LD_D_d8(CPU& cpu); //0x16
void RLA(CPU& cpu); //0x17
void JR_e8(CPU& cpu); //0x18
void ADD_HL_DE(CPU& cpu); //0x19
void LD_A_DE(CPU& cpu); //0x1A
void DEC_DE(CPU& cpu); //0x1B
void INC_E(CPU& cpu); //0x1C
void DEC_E(CPU& cpu); //0x1D
void LD_E_n8(CPU& cpu); // 0x1E
void RRA(CPU& cpu); //0x1F
//---------------------------
void JR_NZ_e8(CPU& cpu);
void LD_HL_n16(CPU& cpu);
void LD_HLplus_A(CPU& cpu);
void INC_HL(CPU& cpu);
void INC_H(CPU& cpu);
void DEC_H(CPU& cpu);
void LD_H_d8(CPU& cpu);
void DAA(CPU& cpu);
void JRZ_e8(CPU& cpu);
void ADD_HL_HL(CPU& cpu);
void LD_A_HLplus(CPU& cpu);
void DEC_HL(CPU& cpu);
void INC_L(CPU& cpu);
void DEC_L(CPU& cpu);
void LD_L_n8(CPU& cpu);
void CPL(CPU& cpu);
//-------------------------
void JR_NZ_e8(CPU& cpu);
void LD_SP_n16(CPU& cpu);
void LD_HL_minus_A(CPU& cpu);
void INC_SP(CPU& cpu);
void INC_HL(CPU& cpu);
void INC_HL_mem(CPU& cpu);
void DEC_HL_mem(CPU& cpu);
void DEC_HL(CPU& cpu);
void LD_HL_d8(CPU& cpu);
void SCF(CPU& cpu);
void JR_C_e8(CPU& cpu);
void ADD_HL_SP(CPU& cpu);
void LD_A_HL_minus(CPU& cpu);
void DEC_SP(CPU& cpu);
void INC_A(CPU& cpu);
void DEC_A(CPU& cpu);
void LD_A_n8(CPU& cpu);
void CCF(CPU& cpu);

void LD_B_B(CPU& cpu);
void LD_B_C(CPU& cpu);
void LD_B_D(CPU& cpu);
void LD_B_E(CPU& cpu);
void LD_B_H(CPU& cpu);
void LD_B_L(CPU& cpu);
void LD_B_HL(CPU& cpu);
void LD_B_A(CPU& cpu);
void LD_C_B(CPU& cpu);
void LD_C_C(CPU& cpu);
void LD_C_D(CPU& cpu);
void LD_C_E(CPU& cpu);
void LD_C_H(CPU& cpu);
void LD_C_L(CPU& cpu);
void LD_C_HL(CPU& cpu);
void LD_C_A(CPU& cpu);
//----------------------
void LD_D_B(CPU& cpu);
void LD_D_C(CPU& cpu);
void LD_D_D(CPU& cpu);
void LD_D_E(CPU& cpu);
void LD_D_H(CPU& cpu);
void LD_D_L(CPU& cpu);
void LD_D_HL(CPU& cpu);
void LD_D_A(CPU& cpu);
void LD_E_B(CPU& cpu);
void LD_E_C(CPU& cpu);
void LD_E_D(CPU& cpu);
void LD_E_E(CPU& cpu);
void LD_E_H(CPU& cpu);
void LD_E_L(CPU& cpu);
void LD_E_HL(CPU& cpu);
void LD_E_A(CPU& cpu);

void LD_H_B(CPU& cpu);
void LD_H_C(CPU& cpu);
void LD_H_D(CPU& cpu);
void LD_H_E(CPU& cpu);
void LD_H_H(CPU& cpu);
void LD_H_L(CPU& cpu);
void LD_H_HL(CPU& cpu);
void LD_H_A(CPU& cpu);
void LD_L_B(CPU& cpu);
void LD_L_C(CPU& cpu);
void LD_L_D(CPU& cpu);
void LD_L_E(CPU& cpu);
void LD_L_H(CPU& cpu);
void LD_L_L(CPU& cpu);
void LD_L_HL(CPU& cpu);
void LD_L_A(CPU& cpu);

//7x row
void LD_HL_B(CPU& cpu);
void LD_HL_C(CPU& cpu);
void LD_HL_D(CPU& cpu);
void LD_HL_E(CPU& cpu);
void LD_HL_H(CPU& cpu);
void LD_HL_L(CPU& cpu);
void HALT(CPU& cpu);
void LD_HL_A(CPU& cpu);
void LD_A_B(CPU& cpu);
void LD_A_C(CPU& cpu);
void LD_A_D(CPU& cpu);
void LD_A_E(CPU& cpu);
void LD_A_H(CPU& cpu);
void LD_A_L(CPU& cpu);
void LD_A_HL(CPU& cpu);
void LD_A_A(CPU& cpu);

//8x row
void ADD_A_B(CPU& cpu);
void ADD_A_C(CPU& cpu);
void ADD_A_D(CPU& cpu);
void ADD_A_E(CPU& cpu);
void ADD_A_H(CPU& cpu);
void ADD_A_L(CPU& cpu);
void ADD_A_HL(CPU& cpu);
void ADD_A_A(CPU& cpu);
void ADC_A_B(CPU& cpu);
void ADC_A_C(CPU& cpu);
void ADC_A_D(CPU& cpu);
void ADC_A_E(CPU& cpu);
void ADC_A_H(CPU& cpu);
void ADC_A_L(CPU& cpu);
void ADC_A_HL(CPU& cpu);
void ADC_A_A(CPU& cpu);
//9x row

void SUB_A_B(CPU& cpu);
void SUB_A_C(CPU& cpu);
void SUB_A_D(CPU& cpu);
void SUB_A_E(CPU& cpu);
void SUB_A_H(CPU& cpu);
void SUB_A_L(CPU& cpu);
void SUB_A_memHL(CPU& cpu);
void SUB_A_A(CPU& cpu);

// Declarations for SBC instructions
void SBC_A_B(CPU& cpu);
void SBC_A_C(CPU& cpu);
void SBC_A_D(CPU& cpu);
void SBC_A_E(CPU& cpu);
void SBC_A_H(CPU& cpu);
void SBC_A_L(CPU& cpu);
void SBC_A_memHL(CPU& cpu);
void SBC_A_A(CPU& cpu);




void AND_A_B(CPU& cpu);
void AND_A_C(CPU& cpu);
void AND_A_D(CPU& cpu);
void AND_A_E(CPU& cpu);
void AND_A_H(CPU& cpu);
void AND_A_L(CPU& cpu);
void AND_A_memHL(CPU& cpu);
void AND_A_A(CPU& cpu);
void XOR_A_B(CPU& cpu);
void XOR_A_C(CPU& cpu);
void XOR_A_D(CPU& cpu);
void XOR_A_E(CPU& cpu);
void XOR_A_H(CPU& cpu);
void XOR_A_L(CPU& cpu);
void XOR_A_memHL(CPU& cpu);
void XOR_A_A(CPU& cpu);


//bx row
void OR_A_B(CPU& cpu);
void OR_A_C(CPU& cpu);
void OR_A_D(CPU& cpu);
void OR_A_E(CPU& cpu);
void OR_A_H(CPU& cpu);
void OR_A_L(CPU& cpu);
void OR_A_memHL(CPU& cpu);
void OR_A_A(CPU& cpu);
void CP_A_B(CPU& cpu);
void CP_A_C(CPU& cpu);
void CP_A_D(CPU& cpu);
void CP_A_E(CPU& cpu);
void CP_A_H(CPU& cpu);
void CP_A_L(CPU& cpu);
void CP_A_memHL(CPU& cpu);
void CP_A_A(CPU& cpu);


void RET_NZ(CPU& cpu);
void POP_BC(CPU& cpu);
void JP_NZ_a16(CPU& cpu);
void JP_a16(CPU& cpu);
void CALL_NZ_a16(CPU& cpu);
void PUSH_BC(CPU& cpu);
void ADD_A_n8(CPU& cpu);
void RST_00(CPU& cpu);
void RET_Z(CPU& cpu);
void RET(CPU& cpu);
void JP_Z_a16(CPU& cpu);
void PREFIX(CPU& cpu);
void CALL_Z_a16(CPU& cpu);
void CALL_a16(CPU& cpu);
void ADC_A_n8(CPU& cpu);
void RST_08(CPU& cpu);



// RET NC
void RET_NC(CPU& cpu);

// POP DE
void POP_DE(CPU& cpu);

// JP NC, a16
void JP_NC_a16(CPU& cpu);

// CALL NC, a16
void CALL_NC_a16(CPU& cpu);

// PUSH DE
void PUSH_DE(CPU& cpu);

// SUB A, n8
void SUB_A_n8(CPU& cpu);

// RST $10
void RST_10(CPU& cpu);

// RET C
void RET_C(CPU& cpu);

// RETI
void RETI(CPU& cpu);

// JP C, a16
void JP_C_a16(CPU& cpu);

// CALL C, a16
void CALL_C_a16(CPU& cpu);

// SBC A, n8
void SBC_A_n8(CPU& cpu);

// RST $18
void RST_18(CPU& cpu);



void JP_a16(CPU& cpu);

void ADD_HL_BC(CPU& cpu);
void LD_A_BC(CPU& cpu);
void LD_a16_SP(CPU& cpu);
void LD_D_B(CPU& cpu);

void ADC_A_n8(CPU& cpu);






void LD_SP(CPU& cpu);
void BIT_7_H (CPU& cpu);

// LDH [a8], A
void LDH_a8_A(CPU& cpu);

// POP HL
void POP_HL(CPU& cpu);

// LDH [C], A
void LDH_C_A(CPU& cpu);

// PUSH HL
void PUSH_HL(CPU& cpu);

// AND A, n8
void AND_A_n8(CPU& cpu);

// RST $20
void RST_20(CPU& cpu);

// ADD SP, e8
void ADD_SP_e8(CPU& cpu);

// JP HL
void JP_HL(CPU& cpu);

// LD [a16], A
void LD_a16_A(CPU& cpu);

// XOR A, n8
void XOR_A_n8(CPU& cpu);

// RST $28
void RST_28(CPU& cpu);

// LDH A, [a8]
void LDH_A_a8(CPU& cpu);

// POP AF
void POP_AF(CPU& cpu);

// LDH A, [C]
void LDH_A_C(CPU& cpu);

// DI
void DI(CPU& cpu);

// PUSH AF
void PUSH_AF(CPU& cpu);

// OR A, n8
void OR_A_n8(CPU& cpu);

// RST $30
void RST_30(CPU& cpu);

// LD HL, SP + e8
void LD_HL_SP_e8(CPU& cpu);

// LD SP, HL
void LD_SP_HL(CPU& cpu);

// LD A, [a16]
void LD_A_a16(CPU& cpu);

// EI
void Elephant_I(CPU& cpu);

// CP A, n8
void CP_A_n8(CPU& cpu);

// RST $38
void RST_38(CPU& cpu);






void RLC_B(CPU& cpu);
void RLC_C(CPU& cpu);
void RLC_D(CPU& cpu);
void RLC_E(CPU& cpu);
void RLC_H(CPU& cpu);
void RLC_L(CPU& cpu);
void RLC_bracket_HL(CPU& cpu);
void RLC_A(CPU& cpu);
void RRC_B(CPU& cpu);
void RRC_C(CPU& cpu);
void RRC_D(CPU& cpu);
void RRC_E(CPU& cpu);
void RRC_H(CPU& cpu);
void RRC_L(CPU& cpu);
void RRC_bracket_HL(CPU& cpu);
void RRC_A(CPU& cpu);



void RL_B(CPU& cpu);
void RL_C(CPU& cpu);
void RL_D(CPU& cpu);
void RL_E(CPU& cpu);
void RL_H(CPU& cpu);
void RL_L(CPU& cpu);
void RL_bracket_HL(CPU& cpu);
void RL_A(CPU& cpu);
void RR_B(CPU& cpu);
void RR_C(CPU& cpu);
void RR_D(CPU& cpu);
void RR_E(CPU& cpu);
void RR_H(CPU& cpu);
void RR_L(CPU& cpu);
void RR_bracket_HL(CPU& cpu);
void RR_A(CPU& cpu);

void SLA_B(CPU& cpu);
void SLA_C(CPU& cpu);
void SLA_D(CPU& cpu);
void SLA_E(CPU& cpu);
void SLA_H(CPU& cpu);
void SLA_L(CPU& cpu);
void SLA_bracket_HL(CPU& cpu);
void SLA_A(CPU& cpu);
void SRA_B(CPU& cpu);
void SRA_C(CPU& cpu);
void SRA_D(CPU& cpu);
void SRA_E(CPU& cpu);
void SRA_H(CPU& cpu);
void SRA_L(CPU& cpu);
void SRA_bracket_HL(CPU& cpu);
void SRA_A(CPU& cpu);

// SWAP Instructions
void SWAP_B(CPU& cpu);
void SWAP_C(CPU& cpu);
void SWAP_D(CPU& cpu);
void SWAP_E(CPU& cpu);
void SWAP_H(CPU& cpu);
void SWAP_L(CPU& cpu);
void SWAP_bracket_HL(CPU& cpu);
void SWAP_A(CPU& cpu);

// SRL Instructions
void SRL_B(CPU& cpu);
void SRL_C(CPU& cpu);
void SRL_D(CPU& cpu);
void SRL_E(CPU& cpu);
void SRL_H(CPU& cpu);
void SRL_L(CPU& cpu);
void SRL_bracket_HL(CPU& cpu);
void SRL_A(CPU& cpu);

// BIT Instructions
void BIT_0_B(CPU& cpu);
void BIT_0_C(CPU& cpu);
void BIT_0_D(CPU& cpu);
void BIT_0_E(CPU& cpu);
void BIT_0_H(CPU& cpu);
void BIT_0_L(CPU& cpu);
void BIT_0_bracket_HL(CPU& cpu);
void BIT_0_A(CPU& cpu);
void BIT_1_B(CPU& cpu);
void BIT_1_C(CPU& cpu);
void BIT_1_D(CPU& cpu);
void BIT_1_E(CPU& cpu);
void BIT_1_H(CPU& cpu);
void BIT_1_L(CPU& cpu);
void BIT_1_bracket_HL(CPU& cpu);
void BIT_1_A(CPU& cpu);

// BIT Instructions for BIT 2
void BIT_2_B(CPU& cpu);
void BIT_2_C(CPU& cpu);
void BIT_2_D(CPU& cpu);
void BIT_2_E(CPU& cpu);
void BIT_2_H(CPU& cpu);
void BIT_2_L(CPU& cpu);
void BIT_2_bracket_HL(CPU& cpu);
void BIT_2_A(CPU& cpu);
void BIT_3_B(CPU& cpu);
void BIT_3_C(CPU& cpu);
void BIT_3_D(CPU& cpu);
void BIT_3_E(CPU& cpu);
void BIT_3_H(CPU& cpu);
void BIT_3_L(CPU& cpu);
void BIT_3_bracket_HL(CPU& cpu);
void BIT_3_A(CPU& cpu);



// BIT Instructions for BIT 4
void BIT_4_B(CPU& cpu);
void BIT_4_C(CPU& cpu);
void BIT_4_D(CPU& cpu);
void BIT_4_E(CPU& cpu);
void BIT_4_H(CPU& cpu);
void BIT_4_L(CPU& cpu);
void BIT_4_bracket_HL(CPU& cpu);
void BIT_4_A(CPU& cpu);
void BIT_5_B(CPU& cpu);
void BIT_5_C(CPU& cpu);
void BIT_5_D(CPU& cpu);
void BIT_5_E(CPU& cpu);
void BIT_5_H(CPU& cpu);
void BIT_5_L(CPU& cpu);
void BIT_5_bracket_HL(CPU& cpu);
void BIT_5_A(CPU& cpu);

void BIT_6_B(CPU& cpu);
void BIT_6_C(CPU& cpu);
void BIT_6_D(CPU& cpu);
void BIT_6_E(CPU& cpu);
void BIT_6_H(CPU& cpu);
void BIT_6_L(CPU& cpu);
void BIT_6_bracket_HL(CPU& cpu);
void BIT_6_A(CPU& cpu);
void BIT_7_B(CPU& cpu);
void BIT_7_C(CPU& cpu);
void BIT_7_D(CPU& cpu);
void BIT_7_E(CPU& cpu);
void BIT_7_H(CPU& cpu);
void BIT_7_L(CPU& cpu);
void BIT_7_bracket_HL(CPU& cpu);
void BIT_7_A(CPU& cpu);

// RES Instructions for RES 0
void RES_0_B(CPU& cpu);
void RES_0_C(CPU& cpu);
void RES_0_D(CPU& cpu);
void RES_0_E(CPU& cpu);
void RES_0_H(CPU& cpu);
void RES_0_L(CPU& cpu);
void RES_0_bracket_HL(CPU& cpu);
void RES_0_A(CPU& cpu);
void RES_1_B(CPU& cpu);
void RES_1_C(CPU& cpu);
void RES_1_D(CPU& cpu);
void RES_1_E(CPU& cpu);
void RES_1_H(CPU& cpu);
void RES_1_L(CPU& cpu);
void RES_1_bracket_HL(CPU& cpu);
void RES_1_A(CPU& cpu);


// RES Instructions for RES 2
void RES_2_B(CPU& cpu);
void RES_2_C(CPU& cpu);
void RES_2_D(CPU& cpu);
void RES_2_E(CPU& cpu);
void RES_2_H(CPU& cpu);
void RES_2_L(CPU& cpu);
void RES_2_bracket_HL(CPU& cpu);
void RES_2_A(CPU& cpu);

// RES Instructions for RES 3
void RES_3_B(CPU& cpu);
void RES_3_C(CPU& cpu);
void RES_3_D(CPU& cpu);
void RES_3_E(CPU& cpu);
void RES_3_H(CPU& cpu);
void RES_3_L(CPU& cpu);
void RES_3_bracket_HL(CPU& cpu);
void RES_3_A(CPU& cpu);



// RES Instructions for RES 4
void RES_4_B(CPU& cpu);
void RES_4_C(CPU& cpu);
void RES_4_D(CPU& cpu);
void RES_4_E(CPU& cpu);
void RES_4_H(CPU& cpu);
void RES_4_L(CPU& cpu);
void RES_4_bracket_HL(CPU& cpu);
void RES_4_A(CPU& cpu);

// RES Instructions for RES 5
void RES_5_B(CPU& cpu);
void RES_5_C(CPU& cpu);
void RES_5_D(CPU& cpu);
void RES_5_E(CPU& cpu);
void RES_5_H(CPU& cpu);
void RES_5_L(CPU& cpu);
void RES_5_bracket_HL(CPU& cpu);
void RES_5_A(CPU& cpu);

// RES Instructions for RES 6
void RES_6_B(CPU& cpu);
void RES_6_C(CPU& cpu);
void RES_6_D(CPU& cpu);
void RES_6_E(CPU& cpu);
void RES_6_H(CPU& cpu);
void RES_6_L(CPU& cpu);
void RES_6_bracket_HL(CPU& cpu);
void RES_6_A(CPU& cpu);

// RES Instructions for RES 7
void RES_7_B(CPU& cpu);
void RES_7_C(CPU& cpu);
void RES_7_D(CPU& cpu);
void RES_7_E(CPU& cpu);
void RES_7_H(CPU& cpu);
void RES_7_L(CPU& cpu);
void RES_7_bracket_HL(CPU& cpu);
void RES_7_A(CPU& cpu);


// SET Instructions for SET 0
void SET_0_B(CPU& cpu);
void SET_0_C(CPU& cpu);
void SET_0_D(CPU& cpu);
void SET_0_E(CPU& cpu);
void SET_0_H(CPU& cpu);
void SET_0_L(CPU& cpu);
void SET_0_bracket_HL(CPU& cpu);
void SET_0_A(CPU& cpu);

// SET Instructions for SET 1
void SET_1_B(CPU& cpu);
void SET_1_C(CPU& cpu);
void SET_1_D(CPU& cpu);
void SET_1_E(CPU& cpu);
void SET_1_H(CPU& cpu);
void SET_1_L(CPU& cpu);
void SET_1_bracket_HL(CPU& cpu);
void SET_1_A(CPU& cpu);

// SET Instructions for SET 2
void SET_2_B(CPU& cpu);
void SET_2_C(CPU& cpu);
void SET_2_D(CPU& cpu);
void SET_2_E(CPU& cpu);
void SET_2_H(CPU& cpu);
void SET_2_L(CPU& cpu);
void SET_2_bracket_HL(CPU& cpu);
void SET_2_A(CPU& cpu);

// SET Instructions for SET 3
void SET_3_B(CPU& cpu);
void SET_3_C(CPU& cpu);
void SET_3_D(CPU& cpu);
void SET_3_E(CPU& cpu);
void SET_3_H(CPU& cpu);
void SET_3_L(CPU& cpu);
void SET_3_bracket_HL(CPU& cpu);
void SET_3_A(CPU& cpu);


// SET Instructions for SET 4
void SET_4_B(CPU& cpu);
void SET_4_C(CPU& cpu);
void SET_4_D(CPU& cpu);
void SET_4_E(CPU& cpu);
void SET_4_H(CPU& cpu);
void SET_4_L(CPU& cpu);
void SET_4_bracket_HL(CPU& cpu);
void SET_4_A(CPU& cpu);

// SET Instructions for SET 5
void SET_5_B(CPU& cpu);
void SET_5_C(CPU& cpu);
void SET_5_D(CPU& cpu);
void SET_5_E(CPU& cpu);
void SET_5_H(CPU& cpu);
void SET_5_L(CPU& cpu);
void SET_5_bracket_HL(CPU& cpu);
void SET_5_A(CPU& cpu);
// SET Instructions for SET 6
void SET_6_B(CPU& cpu);
void SET_6_C(CPU& cpu);
void SET_6_D(CPU& cpu);
void SET_6_E(CPU& cpu);
void SET_6_H(CPU& cpu);
void SET_6_L(CPU& cpu);
void SET_6_bracket_HL(CPU& cpu);
void SET_6_A(CPU& cpu);

// SET Instructions for SET 7
void SET_7_B(CPU& cpu);
void SET_7_C(CPU& cpu);
void SET_7_D(CPU& cpu);
void SET_7_E(CPU& cpu);
void SET_7_H(CPU& cpu);
void SET_7_L(CPU& cpu);
void SET_7_bracket_HL(CPU& cpu);
void SET_7_A(CPU& cpu);

#endif // OPCODE_HANDLERS_H

