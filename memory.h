#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <stdexcept>

class Memory {
    public:


    
        std::vector<uint8_t> rom; // ROM data (up to 2MB for larger games)
        std::vector<uint8_t> ram; // External RAM (up to 32KB for MBCs)
        uint8_t vram[0x2000];     // Video RAM (8KB)
        uint8_t wram[0x2000];     // Work RAM (8KB)
        uint8_t oam[0xA0];        // Sprite Attribute Table (OAM, 160 bytes)
        uint8_t hram[0x7F];       // High RAM (127 bytes)
        uint8_t io_registers[0x80]; // IO Registers
        uint8_t ie;               // Interrupt Enable Register
        
     
        uint8_t currentROMBank; // Current ROM bank (default to 1)

        uint8_t currentRAMBank; // Current RAM bank (default to 0)

        bool ramEnabled ; 
        uint8_t mbcType ;        // MBC type (0 = no MBC, 1 = MBC1, etc.)
        bool mbc1Mode ;      // Mode flag for MBC1 (0 = ROM banking, 1 = RAM banking)


        Memory();
      
        void loadROM(const std::string& romFilename);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);
  

    private:



        void handleMBC1Write(uint16_t addr, uint8_t value);
        void performDMATransfer(uint8_t value);
        void handleMBC2Write(uint16_t addr, uint8_t value);
        void handleMBC3Write(uint16_t addr, uint8_t value);
        void handleIOWrite(uint16_t addr, uint8_t value);

    };
    

#endif