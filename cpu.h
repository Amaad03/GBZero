#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include "opcode_handlers.h"  // Include the opcode handler header

// Typedef for the function pointer type
class CPU;


typedef void (*OpcodeHandler)(CPU& cpu);

class CPU {
public:
    uint8_t A, B, C, D, E, F, H, L;  // 8-bit registers
    uint16_t PC, SP;                  // 16-bit registers for program counter and stack pointer
    uint8_t memory[0x10000];          // 64KB of memory
    uint32_t cycleCount;              // Cycle count for instruction execution

    // Opcode handler table (array of function pointers)
    OpcodeHandler opcodeTable[256];

    CPU();  // Constructor

    // Method declarations

    void executeOpcode(uint8_t opcode);
    uint8_t read(uint16_t addr);
    uint16_t read16(uint16_t addr);
    void updateCycles(uint32_t cycles);  // Function to update cycle count
    void initOpcodeTable();  // Initialize the opcode table with handlers
};

void UNIMPLEMENTED(CPU& cpu);



#endif // CPU_H
