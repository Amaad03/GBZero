#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <iostream>
#include <fstream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>

class Memory {
public: 
    std::vector<uint8_t> memory;
    std::vector<uint8_t> romData;
    std::vector<uint8_t> ramBanks;


    uint8_t currentROMBank = 1;
    uint8_t currentRAMBank = 0;
    bool ramEnabled = false;
    uint8_t mbcType = 0;

    Memory();

    void loadROM(const std::string& filename);
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);
};

#endif
