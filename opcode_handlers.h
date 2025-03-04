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
void LD_C_d8(CPU& cpu);
void RRCA(CPU& cpu);
void RST_38(CPU& cpu);
void JP_a16(CPU& cpu);
void POP_HL(CPU& cpu);
void POP_AF(CPU& cpu);
void RETI(CPU& cpu);




#endif // OPCODE_HANDLERS_H

