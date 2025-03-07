#include "memory.h"
#include <iostream>

Memory::Memory() {
    // Initialize memory regions
    rom.resize(0x200000); // 2MB ROM (max size)
    ram.resize(0x8000);   // 32KB RAM (max size)
    std::fill(vram, vram + 0x2000, 0);
    std::fill(wram, wram + 0x2000, 0);
    std::fill(oam, oam + 0xA0, 0);
    std::fill(hram, hram + 0x7F, 0);
    ie = 0;
}

void Memory::loadROM(const std::string& filename) {
    std::ifstream romFile(filename, std::ios::binary);
    if (!romFile) {
        throw std::runtime_error("Failed to open ROM file.");
    } else {
        std::cout<<"ROM opened successfully" <<std::endl;
    }

    romFile.seekg(0, std::ios::end);
    std::streamsize size = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    if (size == 0) {
        throw std::runtime_error("ROM file is empty.");
    }

    rom.resize(size);

    // Read file byte-by-byte
    for (size_t i = 0; i < static_cast<size_t>(size); ++i) {
        char byte;
        romFile.read(&byte, 1);
        rom[i] = static_cast<uint8_t>(byte);
    }

    std::cout << "[DEBUG] ROM file size: " << size << " bytes." << std::endl;

    // Debugging: Print first 16 bytes
    std::cout << "[DEBUG] First 16 bytes of ROM: ";
    for (int i = 0; i < 16; ++i) {
        printf("%02X ", rom[i]);
    }
    std::cout << std::endl;

    // Check entry point
    if (rom.size() > 0x100) {
        std::cout << "[DEBUG] Entry point instruction: ";
        printf("%02X %02X %02X\n", rom[0x100], rom[0x101], rom[0x102]);
    } else {
        std::cerr << "[ERROR] ROM is too small!\n";
    }

    mbcType = rom[0x147]; // Get MBC type
}




uint8_t Memory::read(uint16_t addr) {
    if (addr <= 0x3FFF) {
        return rom[addr]; // ROM Bank 0
    } else if (addr >= 0x4000 && addr <= 0x7FFF) {
        return rom[(currentROMBank * 0x4000) + (addr - 0x4000)]; // Switchable ROM Bank
    } else if (addr >= 0xA000 && addr <= 0xBFFF && ramEnabled) {
        return ram[(currentRAMBank * 0x2000) + (addr - 0xA000)];
    } else if (addr >= 0x8000 && addr <= 0x9FFF) {
        return vram[addr - 0x8000]; // VRAM
    } else if (addr >= 0xC000 && addr <= 0xDFFF) {
        return wram[addr - 0xC000]; // WRAM
    } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        return oam[addr - 0xFE00]; // OAM
    } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
        return hram[addr - 0xFF80]; // HRAM
    } else if (addr == 0xFFFF) {
        return ie; // Interrupt Enable Register
    }

    // Debugging: log any invalid address accesses
    std::cout << "[DEBUG] Invalid memory access at address: " << std::hex << addr << std::endl;
    return 0xFF; // Default value for unmapped memory
}

void Memory::write(uint16_t addr, uint8_t value) {
    if (addr <= 0x7FFF) {
        // Handle MBC writes
        switch (mbcType) {
            case 1: handleMBC1Write(addr, value); break;
            case 2: handleMBC2Write(addr, value); break;
            case 3: handleMBC3Write(addr, value); break;
            // Add more MBC types as needed
            default: break;
        }
    } else if (addr >= 0xA000 && addr <= 0xBFFF && ramEnabled) {
        // External RAM
        ram[(currentRAMBank * 0x2000) + (addr - 0xA000)] = value;
    } else if (addr >= 0x8000 && addr <= 0x9FFF) {
        vram[addr - 0x8000] = value; // VRAM
    } else if (addr >= 0xC000 && addr <= 0xDFFF) {
        wram[addr - 0xC000] = value; // WRAM
    } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        oam[addr - 0xFE00] = value; // OAM
    } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
        hram[addr - 0xFF80] = value; // HRAM
    } else if (addr == 0xFFFF) {
        ie = value; // Interrupt Enable Register
    }
}

void Memory::handleMBC1Write(uint16_t addr, uint8_t value) {
    if (addr <= 0x1FFF) {
        // Enable/disable RAM
        ramEnabled = (value & 0x0F) == 0x0A;
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        // Set lower 5 bits of ROM bank number
        currentROMBank = (currentROMBank & 0xE0) | (value & 0x1F);
        if (currentROMBank == 0) currentROMBank++; // Bank 0 is not allowed
    } else if (addr >= 0x4000 && addr <= 0x5FFF) {
        // Set upper 2 bits of ROM bank number or RAM bank number
        currentROMBank = (currentROMBank & 0x1F) | ((value & 0x03) << 5);
    } else if (addr >= 0x6000 && addr <= 0x7FFF) {
        // MBC1 mode switching (not implemented here)
    }
}

void Memory::handleMBC2Write(uint16_t addr, uint8_t value) {
    if (addr <= 0x1FFF) {
        // RAM enable/disable (only lower bit of address matters)
        if ((addr & 0x0100) == 0) {
            ramEnabled = (value & 0x0F) == 0x0A;
        }
    } else if (addr >= 0x2100 && addr <= 0x3FFF) {
        // ROM bank selection (only lower 4 bits matter)
        currentROMBank = value & 0x0F;
        if (currentROMBank == 0) currentROMBank++; // Bank 0 is not allowed
    }
}

void Memory::handleMBC3Write(uint16_t addr, uint8_t value) {
    if (addr <= 0x1FFF) {
        // RAM and RTC enable/disable
        ramEnabled = (value & 0x0F) == 0x0A;
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        // ROM bank selection
        currentROMBank = value & 0x7F; // 7 bits for ROM bank
        if (currentROMBank == 0) currentROMBank++; // Bank 0 is not allowed
    } else if (addr >= 0x4000 && addr <= 0x5FFF) {
        // RAM bank or RTC register selection
        if (value <= 0x03) {
            currentRAMBank = value; // RAM bank 0-3
        } else if (value >= 0x08 && value <= 0x0C) {
            // Select RTC register (e.g., seconds, minutes, hours, etc.)
            // Implement RTC handling here
        }
    } else if (addr >= 0x6000 && addr <= 0x7FFF) {
        // Latch RTC data (optional)
        // Implement RTC latching here
    }
}