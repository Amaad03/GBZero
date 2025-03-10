#include "ppu.h"
#include <iostream>
#include <SDL2/SDL.h>

PPU::PPU(Memory& memory)
    : memory(memory),
      
      LCDC(memory.io_registers[0x40]), // 0xFF40 - 0xFF00 = 0x40
      STAT(memory.io_registers[0x41]), // 0xFF41 - 0xFF00 = 0x41
      SCY(memory.io_registers[0x42]),  // 0xFF42 - 0xFF00 = 0x42
      SCX(memory.io_registers[0x43]),  // 0xFF43 - 0xFF00 = 0x43
      LY(memory.io_registers[0x44]),   // 0xFF44 - 0xFF00 = 0x44
      LYC(memory.io_registers[0x45]),  // 0xFF45 - 0xFF00 = 0x45
      BGP(memory.io_registers[0x47]),  // 0xFF47 - 0xFF00 = 0x47
      OBP0(memory.io_registers[0x48]), // 0xFF48 - 0xFF00 = 0x48
      OBP1(memory.io_registers[0x49]), // 0xFF49 - 0xFF00 = 0x49
      WY(memory.io_registers[0x4A]),   // 0xFF4A - 0xFF00 = 0x4A
      WX(memory.io_registers[0x4B]) {  // 0xFF4B - 0xFF00 = 0x4B
    initializeSDL(); // Initialize SDL
}

void PPU::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Create an SDL window
    window = SDL_CreateWindow("Game Boy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Create a renderer for the window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Create a texture to render the framebuffer
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, 160, 144);
    if (!texture) {
        std::cerr << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
}

void PPU::renderFrame() {
    // Update the texture with the current framebuffer
    SDL_UpdateTexture(texture, NULL, framebuffer.data(), 160 * 3);

    // Clear the renderer
    SDL_RenderClear(renderer);

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Present the renderer
    SDL_RenderPresent(renderer);
}

void PPU::step(unsigned int cycles) {
    if (!isLCDEnabled()) {
        LY = 0;
        clock = 0;
        return;
    }

    clock += cycles;

    // Each scanline takes 456 cycles
    if (clock >= 456) {
        clock -= 456;
        LY++;

        if (LY == 144) {
            // Enter V-Blank
            STAT |= 0x01; // Set V-Blank flag
            memory.io_registers[0x0F] |= 0x01; // Request V-Blank interrupt
            renderFrame();
        } else if (LY > 153) {
            // Reset to top of screen
            LY = 0;
        } else if (LY < 144) {
            // Render scanline
            renderScanline();
        }

        // Check for LY == LYC coincidence
        if (LY == LYC) {
            STAT |= 0x04; // Set coincidence flag
            if (STAT & 0x40) {
                memory.io_registers[0x0F] |= 0x02; // Request STAT interrupt
            }
        } else {
            STAT &= ~0x04; // Clear coincidence flag
        }
    }
}

// The rest of your render and helper methods remain unchanged
bool PPU::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return false; // Exit the loop if the window is closed
        }
    }
    return true;
}

void PPU::renderScanline() {
    if (isBackgroundEnabled()) {
        renderBackground();
    }
    if (isWindowEnabled()) {
        renderWindow();
    }
    if (isSpritesEnabled()) {
        renderSprites();
    }
}

void PPU::renderBackground() {
    uint8_t y = LY + SCY;
    for (uint8_t x = 0; x < 160; x++) {
        uint8_t pixelX = x + SCX;
        uint8_t tileData = getBackgroundTileData(pixelX, y);
        uint8_t color = getColor(tileData, BGP);
        drawPixel(x, LY, color);
    }
}

void PPU::renderWindow() {
    if (WY > LY) return; // Window is not visible on this scanline

    uint8_t y = LY - WY;
    for (uint8_t x = 0; x < 160; x++) {
        if (x < WX - 7) continue; // Window starts at WX - 7

        uint8_t pixelX = x - (WX - 7);
        uint8_t tileData = getWindowTileData(pixelX, y);
        uint8_t color = getColor(tileData, BGP);
        drawPixel(x, LY, color);
    }
}

