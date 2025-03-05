#include "cpu.h"
#include "opcode_handlers.h"
#include <iostream>

CPU::CPU() {
    A = B = C = D = E = H = L = 0;  // Initialize 8-bit registers
    F = 0;  // Flags register

    cycleCount = 0;
    interruptsEnabled = false;

    reset();
    initOpcodeTable();
}

void CPU::initOpcodeTable() {
    std::fill(std::begin(opcodeTable), std::end(opcodeTable), &UNIMPLEMENTED);  

    opcodeTable[0x00] = &NOP;
    opcodeTable[0x01] = &LD_BC_d16;
    opcodeTable[0x02] = &LD_BC_A;
    opcodeTable[0x03] = &INC_BC;
    opcodeTable[0x04] = &INC_B;
    opcodeTable[0x05] = &DEC_B;
    opcodeTable[0x06] = &LD_B_d8;
    opcodeTable[0x07] = &RLCA;
    opcodeTable[0x08] = &LD_a16_SP;
    opcodeTable[0x09] = &ADD_HL_BC;
    opcodeTable[0x0A] = &LD_A_BC;
    opcodeTable[0x0B] = &DEC_BC;
    opcodeTable[0x0C] = &INC_C;
    opcodeTable[0x0D] = &DEC_C;
    opcodeTable[0x0E] = &LD_C_n8;
    opcodeTable[0x0F] = &RRCA;
    opcodeTable[0xC3] = &JP_a16;
    opcodeTable[0xD9] = &RETI;
    opcodeTable[0xE1] = &POP_HL;
    opcodeTable[0xF1] = &POP_AF;
    opcodeTable[0xFF] = &RST_38;
    opcodeTable[0x50] = &LD_D_B;

    // Add more opcodes as needed...
}

void CPU::reset() {
    SP = 0xFFFE;  // Stack Pointer
    PC = 0x0100;  // Program Counter starts at 0x0100
    cycleCount = 0;
}

void UNIMPLEMENTED(CPU& cpu) {
    std::cerr << "Unimplemented opcode: 0x" 
              << std::hex << static_cast<int>(cpu.readMemory(cpu.PC)) << std::endl;
    exit(1);
}

void CPU::executeOpcode(uint8_t opcode) {
    std::cout << "PC: 0x" << std::hex << PC 
              << " | Opcode: 0x" << static_cast<int>(opcode) << std::endl;

    if (opcodeTable[opcode]) {
        opcodeTable[opcode](*this);  // Execute opcode
    } else {
        std::cerr << "Unknown opcode: 0x" << std::hex << static_cast<int>(opcode) << std::endl;
        exit(1);
    }
}
void CPU::executeNextInstruction() {
    uint8_t opcode = readMemory(PC);  // Fetch the opcode from memory
    PC++;  // Increment the program counter to point to the next instruction
    executeOpcode(opcode);  // Execute the opcode
    updateCycles(4);  // Update the cycle count (adjust the cycle count as needed based on the opcode)
}
uint8_t CPU::readMemory(uint16_t addr) {
    return memory.read(addr);
}

void CPU::writeMemory(uint16_t addr, uint8_t value) {
    memory.write(addr, value);
}

uint16_t CPU::read16(uint16_t addr) {
    uint8_t low = readMemory(addr);
    uint8_t high = readMemory(addr + 1);
    return (high << 8) | low;
}

void CPU::write16(uint16_t addr, uint16_t value) {
    if (addr < 0xFF00 || addr > 0xFFFF) {
        std::cerr << "Invalid memory write at address: 0x" << std::hex << addr << std::endl;
        exit(1);
    }
    writeMemory(addr, value & 0xFF);
    writeMemory(addr + 1, (value >> 8) & 0xFF);
}

void CPU::updateCycles(uint32_t cycles) {
    cycleCount += cycles;
    std::cout << "[DEBUG] Total cycle count: " << std::hex << cycleCount << std::endl;
}

void CPU::enableInterrupts() {
    interruptsEnabled = true;
    std::cout << "[DEBUG] Interrupts Enabled" << std::endl;
}

void CPU::disableInterrupts() {
    interruptsEnabled = false;
    std::cout << "[DEBUG] Interrupts Disabled" << std::endl;
}

void CPU::pushToStack(uint16_t value) {
    if (SP < 0xFF80) {
        std::cerr << "Stack underflow! SP: 0x" << std::hex << SP << std::endl;
        exit(1);
    }
    SP -= 2;
    write16(SP, value);
}

uint16_t CPU::popFromStack() {
    if (SP >= 0xFFFF) {
        std::cerr << "Stack overflow! SP: 0x" << std::hex << SP << std::endl;
        exit(1);
    }

    uint16_t value = read16(SP);
    SP += 2;

    std::cout << "[DEBUG] Popped 0x" << std::hex << value 
              << " from SP: 0x" << SP << std::endl;

    return value;
}
