#include "memory.h"

Memory::Memory() {
    memory.resize(0x10000, 0);
}


void Memory::loadROM(const std::string& filename) {
    std::ifstream romFile(filename, std::ios::binary);
    if (!romFile) {
        std::cerr << " Failed to open ROM File!" << std::endl;
        exit(1);
    } else {
        std::cerr << "Rom opened properly!" <<std::endl;
    }
    romData.assign(std::istreambuf_iterator<char>(romFile), std::istreambuf_iterator<char>());
    romFile.close();

    mbcType = romData[0x147];
    std::cout << "Loaded ROM, MBC Type:" << int(mbcType) <<std::endl;

    // Resize RAM based on MBC
    if (mbcType == 0x02 || mbcType == 0x03) {
        ramBanks.resize(4 * 0x2000, 0);
    } else if (mbcType == 0x05 || mbcType == 0x06) {
        ramBanks.resize(1 * 0x2000, 0);
    } else if (mbcType == 0x0F || mbcType == 0x10 || mbcType == 0x11 || mbcType == 0x12 || mbcType == 0x13) {
        ramBanks.resize(16 * 0x2000, 0);
    } else if (mbcType == 0x19 || mbcType == 0x1A || mbcType == 0x1B) {
        ramBanks.resize(16 * 0x2000, 0);
    }
}


uint8_t Memory::read(uint16_t addr) {
    if (addr <= 0x3FFF) {
        return romData[addr]; // Always Bank 0
    } else if (addr >= 0x4000 && addr <= 0x7FFF) {
        size_t offset = (currentROMBank * 0x4000) + (addr - 0x4000);
        return romData[offset % romData.size()];
    } else if (addr >= 0xA000 && addr <= 0xBFFF && ramEnabled) {
        return ramBanks[(currentRAMBank * 0x2000) + (addr - 0xA000)];
    } else {
        return memory[addr];
    }
}

void Memory::write(uint16_t addr, uint8_t value) {
    if (addr >= 0x2000 && addr <= 0x3FFF) { 
        if (mbcType == 1) {
            currentROMBank = value & 0x1F;
            if (currentROMBank == 0) currentROMBank = 1; 
            std::cout << "Switched to ROM Bank: " << int(currentROMBank) << std::endl;
        }
    } else if (addr >= 0x4000 && addr <= 0x5FFF) {
        if (mbcType == 1) {
            currentRAMBank = value & 0x03;
            std::cout << "Switched to RAM Bank: " << int(currentRAMBank) << std::endl;
        }
    } else if (addr >= 0xA000 && addr <= 0xBFFF && ramEnabled) {
        ramBanks[(currentRAMBank * 0x2000) + (addr - 0xA000)] = value;
    } else {
        memory[addr] = value;
    }
}
