#ifndef PPU_H
#define PPU_H

#include "memory.h"
#include <array>
#include <cstdint>
#include <SDL2/SDL.h>

class PPU {
public:

    uint8_t LCDControlRegister = 0;
    uint8_t SCYRegister = 0;        // Scroll Y
    uint8_t SCXRegister = 0;        // Scroll X
    uint8_t BGPRegister = 0;        // Background Palette
    uint8_t OBP0Register = 0;      // Object Palette 0 (Sprites)
    uint8_t OBP1Register = 0;      // Object Palette 1 (Sprites)


    uint32_t framebuffer[160*144];


    uint8_t currentScanline = 0;
    uint8_t currentMode = 0;

    std::array<uint32_t, 4> bgPalette;  
    std::array<uint32_t, 4> spritePalette0;
    std::array<uint32_t, 4> spritePalette1; 

    // SDL2 members
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;



    void reset();
    void renderScanline();
    void renderSprites();
    void step(int cycles);
    void updatePalettes();

    void initializeSDL();
    void renderFrame();


    private:
    // Helper methods
    uint8_t getColor(uint8_t colorID, bool isSprite);
};







#endif // PPU_H