#include "ppu.h"
#include <iostream>
#include <algorithm> // For std::fill_n

PPU::PPU(CPU& cpu, Memory& memory)
    : memory(memory), cpu(cpu),
      LCDC(memory.io_registers[0x40]),
      STAT(memory.io_registers[0x41]),
      SCY(memory.io_registers[0x42]),
      SCX(memory.io_registers[0x43]),
      LY(memory.io_registers[0x44]),
      LYC(memory.io_registers[0x45]),
      BGP(memory.io_registers[0x47]),
      OBP0(memory.io_registers[0x48]),
      OBP1(memory.io_registers[0x49]),
      WY(memory.io_registers[0x4A]),
      WX(memory.io_registers[0x4B]) {
    reset();
    initializeSDL();
}

PPU::~PPU() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void PPU::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow("Game Boy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160 * 3, 144 * 3, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    if (!texture) {
        std::cerr << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    std::cout << "SDL initialized successfully!" << std::endl;
}

void PPU::reset() {
    clearFramebuffer();
    currentScanline = 0;
    updatePalettes();
}

void PPU::clearFramebuffer() {
    std::fill_n(framebuffer, 160 * 144, 0xFFFFFFFF); 
    std::cout << "Framebuffer cleared!" << std::endl;
}

void PPU::renderScanline(uint8_t currentScanline) {
    if (!(LCDC & 0x80)) {
        std::cout << "LCD disabled, skipping scanline rendering." << std::endl;
        return;
    }

    for (int x = 0; x < 160; x++) {
        uint8_t color = calculatePixelColor(x, currentScanline);
        framebuffer[currentScanline * 160 + x] = bgPalette[color];
        std::cout << "Pixel at (" << x << ", " << currentScanline << ") set to color " << (int)color << std::endl;
    }

    if (LCDC & 0x02) {
        renderSprites(currentScanline);
    }
}

void PPU::renderSprites(uint8_t currentScanline) {
    for (int i = 0; i < 40; i++) {
        uint8_t yPos = memory.oam[i * 4] - 16;
        uint8_t xPos = memory.oam[i * 4 + 1] - 8;
        uint8_t tileIndex = memory.oam[i * 4 + 2];
        uint8_t attributes = memory.oam[i * 4 + 3];

        if (currentScanline >= yPos && currentScanline < yPos + 8) {
            uint16_t tileAddress = 0x8000 + tileIndex * 16 + (currentScanline - yPos) * 2;
            uint8_t lowByte = memory.vram[tileAddress];
            uint8_t highByte = memory.vram[tileAddress + 1];
            for (int x = 0; x < 8; x++) {
                uint8_t colorBit = 7 - x;
                uint8_t color = ((highByte >> colorBit) & 1) << 1 | ((lowByte >> colorBit) & 1);

                if (color != 0) {
                    if (!(attributes & 0x80) || framebuffer[currentScanline * 160 + xPos + x] == bgPalette[0]) {
                        uint32_t* palette = (attributes & 0x10) ? spritePalette1.data() : spritePalette0.data();
                        framebuffer[currentScanline * 160 + xPos + x] = palette[color];
                    }
                }
            }
        }
    }
}

void PPU::step(int cycles) {
    static int scanlineCounter = 0;
    scanlineCounter += cycles;
    std::cout<<"the scanline counter value is : " << int(scanlineCounter ) << std::endl;
    if (scanlineCounter >= 456) {
        scanlineCounter -= 456;
        currentScanline++;

        if (currentScanline < 144) {
            renderScanline(currentScanline);
        } else if (currentScanline == 144) {
            cpu.interruptFlags |= 0x01; // Trigger V-Blank interrupt
        } else if (currentScanline > 153) {
            currentScanline = 0;
            renderFrame();
        }

        updateLCDStatus();
    }
}

void PPU::updateLCDStatus() {
    LY = currentScanline;

    if (LY == LYC) {
        STAT |= 0x04; // Set LY=LYC flag
    } else {
        STAT &= ~0x04; // Clear LY=LYC flag
    }

    if (currentScanline < 144) {
        STAT = (STAT & 0xFC) | 0x03; // Mode 3: Pixel Transfer
    } else if (currentScanline == 144) {
        STAT = (STAT & 0xFC) | 0x01; // Mode 1: V-Blank
    } else if (currentScanline > 153) {
        STAT = (STAT & 0xFC) | 0x02; // Mode 2: OAM Search
    }
}

