#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include "opcode_handlers.h"  // Include the opcode handler header

#include "memory.h"

typedef void (*OpcodeHandler)(CPU& cpu);

class CPU {
public:
    uint8_t A, B, C, D, E, F, H, L;  // 8-bit registers
    uint16_t PC = 0x100;
    uint16_t SP = 0xFFFE;                // 16-bit registers for program counter and stack pointer
    Memory memory;      // 64KB of memory
    uint32_t cycleCount;              // Cycle count for instruction execution
    bool interruptsEnabled;  
    
    // Opcode handler table (array of function pointers)
    OpcodeHandler opcodeTable[256];

    CPU();  // Constructor

    // Method declarations

    void executeOpcode(uint8_t opcode);
    //uint8_t read(uint16_t addr);
    uint16_t read16(uint16_t addr);
    //void write(uint16_t addr, uint8_t value);
    void write16(uint16_t addr, uint16_t value);
    void updateCycles(uint32_t cycles);
    void pushToStack(uint16_t value);
    uint16_t popFromStack();
    void initOpcodeTable();
    void enableInterrupts();
    void disableInterrupts();
    void executeNextInstruction();
    uint8_t readMemory(uint16_t addr);
    void writeMemory(uint16_t addr, uint8_t value);
    void reset();
};

// Global function for unimplemented opcodes
void UNIMPLEMENTED(CPU& cpu);


#endif // CPU_H
