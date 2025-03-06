#ifndef OPCODE_HANDLERS_H       
#define OPCODE_HANDLERS_H
class CPU;

// Declare opcode handler functions
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
void LD_D_n8(CPU& cpu); //0x16
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
void LD_H_n8(CPU& cpu);
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
void LD_HL_n8(CPU& cpu);
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
void no_opcode(CPU& cpu); 








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
void EI(CPU& cpu);

// CP A, n8
void CP_A_n8(CPU& cpu);

// RST $38
void RST_38(CPU& cpu);
void SET_7_A(CPU& cpu);
#endif // OPCODE_HANDLERS_H

