#ifndef PPU_H
#define PPU_H

#include "memory.h"
#include <array>
#include <cstdint>
#include <SDL2/SDL.h>


class PPU {
public:
    PPU(Memory& memory);
    ~PPU(); 
    //~PPU();
    void step(unsigned int cycles);
    void renderScanline();
    void renderBackground();
    bool handleEvents();
    void renderWindow();
    void renderSprites();
    void drawPixel(uint8_t x, uint8_t y, uint8_t color);

    // Framebuffer to store the final rendered image
    std::array<uint8_t, 160 * 144 * 3> framebuffer; // 160x144 pixels, 3 bytes per pixel (RGB)
    void initializeSDL(); // Initialize SDL
    void renderFrame();    // Display the frame on the SDL window
   
private:
    Memory& memory;
    SDL_Window* window;         // Declare the SDL Window
    SDL_Renderer* renderer;     // Declare the SDL Renderer
    SDL_Texture* texture;       // Declare the SDL Texture
    
    // PPU registers
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

    // Internal counters
    unsigned int clock = 0; // Tracks cycles for PPU timing
    //uint8_t currentScanline = 0; // Current scanline being rendered

    // Helper functions
    bool isLCDEnabled() const;
    bool isBackgroundEnabled() const;
    bool isWindowEnabled() const;
    bool isSpritesEnabled() const;
    uint8_t getBackgroundTileData(uint8_t x, uint8_t y) const;
    uint8_t getWindowTileData(uint8_t x, uint8_t y) const;
    uint8_t getSpriteTileData(uint8_t tileIndex, uint8_t line) const;
    uint8_t getColor(uint8_t colorId, uint8_t palette) const;
};

#endif