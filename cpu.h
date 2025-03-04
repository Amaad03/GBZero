#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include "opcode_handlers.h"  // Include the opcode handler header

class CPU {
public:
    uint8_t A, B, C, D, E, F, H, L;  // Registers
    uint16_t PC, SP;                  // Program Counter and Stack Pointer
    uint8_t memory[0x10000];          // 64KB memory
    uint32_t cycleCount; 

    // Function pointer array (or map) for opcode handling
    typedef void (*OpcodeHandler)(CPU& cpu);
    OpcodeHandler opcodeTable[256];

    CPU();

    void executeOpcode(uint8_t opcode);
    uint8_t read(uint16_t addr);
    uint16_t read16(uint16_t addr);
    void updateCycles(uint32_t cycleCount);  // Function to update cycle count
private:
    void initOpcodeTable();
};

#endif // CPU_H
