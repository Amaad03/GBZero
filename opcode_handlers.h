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


void RST_38(CPU& cpu);
void JP_a16(CPU& cpu);
void POP_HL(CPU& cpu);
void POP_AF(CPU& cpu);
void RETI(CPU& cpu);
void ADD_HL_BC(CPU& cpu);
void LD_A_BC(CPU& cpu);
void LD_a16_SP(CPU& cpu);
void LD_D_B(CPU& cpu);

void ADC_A_n8(CPU& cpu);
void no_opcode(CPU& cpu); 
#endif // OPCODE_HANDLERS_H

