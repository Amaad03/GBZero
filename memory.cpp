#include "memory.h"

Memory::Memory() {
    memory.resize(0x10000, 0);
}


void Memory::loadROM(const std::string& filename) {
    std::ifstream romFile(filename, std::ios::binary);
    if (!romFile) {
        std::cerr << "Failed to open ROM File!" << std::endl;
        exit(1);
    } else {
        std::cerr << "Rom opened properly!" << std::endl;
    }
    romData.assign(std::istreambuf_iterator<char>(romFile), std::istreambuf_iterator<char>());
    romFile.close();

    mbcType = romData[0x147];  // Read MBC type from the ROM header
    std::cout << "Loaded ROM, MBC Type: " << int(mbcType) << std::endl;

    // Resize RAM based on MBC type
    switch (mbcType) {
        case 0x00: // ROM Only
            break;
        case 0x01: // MBC1
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x02: // MBC1 + RAM
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x03: // MBC1 + RAM + BATTERY
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x05: // MBC2
            ramBanks.resize(1 * 0x2000, 0);
            break;
        case 0x06: // MBC2 + BATTERY
            ramBanks.resize(1 * 0x2000, 0);
            break;
        case 0x08: // ROM + RAM
            break;
        case 0x09: // ROM + RAM + BATTERY
            break;
        case 0x0B: // MMM01
            break;
        case 0x0C: // MMM01 + RAM
            break;
        case 0x0D: // MMM01 + RAM + BATTERY
            break;
        case 0x0F: // MBC3 + TIMER + BATTERY
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x10: // MBC3 + TIMER + RAM + BATTERY
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x11: // MBC3
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x12: // MBC3 + RAM
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x13: // MBC3 + RAM + BATTERY
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x19: // MBC5
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x1A: // MBC5 + RAM
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x1B: // MBC5 + RAM + BATTERY
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x1C: // MBC5 + RUMBLE
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x1D: // MBC5 + RUMBLE + RAM
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x1E: // MBC5 + RUMBLE + RAM + BATTERY
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x20: // MBC6
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0x22: // MBC7 + SENSOR + RUMBLE + RAM + BATTERY
            ramBanks.resize(4 * 0x2000, 0);
            break;
        case 0xFC: // POCKET CAMERA
            break;
        case 0xFD: // BANDAI TAMA5
            break;
        case 0xFE: // HuC3
            break;
        case 0xFF: // HuC1 + RAM + BATTERY
            break;
        default:
            std::cerr << "Unsupported MBC Type: " << int(mbcType) << std::endl;
            exit(1);  // Exit since the emulator can't continue with an unsupported MBC
    }
}


uint8_t Memory::read(uint16_t addr) {
    // Read from ROM bank 0 (0x0000 - 0x3FFF)
    if (addr <= 0x3FFF) {
        return romData[addr]; // Always Bank 0 for addresses 0x0000 - 0x3FFF
    }
    
    // Read from ROM bank (0x4000 - 0x7FFF)
    else if (addr >= 0x4000 && addr <= 0x7FFF) {
        // Calculate offset for the current ROM bank
        size_t offset = (currentROMBank * 0x4000) + (addr - 0x4000);
        if (offset < romData.size()) {
            return romData[offset]; // Ensure we're within bounds of ROM data
        } else {
            // Handle ROM read out-of-bounds (if necessary)
            return 0xFF; // Return a default value (or handle error)
        }
    }

    // Read from RAM bank (0xA000 - 0xBFFF)
    else if (addr >= 0xA000 && addr <= 0xBFFF) {
        if (ramEnabled) {
            // Calculate the offset within the RAM bank
            size_t offset = (currentRAMBank * 0x2000) + (addr - 0xA000);
            if (offset < ramBanks.size()) {
                return ramBanks[offset]; // Ensure we're within the bounds of RAM
            } else {
                // Handle RAM read out-of-bounds (if necessary)
                return 0xFF; // Return a default value (or handle error)
            }
        }
        else {
            // Return 0xFF if RAM is not enabled
            return 0xFF;
        }
    }
    
    // Default case - Invalid address or unimplemented regions
    return 0xFF; // Return a default value for invalid addresses
}


void Memory::write(uint16_t addr, uint8_t value) {
    // Handle ROM bank selection (0x2000 - 0x3FFF)
    if (addr >= 0x2000 && addr <= 0x3FFF) { 
        if (mbcType == 1 || mbcType == 0x19) { // MBC1 or MBC5
            currentROMBank = value & 0x1F;
            if (currentROMBank == 0) currentROMBank = 1; // ROM bank 0 cannot be selected
            std::cout << "Switched to ROM Bank: " << int(currentROMBank) << std::endl;
        }
    }
    // Handle RAM bank selection (0x4000 - 0x5FFF)
    else if (addr >= 0x4000 && addr <= 0x5FFF) {
        if (mbcType == 1 || mbcType == 0x19) { // MBC1 or MBC5
            currentRAMBank = value & 0x03;
            std::cout << "Switched to RAM Bank: " << int(currentRAMBank) << std::endl;
        }
    }
    // Enable RAM (0x0000 - 0x1FFF)
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        if (value == 0xA0) { // A0 enables RAM for MBC1, MBC5
            ramEnabled = true;
            std::cout << "RAM Enabled" << std::endl;
        } else {
            ramEnabled = false;
            std::cout << "RAM Disabled" << std::endl;
        }
    }
    // Write to RAM (0xA000 - 0xBFFF)
    else if (addr >= 0xA000 && addr <= 0xBFFF && ramEnabled) {
        if (addr >= 0xA000 && addr <= 0xBFFF) {
            ramBanks[(currentRAMBank * 0x2000) + (addr - 0xA000)] = value;
            std::cout << "Writing " << int(value) << " to address: " << std::hex << addr << std::endl;
        } else {
            std::cerr << "Invalid memory write at address: " << std::hex << addr << std::endl;
        }
    }
    // Write to VRAM (0x8000 - 0x9FFF)
    else if (addr >= 0x8000 && addr <= 0x9FFF) {
        memory[addr] = value;  // Allow writes to VRAM
        std::cout << "Writing " << int(value) << " to VRAM address: " << std::hex << addr << std::endl;
    }
    // Default memory write
    else {
        memory[addr] = value;
        std::cout << "Writing " << int(value) << " to address: " << std::hex << addr << std::endl;
    }
}