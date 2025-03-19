#ifndef PPU_H
#define PPU_H

#include "memory.h"
#include <array>
#include <cstdint>
#include <SDL2/SDL.h>
#include "cpu.h"

class PPU {
public:
    Memory& memory;
    CPU& cpu;

    uint8_t& LCDC; // LCD Control (0xFF40)
    uint8_t& STAT; // LCD Status (0xFF41)
    uint8_t& SCY;  // Scroll Y (0xFF42)
    uint8_t& SCX;  // Scroll X (0xFF43)
    uint8_t& LY;   // LCD Y Coordinate (0xFF44)
    uint8_t& LYC;  // LY Compare (0xFF45)
    uint8_t& BGP;  // Background Palette (0xFF47)
    uint8_t& OBP0; // Object Palette 0 (0xFF48)
    uint8_t& OBP1; // Object Palette 1 (0xFF49)
    uint8_t& WY;   // Window Y Position (0xFF4A)
    uint8_t& WX;   // Window X Position (0xFF4B)

    uint32_t framebuffer[160 * 144]; // Framebuffer for the display

    PPU(CPU& cpu, Memory& memory);
    ~PPU();

    uint8_t currentScanline = 0;
    uint8_t currentMode = 0;

    std::array<uint32_t, 4> bgPalette;       // Background palette colors
    std::array<uint32_t, 4> spritePalette0;  // Sprite palette 0 colors
    std::array<uint32_t, 4> spritePalette1;  // Sprite palette 1 colors

    // SDL2 members
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    void reset();
    void renderScanline(uint8_t currentScanline);
    void renderSprites(uint8_t currentScanline);
    void step(int cycles);
    void updatePalettes();
    void clearFramebuffer();
    void initializeSDL();
    void renderFrame();
    void updateLCDStatus();
    uint8_t calculatePixelColor(uint8_t x, uint8_t y);
    uint8_t getSpritePixelColor(uint8_t x, uint8_t y);
    uint8_t getBackgroundPixelColor(uint8_t x, uint8_t y);

private:
    // Helper methods
    uint8_t getColor(uint8_t colorID, bool isSprite);
};

#endif // PPU_H