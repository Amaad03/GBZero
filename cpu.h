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

    uint16_t PC, SP;   // Program counter and stack pointer
    Memory& memory;    // Reference to memory
    uint32_t cycleCount; // Cycle count for instruction execution
    bool interruptsEnabled;
    bool isStopped;

    // Constructor
    CPU(Memory& mem);

    // Reset the CPU
    void reset();
    OpcodeHandler opcodeTable[256]  = {0}; // Opcode handler table (array of function pointers)
    OpcodeHandler prefixedOpcodeTable[256] = {0};
    void executeOpcode(uint8_t opcode);
    void executeNextInstruction();

    uint8_t read8(uint16_t addr);
    void writeMemory(uint16_t addr, uint8_t value);
    uint16_t read16(uint16_t addr);
    void write16(uint16_t addr, uint16_t value);

    void updateCycles(uint32_t cycles);

    bool getZeroFlag();
    bool getSubtractFlag();
    bool getHalfCarryFlag();
    bool getCarryFlag();

    void initOpcodeTable();
    void initPreOpcodeTable();
    void enableInterrupts();
    void disableInterrupts();
    void disableBootROM();
    
    void run();
    bool interruptOccurred();
    void handleInterrupts();
    
    // Flag utilities
    void setZeroFlag(bool value);
    void setSubtractFlag(bool value);
    void setHalfCarryFlag(bool value);
    void setCarryFlag(bool value);
    void BIT(uint8_t bit, uint8_t reg);


    uint16_t pop16();
    void push16(uint16_t value);
    //arithmetic
    void increment8(uint8_t& reg);
    void increment16(uint8_t& high, uint8_t& low);
    void decrement8(uint8_t& reg);
    void decrement16(uint8_t& high, uint8_t& low);
    uint16_t getBC() const;
    uint16_t getDE() const;
    uint16_t getHL() const;
    uint16_t getAF() const;

    // Setter functions for 16-bit registers
    void setBC(uint16_t value);
    void setDE(uint16_t value);
    void setHL(uint16_t value);
    void setAF(uint16_t value);
    uint8_t fetch();

    void dumpROMHeader();
};

void UNIMPLEMENTED(CPU& cpu);
#endif // CPU_H