void PPU::updatePalettes() {
    for (int i = 0; i < 4; i++) {
        bgPalette[i] = getColor((BGP >> (i * 2)) & 0x03, false);
        spritePalette0[i] = getColor((OBP0 >> (i * 2)) & 0x03, true);
        spritePalette1[i] = getColor((OBP1 >> (i * 2)) & 0x03, true);
    }
}

uint8_t PPU::getColor(uint8_t colorID, bool isSprite) {
    switch (colorID) {
        case 0: return 0xFF; // White
        case 1: return 0xAA; // Light Gray
        case 2: return 0x55; // Dark Gray
        case 3: return 0x00; // Black
        default: return 0xFF; // Default to white
    }
}
uint8_t PPU::calculatePixelColor(uint8_t x, uint8_t y) {
    uint8_t color = 0; // Default color (transparent)

    // Calculate background pixel color
    if (LCDC & 0x01) { // Background enabled
        uint8_t bgX = (x + SCX) % 256;
        uint8_t bgY = (y + SCY) % 256;
        color = getBackgroundPixelColor(bgX, bgY);
    }

    // Calculate window pixel color (if enabled and visible)
    if ((LCDC & 0x20) && (x >= WX - 7) && (y >= WY)) { // Window enabled and visible
        uint8_t winX = x - (WX - 7);
        uint8_t winY = y - WY;
        color = getBackgroundPixelColor(winX, winY);
    }

    // Calculate sprite pixel color (if enabled)
    if (LCDC & 0x02) { // Sprites enabled
        uint8_t spriteColor = getSpritePixelColor(x, y);
        if (spriteColor != 0) { // Sprite pixel is not transparent
            color = spriteColor;
        }
    }

    return color;
}
uint8_t PPU::getBackgroundPixelColor(uint8_t x, uint8_t y) {
    // Determine which tile map to use
    uint16_t tileMapAddress = (LCDC & 0x08) ? 0x1C00 : 0x1800; // Background tile map
    if (y >= WY && x >= WX - 7) { // Window tile map
        tileMapAddress = (LCDC & 0x40) ? 0x1C00 : 0x1800;
    }

    // Get the tile index from the tile map
    uint8_t tileX = x / 8;
    uint8_t tileY = y / 8;
    uint8_t tileIndex = memory.vram[tileMapAddress + tileY * 32 + tileX];

    // Get the tile data address
    uint16_t tileDataAddress = (LCDC & 0x10) ? 0x0000 : 0x1000;
    uint16_t tileAddress = tileDataAddress + tileIndex * 16 + (y % 8) * 2;

    // Get the pixel color from the tile data
    uint8_t lowByte = memory.vram[tileAddress];
    uint8_t highByte = memory.vram[tileAddress + 1];
    uint8_t colorBit = 7 - (x % 8);
    uint8_t color = ((highByte >> colorBit) & 1) << 1 | ((lowByte >> colorBit) & 1);

    return color;
}
uint8_t PPU::getSpritePixelColor(uint8_t x, uint8_t y) {
    for (int i = 0; i < 40; i++) {
        // Get sprite attributes from OAM
        uint8_t yPos = memory.oam[i * 4] - 16;
        uint8_t xPos = memory.oam[i * 4 + 1] - 8;
        uint8_t tileIndex = memory.oam[i * 4 + 2];
        //uint8_t attributes = memory.oam[i * 4 + 3];

        // Check if the sprite is at this position
        if (y >= yPos && y < yPos + 8 && x >= xPos && x < xPos + 8) {
            // Get the tile data address
            uint16_t tileAddress = 0x8000 + tileIndex * 16 + (y - yPos) * 2;

            // Get the pixel color from the tile data
            uint8_t lowByte = memory.vram[tileAddress];
            uint8_t highByte = memory.vram[tileAddress + 1];
            uint8_t colorBit = 7 - (x - xPos);
            uint8_t color = ((highByte >> colorBit) & 1) << 1 | ((lowByte >> colorBit) & 1);

            // Check if the pixel is not transparent
            if (color != 0) {
                return color;
            }
        }
    }

    return 0; // No sprite pixel at this position
}

void PPU::renderFrame() {
    SDL_UpdateTexture(texture, nullptr, framebuffer, 160 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    std::cout << "Frame rendered!" << std::endl;
}