#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include "opcode_handlers.h"  // Include the opcode handler header
#include <iostream>
#include "memory.h"

typedef void (*OpcodeHandler)(CPU& cpu);

class CPU {
public:
    // 8-bit registers
    uint8_t A, F;  // Accumulator and Flags
    uint8_t B, C;  // BC register pair
    uint8_t D, E;  // DE register pair
    uint8_t H, L;  // HL register pair

    // 16-bit registers
    uint16_t AF, BC, DE, HL;
    uint16_t PC, SP;   // Program counter and stack pointer
    Memory memory;     // 64KB of memory
    uint32_t cycleCount; // Cycle count for instruction execution
    bool interruptsEnabled;

    OpcodeHandler opcodeTable[256]; // Opcode handler table (array of function pointers)

    CPU();  // Constructor

    void executeOpcode(uint8_t opcode);
    void executeNextInstruction();

    uint8_t readMemory(uint16_t addr);
    void writeMemory(uint16_t addr, uint8_t value);
    uint16_t read16(uint16_t addr);
    void write16(uint16_t addr, uint16_t value);

    void pushToStack(uint16_t value);
    uint16_t popFromStack();

    void updateCycles(uint32_t cycles);

    bool getZeroFlag(bool value);
    bool getSubtractFlag(bool value);
    bool getHalfCarryFlag(bool value);
    bool getCarryFlag();

    void updateAF();  // Update AF based on A and F
    void updateBC();  // Update BC based on B and C
    void updateDE();  // Update DE based on D and E
    void updateHL();  // Update HL based on H and L

    void updateA_F();  // Update A and F based on AF
    void updateB_C();  // Update B and C based on BC
    void updateD_E();  // Update D and E based on DE
    void updateH_L();  // Update H and L based on HL

    void initOpcodeTable();
    void enableInterrupts();
    void disableInterrupts();

    void reset();
    
    // Flag utilities
    void setZeroFlag(bool value);
    void setSubtractFlag(bool value);
    void setHalfCarryFlag(bool value);
    void setCarryFlag(bool value);



    //arithmetic
    void increment8(uint8_t& reg);
    void increment16(uint16_t& reg, uint8_t& high, uint8_t& low);
    void decrement8(uint8_t& reg);
    void decrement16(uint16_t& reg, uint8_t& high, uint8_t& low);
};

void UNIMPLEMENTED(CPU& cpu);

#endif // CPU_H
