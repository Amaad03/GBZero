#include "cpu.h"
#include <iostream>
#include <fstream>
#include <vector>

// Function to load ROM into CPU memory
bool loadROM(CPU& cpu, const std::string& romPath) {
    std::ifstream romFile(romPath, std::ios::binary);
    if (!romFile) {
        std::cerr << "Failed to open ROM file: " << std::endl;
        return 1;
    } else {
        std::cout << "Succeeded in opening ROM file: "<< std::endl;
    }

    // Read the ROM into a vector
    std::vector<uint8_t> romData((std::istreambuf_iterator<char>(romFile)),
                                 std::istreambuf_iterator<char>());
    romFile.close(); // Close file after reading

    std::cout << "ROM Size: " << romData.size() << " bytes" << std::endl;

    // Load the ROM data into CPU memory starting at 0x100
    std::cout << "Loading ROM..." << std::endl;

    for (size_t i = 0; i < romData.size(); ++i) {
        cpu.memory[0x100 + i] = romData[i];
        if (i < 16) {
            std::cout << "0x" << std::hex << (0x100 + i) << ": 0x" << int(cpu.memory[0x100 + i]) << std::endl;
        }
    }

   
    std::cout << "Bytes from 0x100 to 0x120: ";
    for (int i = 0; i < 32; ++i) {
        std::cout << std::hex << int(cpu.memory[0x100 + i]) << " ";
    }
    std::cout << std::endl;

    return true;
}

int main() {
    // Create an instance of the CPU
    CPU cpu;

    // Load ROM into CPU memory
    loadROM(cpu, "Pokemon Red (UE) [S][!].gb");
    std::cout << "Starting execution at PC: 0x" << std::hex << int(cpu.PC) << std::endl;

    // Simulate CPU execution
    while (true) {
        uint8_t opcode = cpu.read(cpu.PC);  // Fetch opcode
        std::cout << "PC: 0x" << std::hex << int(cpu.PC)
                  << " -> Executing opcode: 0x" << int(opcode) << std::endl;
        
        std::cout << "Before executing, PC: 0x" << std::hex << int(cpu.PC) << std::endl;
        cpu.executeOpcode(opcode);
        std::cout << "After executing, PC: 0x" << std::hex << int(cpu.PC) << std::endl;
                  
        
        cpu.PC++; // Increment PC after executing the opcode
        if (cpu.PC > 0x0100) { // Break condition for debugging (example)
            break;
        }
    }
    
}