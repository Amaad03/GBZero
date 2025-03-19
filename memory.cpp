#include "memory.h"
#include <iostream>
#include <cstring>


Memory::Memory() {
    rom.resize(0x200000); 
    ram.resize(0x8000);  
    std::fill(vram, vram + 0x2000, 0);
    std::fill(wram, wram + 0x2000, 0);
    std::fill(oam, oam + 0xA0, 0);
    std::fill(hram, hram + 0x7F, 0);
    ie = 0;
    ramEnabled = false;
    currentROMBank = 1;
    currentRAMBank = 0;
    mbc1Mode = false;
    std::fill_n(io_registers, 0x80, 0x00);

    io_registers[0xff] = 0x00;

    io_registers[0x43] = 0x00;
    io_registers[0x42] = 0x00;
    io_registers[0x01] = 0x00;
    io_registers[0x02] = 0x7e;
    io_registers[0x4b] = 0x00;
    io_registers[0x4a] = 0x00;
    io_registers[0x06] = 0x00;
    io_registers[0x07] = 0xf8;
    io_registers[0x47] = 0xFC;
    io_registers[0x48] = 0x00;
    io_registers[0x49] = 0x00;
    io_registers[0x44] = 0x00;   // This is FF44
    io_registers[0x0F] = 0xE1;   // This is FF0F
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
    romFile.read(reinterpret_cast<char*>(rom.data()), size);
    romFile.close();

    std::cout << "[DEBUG] ROM file size: " << size << " bytes." << std::endl;
    
    mbcType = rom[0x147]; 
}



uint8_t Memory::read(uint16_t addr) {
    // Cartridge ROM (0x0000–0x7FFF)
    if (addr <= 0x7FFF) {
        if (addr <= 0x3FFF) {
            return rom[addr];
        } else {
            return rom[(currentROMBank * 0x4000) + (addr - 0x4000)];
        }
    }

    // Video RAM (0x8000–0x9FFF)
    if (addr >= 0x8000 && addr <= 0x9FFF) {
        return vram[addr - 0x8000];
    }

    // External RAM (0xA000–0xBFFF)
    if (addr >= 0xA000 && addr <= 0xBFFF) {
        if (ramEnabled) {
            return ram[(currentRAMBank * 0x2000) + (addr - 0xA000)];
        } else {
            return 0xFF; 
        }
    }

    // Work RAM (0xC000–0xDFFF) and Echo RAM (0xE000–0xFDFF)
    if (addr >= 0xC000 && addr <= 0xFDFF) {
        return wram[addr - 0xC000];
    }

    // Sprite Attribute Table (OAM, 0xFE00–0xFE9F)
    if (addr >= 0xFE00 && addr <= 0xFE9F) {
        return oam[addr - 0xFE00];
    }

    // Unusable Memory (0xFEA0–0xFEFF)
    if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        return 0xFF;
    }

    // IO Registers (0xFF00–0xFF7F)
    if (addr >= 0xFF00 && addr <= 0xFF7F) {
        // Debugging
        std::cout << "[DEBUG] Read from I/O register: 0x" << std::hex << addr
                  << ", Value: 0x" << static_cast<int>(io_registers[addr - 0xFF00]) << std::endl;
        return io_registers[addr - 0xFF00];
    }

    // High RAM (HRAM, 0xFF80–0xFFFE)
    if (addr >= 0xFF80 && addr <= 0xFFFE) {
        return hram[addr - 0xFF80];
    }

    // Interrupt Enable Register (0xFFFF)
    if (addr == 0xFFFF) {
        return ie;
    }

    // Invalid memory access
    std::cout << "[DEBUG] Invalid memory access at address: 0x" << std::hex << addr << std::endl;
    return 0xFF;
}

void Memory::write(uint16_t addr, uint8_t value) {
    // Cartridge ROM (0x0000–0x7FFF)
    if (addr <= 0x7FFF) {
        switch (mbcType) {
            case 1: handleMBC1Write(addr, value); break;
            case 2: handleMBC2Write(addr, value); break;
            case 3: handleMBC3Write(addr, value); break;
            default: break;
        }
        return;
    }

    // Handle RAM writes
    if (addr >= 0xA000 && addr <= 0xBFFF) {
        if (ramEnabled && !ram.empty()) {
            ram[(currentRAMBank * 0x2000) + (addr - 0xA000)] = value;
        } else {
            std::cerr << "[ERROR] Attempted to write to unallocated or disabled RAM at 0x" << std::hex << addr << std::endl;
        }
        return;
    }

    // Handle VRAM writes
    if (addr >= 0x8000 && addr <= 0x9FFF) {
        vram[addr - 0x8000] = value;
        return;
    }

    // Handle WRAM writes
    if (addr >= 0xC000 && addr <= 0xDFFF) {
        wram[addr - 0xC000] = value;
        return;
    }

    // Handle OAM writes
    if (addr >= 0xFE00 && addr <= 0xFE9F) {
        oam[addr - 0xFE00] = value;
        return;
    }

    // Handle HRAM writes
    if (addr >= 0xFF80 && addr <= 0xFFFE) {
        hram[addr - 0xFF80] = value;
        return;
    }

    // Handle Interrupt Enable Register write (0xFFFF)
    if (addr == 0xFFFF) {
        ie = value;
        return;
    }

    // Handle I/O register writes (0xFF00–0xFF7F)
    if (addr >= 0xFF00 && addr <= 0xFF7F) {
        std::cout << "[DEBUG] Writing to I/O register: 0x" << std::hex << addr
                  << ", Value: 0x" << static_cast<int>(value) << std::endl;
        io_registers[addr - 0xFF00] = value;
        return;
    }

    // Handle invalid memory writes
    std::cerr << "[ERROR] Invalid memory write at address: 0x" << std::hex << addr << std::endl;
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
/*
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
        default:
            io_registers[addr - 0xFF00] = value;
            break;
    }
}
*/


void Memory::performDMATransfer(uint8_t value) {
    uint16_t startAddress = value * 0x100; // 0x100 is 256 bytes per transfer
    for (int i = 0; i < 0xA0; i++) { // 0xA0 = 160 bytes for OAM
        oam[i] = read(startAddress + i); // Transfer data from RAM to OAM
    }
}
