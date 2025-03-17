#include "ppu.h"
#include <iostream>

PPU::PPU(Memory& memory)
    reset();
}

PPU::~PPU() {
    // Clean up SDL resources
  
}

void PPU::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow("Game Boy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    if (!texture) {
        std::cerr << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
}


void PPU::renderScanline() {
    // Implement scanline rendering
}

void PPU::renderSprites() {
    // Implement sprite rendering
}

void PPU::step(int cycles) {
    // Advance the PPU by the given number of cycles
}

void PPU::updatePalettes() {
    // Update color palettes based on BGP, OBP0, and OBP1 registers
}

uint8_t PPU::getColor(uint8_t colorID, bool isSprite) {
    // Get the color from the appropriate palette
    return 0; // Placeholder
}








// so this renderframe will serve to updte the different aspects of the sdl2 rendering and textures
void PPU::renderFrame() {
    SDL_UpdateTexture(texture, nullptr, framebuffer, 160 * sizeof(uint32_t));

    // Clear the renderer
    SDL_RenderClear(renderer);

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // Present the renderer
    SDL_RenderPresent(renderer);
}