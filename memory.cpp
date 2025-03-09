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
    ramEnabled = true;
}

void Memory::loadROM(const std::string& filename) {
    std::ifstream romFile(filename, std::ios::binary);
    if (!romFile) {
        throw std::runtime_error("Failed to open ROM file.");
    } else {
        std::cout << "ROM opened successfully" << std::endl;
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
        printf("%02X %02X %02X %02X %02X \n", rom[0x100], rom[0x101], rom[0x102], rom[0x103], rom[0x104]);
    } else {
        std::cerr << "[ERROR] ROM is too small!" << std::endl;
    }

    mbcType = rom[0x147]; // Get MBC type
}



uint8_t Memory::read(uint16_t addr) {
    if (addr >= 0xFF00 && addr <= 0xFFFF) {
        return io_registers[addr - 0xFF00]; // MMIO Registers
    }
    if (addr <= 0x3FFF) {
        return rom[addr]; // ROM Bank 0
    } else if (addr >= 0x4000 && addr <= 0x7FFF) {
        return rom[(currentROMBank * 0x4000) + (addr - 0x4000)];
    } else if (addr >= 0xA000 && addr <= 0xBFFF && ramEnabled) {
        return ram[(currentRAMBank * 0x2000) + (addr - 0xA000)];
    } else if (addr >= 0x8000 && addr <= 0x9FFF) {
        return vram[addr - 0x8000];
    } else if (addr >= 0xC000 && addr <= 0xDFFF) {
        return wram[addr - 0xC000];
    } else if (addr >= 0xE000 && addr <= 0xFDFF) {
        return wram[addr - 0xE000]; // Echo RAM (Mirror of WRAM)
    } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        return oam[addr - 0xFE00];
    } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
        return hram[addr - 0xFF80];
    } else if (addr == 0xFFFF) {
        return ie; // Interrupt Enable Register
    }

    std::cout << "[DEBUG] Invalid memory access at address: " << std::hex << addr << std::endl;
    return 0xFF;
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
    }

    if (addr >= 0xA000 && addr <= 0xBFFF && ramEnabled) {
        if (!ram.empty() && ramEnabled) {
            ram[(currentRAMBank * 0x2000) + (addr - 0xA000)] = value;
        } else {
            printf("[ERROR] Attempted to write to unallocated RAM at 0x%04X\n", addr);
        }
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
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        handleIOWrite(addr, value);
    }
}

void Memory::handleMBC1Write(uint16_t addr, uint8_t value) {
    if (addr <= 0x1FFF) {
        // Enable/disable RAM
        ramEnabled = (value & 0x0F) == 0x0A;
        printf("[DEBUG] RAM Enable Write: Value = 0x%02X, RAM Enabled = %d\n", value, ramEnabled);
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        // Set lower 5 bits of ROM bank number
        uint8_t romBank = value & 0x1F;
        if (romBank == 0) romBank = 1; // Bank 0 is not allowed

        currentROMBank = (currentROMBank & 0xE0) | romBank;
    } else if (addr >= 0x4000 && addr <= 0x5FFF) {
        // Set upper 2 bits of ROM bank number or RAM bank number
        if (mbc1Mode == 0) {
            // Mode 0: Affects ROM banking
            currentROMBank = (currentROMBank & 0x1F) | ((value & 0x03) << 5);
        } else {
            // Mode 1: Affects RAM banking
            currentRAMBank = value & 0x03; // Only 2 bits for RAM banks 0-3
        }
    } else if (addr >= 0x6000 && addr <= 0x7FFF) {
        // Mode select: 0 = Large ROM (default), 1 = Small ROM + RAM banking
        mbc1Mode = value & 0x01;
    }
}

void Memory::handleMBC2Write(uint16_t addr, uint8_t value) {
    if (addr <= 0x1FFF) {
        // Enable RAM (only if bit 8 of addr is 0)
        if ((addr & 0x0100) == 0) {
            ramEnabled = (value & 0x0F) == 0x0A;
        }
    } else if (addr >= 0x2100 && addr <= 0x3FFF) {
        // Set ROM bank (only lower 4 bits matter)
        currentROMBank = value & 0x0F;
        if (currentROMBank == 0) currentROMBank = 1; // Bank 0 is not allowed
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

void Memory::handleIOWrite(uint16_t addr, uint8_t value) {
    switch (addr) {
        case 0xFF04: // DIV register reset
            io_registers[0x04] = 0;
            break;
        case 0xFF44: // LY register reset
            io_registers[0x44] = 0;
            break;
        case 0xFF46: // DMA Transfer
            performDMATransfer(value);
            break;
        case 0xFF50: // Disable boot ROM
            bootROMEnabled = false;
            break;
        default:
            io_registers[addr - 0xFF00] = value;
            break;
    }
}

void Memory::performDMATransfer(uint8_t value) {
    uint16_t startAddress = value * 0x100; // 0x100 is 256 bytes per transfer
    for (int i = 0; i < 0xA0; i++) { // 0xA0 = 160 bytes for OAM
        oam[i] = read(startAddress + i); // Transfer data from RAM to OAM
    }
}