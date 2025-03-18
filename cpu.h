#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "opcode_handlers.h" 
#include <iostream>
#include "memory.h"
#include "ppu.h"
typedef void (*OpcodeHandler)(CPU& cpu);

class PPU;

class CPU {
public:

    uint8_t A, F;  // Accumulator and Flags
    uint8_t B, C;  // BC register pair
    uint8_t D, E;  // DE register pair
    uint8_t H, L;  // HL register pair

    uint16_t PC, SP;   // Program counter and stack pointer
    Memory& memory;    // Reference to memory
    PPU* ppu = nullptr; 


    uint32_t cycleCount; // Cycle count for instruction execution
    bool interruptsEnabled;
    bool isStopped;
    bool interruptsEnableNextInstructions;
    uint8_t interruptFlags;
    uint8_t interruptEnable;


    CPU(Memory& mem);

    void setPPU(PPU& ppuRef);  // Function to set the PPU reference


    void reset();
    OpcodeHandler opcodeTable[256]  = {0}; 
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

    void serviceInterrupt(uint16_t address, int bit);
    bool interruptOccurred();
    void handleInterrupts();

    void setZeroFlag(bool value);
    void setSubtractFlag(bool value);
    void setHalfCarryFlag(bool value);
    void setCarryFlag(bool value);
    void BIT(uint8_t bit, uint8_t reg);
    void RES_n_X(CPU& cpu, uint8_t bit, uint8_t& reg);
    void RES_n_HL(CPU& cpu, uint8_t bit);
    void SET_n_X(CPU& cpu, uint8_t bit, uint8_t& reg);
    void SET_n_HL(CPU& cpu, uint8_t bit);

    uint16_t pop16();
    void push16(uint16_t value);
    void increment8(uint8_t& reg);
    void increment16(uint8_t& high, uint8_t& low);
    void decrement8(uint8_t& reg);
    void decrement16(uint8_t& high, uint8_t& low);
    uint16_t getBC() const;
    uint16_t getDE() const;
    uint16_t getHL() const;
    uint16_t getAF() const;


    void setBC(uint16_t value);
    void setDE(uint16_t value);
    void setHL(uint16_t value);
    void setAF(uint16_t value);
    uint8_t fetch();

    void dumpROMHeader();
};

void UNIMPLEMENTED(CPU& cpu);
#endif // CPU_H
