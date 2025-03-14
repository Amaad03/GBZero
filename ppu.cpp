#include "ppu.h"
#include <iostream>

PPU::PPU(Memory& memory)
    : memory(memory),
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
    initializeSDL(); // Initialize SDL
}

PPU::~PPU() {
    // Clean up SDL resources
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

void PPU::step(unsigned int cycles) {
    clock += cycles;

    // Render one scanline every 456 cycles
    if (clock >= 456) {
        clock -= 456;
        currentScanline++;

        if (currentScanline < 144) {
            renderScanline(); // Render the current scanline
        } else if (currentScanline == 144) {
            updateLCDStatus(); // Enter vertical blanking period
            renderFrame();     // Render the final frame
        } else if (currentScanline > 153) {
            currentScanline = 0; // Reset scanline counter
        }
    }
}

void PPU::renderScanline() {
    if (isBackgroundEnabled()) {
        for (int x = 0; x < 160; x++) {
            uint8_t tileData = getBackgroundTileData(x, currentScanline);
            uint8_t color = getColor(tileData, BGP);
            framebuffer[currentScanline * 160 + x] = color;
        }
    }

    if (isWindowEnabled()) {
        for (int x = 0; x < 160; x++) {
            uint8_t tileData = getWindowTileData(x, currentScanline);
            uint8_t color = getColor(tileData, BGP);
            framebuffer[currentScanline * 160 + x] = color;
        }
    }

    renderSpritesLine(currentScanline);
}

void PPU::renderFrame() {
    void* pixels;
    int pitch;
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
        std::cerr << "SDL_LockTexture failed: " << SDL_GetError() << std::endl;
        return;
    }

    uint8_t* pixelData = static_cast<uint8_t*>(pixels);

    for (int y = 0; y < 144; y++) {
        for (int x = 0; x < 160; x++) {
            uint8_t pixel = framebuffer[y * 160 + x];
            uint8_t r, g, b;

            switch (pixel) {
                case 0: r = 255; g = 255; b = 255; break; // White
                case 1: r = 160; g = 160; b = 160; break; // Light gray
                case 2: r = 80; g = 80; b = 80; break;    // Dark gray
                case 3: r = 0; g = 0; b = 0; break;       // Black
                default: r = 255; g = 255; b = 255; break; // Default to white
            }

            pixelData[(y * 160 + x) * 3 + 0] = r; // R
            pixelData[(y * 160 + x) * 3 + 1] = g; // G
            pixelData[(y * 160 + x) * 3 + 2] = b; // B
        }
    }

    SDL_UnlockTexture(texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool PPU::isBackgroundEnabled() const {
    return (LCDC & 0x01) != 0;
}

bool PPU::isWindowEnabled() const {
    return (LCDC & 0x20) != 0;
}

uint8_t PPU::getBackgroundTileData(uint8_t x, uint8_t y) const {
    // Calculate the tile coordinates
    uint8_t tileX = (x + SCX) / 8;
    uint8_t tileY = (y + SCY) / 8;

    // Calculate the tile index in the background map
    uint16_t tileMapAddress = (LCDC & 0x08) ? 0x1C00 : 0x1800; // Choose background map (0x9800 or 0x9C00)
    uint8_t tileIndex = memory.read(tileMapAddress + tileY * 32 + tileX);

    // Calculate the address of the tile data
    uint16_t tileDataAddress;
    if (LCDC & 0x10) {
        // Tile data at 0x8000-0x8FFF (unsigned)
        tileDataAddress = 0x8000 + tileIndex * 16;
    } else {
        // Tile data at 0x8800-0x97FF (signed)
        tileDataAddress = 0x9000 + static_cast<int8_t>(tileIndex) * 16;
    }

    // Calculate the line within the tile
    uint8_t tileLine = (y + SCY) % 8;
    uint8_t data1 = memory.read(tileDataAddress + tileLine * 2);
    uint8_t data2 = memory.read(tileDataAddress + tileLine * 2 + 1);

    // Calculate the bit position for the pixel
    uint8_t bit = 7 - (x + SCX) % 8;
    uint8_t colorId = ((data1 >> bit) & 1) | (((data2 >> bit) & 1) << 1);

    return colorId;
}

uint8_t PPU::getWindowTileData(uint8_t x, uint8_t y) const {
    // Calculate the tile coordinates relative to the window
    uint8_t tileX = (x - WX + 7) / 8; // WX is offset by 7
    uint8_t tileY = (y - WY) / 8;

    // Calculate the tile index in the window map
    uint16_t tileMapAddress = (LCDC & 0x40) ? 0x1C00 : 0x1800; // Choose window map (0x9800 or 0x9C00)
    uint8_t tileIndex = memory.read(tileMapAddress + tileY * 32 + tileX);

    // Calculate the address of the tile data
    uint16_t tileDataAddress;
    if (LCDC & 0x10) {
        // Tile data at 0x8000-0x8FFF (unsigned)
        tileDataAddress = 0x8000 + tileIndex * 16;
    } else {
        // Tile data at 0x8800-0x97FF (signed)
        tileDataAddress = 0x9000 + static_cast<int8_t>(tileIndex) * 16;
    }

    // Calculate the line within the tile
    uint8_t tileLine = (y - WY) % 8;
    uint8_t data1 = memory.read(tileDataAddress + tileLine * 2);
    uint8_t data2 = memory.read(tileDataAddress + tileLine * 2 + 1);

    // Calculate the bit position for the pixel
    uint8_t bit = 7 - (x - WX + 7) % 8;
    uint8_t colorId = ((data1 >> bit) & 1) | (((data2 >> bit) & 1) << 1);

    return colorId;
}

uint8_t PPU::getColor(uint8_t colorId, uint8_t palette) const {
    // Extract the color from the palette
    return (palette >> (colorId * 2)) & 0x03;
}

void PPU::renderSpritesLine(uint8_t scanline) {
    // Check if sprites are enabled
    if (!(LCDC & 0x02)) return;

    // Determine sprite size (8x8 or 8x16)
    uint8_t spriteHeight = (LCDC & 0x04) ? 16 : 8;

    // Iterate through the sprite attributes in OAM (0xFE00-0xFE9F)
    for (int i = 0; i < 40; i++) {
        uint16_t oamAddress = 0xFE00 + i * 4;
        uint8_t yPos = memory.read(oamAddress) - 16;
        uint8_t xPos = memory.read(oamAddress + 1) - 8;
        uint8_t tileIndex = memory.read(oamAddress + 2);
        uint8_t attributes = memory.read(oamAddress + 3);

        // Check if the sprite is on the current scanline
        if (scanline >= yPos && scanline < yPos + spriteHeight) {
            // Determine the tile line to render
            uint8_t tileLine = scanline - yPos;
            if (attributes & 0x40) { // Y-flip
                tileLine = spriteHeight - 1 - tileLine;
            }

            // Calculate the address of the tile data
            uint16_t tileDataAddress = 0x8000 + tileIndex * 16 + tileLine * 2;

            // Read the tile data
            uint8_t data1 = memory.read(tileDataAddress);
            uint8_t data2 = memory.read(tileDataAddress + 1);

            // Determine the palette
            uint8_t palette = (attributes & 0x10) ? OBP1 : OBP0;

            // Render the sprite pixels
            for (int bit = 0; bit < 8; bit++) {
                uint8_t pixelX = xPos + (7 - bit);
                if (pixelX >= 160) continue; // Skip pixels outside the screen

                // Calculate the color ID
                uint8_t colorId = ((data1 >> bit) & 1) | (((data2 >> bit) & 1) << 1);
                if (colorId == 0) continue; // Transparent pixel

                // Check if the sprite has priority over the background
                if ((attributes & 0x80) && framebuffer[scanline * 160 + pixelX] != 0) continue;

                // Set the pixel color
                framebuffer[scanline * 160 + pixelX] = getColor(colorId, palette);
            }
        }
    }
}

void PPU::updateLCDStatus() {
    // Update the LY register
    LY = currentScanline;

    // Check for LY == LYC coincidence
    if (LY == LYC) {
        STAT |= 0x04; // Set coincidence flag
        if (STAT & 0x40) {
            // Trigger LCD STAT interrupt
            memory.write(0xFF0F, memory.read(0xFF0F) | 0x02);
        }
    } else {
        STAT &= ~0x04; // Clear coincidence flag
    }

    // Update the mode bits in STAT
    if (currentScanline >= 144) {
        STAT = (STAT & 0xFC) | 0x01; // Mode 1: V-Blank
        if (STAT & 0x10) {
            // Trigger LCD STAT interrupt
            memory.write(0xFF0F, memory.read(0xFF0F) | 0x02);
        }
    } else {
        STAT = (STAT & 0xFC) | 0x02; // Mode 2: OAM Search
        if (STAT & 0x20) {
            // Trigger LCD STAT interrupt
            memory.write(0xFF0F, memory.read(0xFF0F) | 0x02);
        }
    }
}