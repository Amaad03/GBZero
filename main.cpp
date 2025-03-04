#include "cpu.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
// Function to load ROM into CPU memory
int main() {
    // Initialize CPU
    CPU cpu;

    // Open the ROM file
    std::ifstream romFile("Tetris.gb", std::ios::binary);
    if (!romFile) {
        std::cerr << "Failed to open ROM file!" << std::endl;
        return 1;
    } else {
        std::cerr <<"Rom opened properly" << std::endl;
    }

    // Read the ROM into a vector
    std::vector<uint8_t> romData((std::istreambuf_iterator<char>(romFile)),
                                 std::istreambuf_iterator<char>());
    romFile.close(); // Close file after reading


    for (size_t i = 0; i < romData.size(); ++i) {
        if (0x100 + i >= 0x10000) {
            std::cerr << "ROM too large to fit in memory!" << std::endl;
            exit(1);
        }
        cpu.memory[0x100 + i] = romData[i];
    }
    std::cout << "ROM loaded successfully. Starting PC: 0x" << std::hex << int(cpu.PC) << std::endl;
    
    std::cout << "Entering main emulation loop..." << std::endl;

    // Main emulation loop
    while (true) {
        // Fetch the opcode at the current PC
        uint8_t opcode = cpu.read(cpu.PC);
        std::cout << "PC: 0x" << std::hex << int(cpu.PC) << " Fetching opcode: 0x" << std::hex << int(opcode) << std::endl;

        // Execute the opcode
        cpu.executeOpcode(opcode);

        // Optional: Sleep for a short time (for debugging purposes)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Adjust timing as needed
    }

    return 0;
}