void PPU::renderSprites() {
    for (int i = 0; i < 40; i++) {
        uint8_t index = i * 4;
        uint8_t yPos = memory.oam[index] - 16;
        uint8_t xPos = memory.oam[index + 1] - 8;
        uint8_t tileIndex = memory.oam[index + 2];
        uint8_t attributes = memory.oam[index + 3];

        if (LY < yPos || LY >= yPos + 8) continue; // Sprite not on this scanline

        bool usePalette1 = attributes & 0x10;
        bool xFlip = attributes & 0x20;
        bool yFlip = attributes & 0x40;
        bool behindBackground = attributes & 0x80;

        uint8_t line = LY - yPos;
        if (yFlip) line = 7 - line;

        uint8_t tileData = getSpriteTileData(tileIndex, line);
        uint8_t palette = usePalette1 ? OBP1 : OBP0;

        for (uint8_t x = 0; x < 8; x++) {
            uint8_t pixelX = xPos + (xFlip ? 7 - x : x);
            if (pixelX >= 160) continue; // Offscreen

            uint8_t color = getColor(tileData, palette);
            if (color == 0) continue; // Transparent pixel

            if (behindBackground && (framebuffer[(LY * 160 + pixelX) * 3] != 0)) continue;

            drawPixel(pixelX, LY, color);
        }
    }
}

void PPU::drawPixel(uint8_t x, uint8_t y, uint8_t color) {
    uint8_t r = 0, g = 0, b = 0;
    switch (color) {
        case 0: r = g = b = 255; break; // White
        case 1: r = g = b = 192; break; // Light gray
        case 2: r = g = b = 96; break;  // Dark gray
        case 3: r = g = b = 0; break;   // Black
    }

    framebuffer[(y * 160 + x) * 3] = r;
    framebuffer[(y * 160 + x) * 3 + 1] = g;
    framebuffer[(y * 160 + x) * 3 + 2] = b;
}

bool PPU::isLCDEnabled() const {
    return LCDC & 0x80;
}

bool PPU::isBackgroundEnabled() const {
    return LCDC & 0x01;
}

bool PPU::isWindowEnabled() const {
    return LCDC & 0x20;
}

bool PPU::isSpritesEnabled() const {
    return LCDC & 0x02;
}

uint8_t PPU::getBackgroundTileData(uint8_t x, uint8_t y) const {
    uint16_t tileMapAddr = (LCDC & 0x08) ? 0x1C00 : 0x1800;
    tileMapAddr += ((y / 8) * 32) + (x / 8);

    uint8_t tileIndex = memory.vram[tileMapAddr];
    uint16_t tileDataAddr = (LCDC & 0x10) ? 0x0000 : 0x1000;
    tileDataAddr += tileIndex * 16 + (y % 8) * 2;

    uint8_t byte1 = memory.vram[tileDataAddr];
    uint8_t byte2 = memory.vram[tileDataAddr + 1];
    uint8_t bit = 7 - (x % 8);
    return ((byte1 >> bit) & 1) | (((byte2 >> bit) & 1) << 1);
}

uint8_t PPU::getWindowTileData(uint8_t x, uint8_t y) const {
    uint16_t tileMapAddr = (LCDC & 0x40) ? 0x1C00 : 0x1800;
    tileMapAddr += ((y / 8) * 32) + (x / 8);

    uint8_t tileIndex = memory.vram[tileMapAddr];
    uint16_t tileDataAddr = (LCDC & 0x10) ? 0x0000 : 0x1000;
    tileDataAddr += tileIndex * 16 + (y % 8) * 2;

    uint8_t byte1 = memory.vram[tileDataAddr];
    uint8_t byte2 = memory.vram[tileDataAddr + 1];
    uint8_t bit = 7 - (x % 8);
    return ((byte1 >> bit) & 1) | (((byte2 >> bit) & 1) << 1);
}

uint8_t PPU::getSpriteTileData(uint8_t tileIndex, uint8_t line) const {
    uint16_t tileDataAddr = 0x0000 + tileIndex * 16 + line * 2;
    uint8_t byte1 = memory.vram[tileDataAddr];
    uint8_t byte2 = memory.vram[tileDataAddr + 1];
    return (byte1 & 0xFF) | ((byte2 & 0xFF) << 8);
}

uint8_t PPU::getColor(uint8_t colorId, uint8_t palette) const {
    return (palette >> (colorId * 2)) & 0x03;
}