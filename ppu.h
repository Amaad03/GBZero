#ifndef PPU_H
#define PPU_H

#include "memory.h"
#include <array>
#include <cstdint>
#include <SDL2/SDL.h>

class PPU { 
public:
    PPU(Memory& memory); // Constructor
    ~PPU();              // Destructor

    void step(unsigned int cycles); // Step through the PPU for a certain number of cycles
    void renderFrame();             // Render the current frame
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    void initializeSDL(); // Initialize SDL and create window/renderer
private:
    // Reference to memory
    Memory& memory;

    // PPU Registers
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

    // Internal state
    unsigned int clock = 0;       // Track cycles for PPU timing
    uint8_t currentScanline = 0;  // Current scanline being rendered
    std::array<uint8_t, 160 * 144> framebuffer; // Framebuffer for the final image

    // SDL resources


    // Helper functions

    void renderScanline(); // Render the current scanline
    void renderSpritesLine(uint8_t scanline); // Render sprites for the current scanline
    void updateLCDStatus(); // Update the LCD status

    // Utility functions
    bool isBackgroundEnabled() const;
    bool isWindowEnabled() const;
    uint8_t getBackgroundTileData(uint8_t x, uint8_t y) const;
    uint8_t getWindowTileData(uint8_t x, uint8_t y) const;
    uint8_t getColor(uint8_t colorId, uint8_t palette) const;
};

#endif // PPU_H