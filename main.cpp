#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include "cpu.h"
#include "memory.h"
#include "ppu.h"

const int SCALE_FACTOR = 3; // Increase or decrease to scale the screen
const int CYCLES_PER_FRAME = 70224; // Total CPU cycles per frame (154 scanlines × 456 cycles)
const int FRAME_RATE = 60; // Target frame rate (60 FPS)
const int FRAME_DELAY = 1000 / FRAME_RATE; // Delay per frame in milliseconds

int main(int argc, char* argv[]) {
    // Initialize memory, PPU, and CPU
    Memory memory;
    CPU cpu(memory);
    PPU ppu(cpu, memory);

    // Initialize the CPU
    cpu.reset();
    cpu.memory.loadROM("roms/Red.gb");
    cpu.dumpROMHeader();

    // Resize window based on the scaling factor
    int windowWidth = 160 * SCALE_FACTOR;
    int windowHeight = 144 * SCALE_FACTOR;
    SDL_SetWindowSize(ppu.window, windowWidth, windowHeight);

    bool running = true;
    SDL_Event e;

    // Frame timing variables
    Uint32 frameStart;
    int frameTime;

    while (running) {
        frameStart = SDL_GetTicks(); // Get the current time

        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            // Handle key presses for input (optional)
            // Example: if (e.type == SDL_KEYDOWN) { ... }
        }

        // Execute CPU instructions and update PPU
        cpu.executeNextInstruction();

        // Render the frame
        ppu.renderFrame();

        // Limit frame rate to 60 FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    // Clean up and quit SDL2
    SDL_DestroyTexture(ppu.texture);
    SDL_DestroyRenderer(ppu.renderer);
    SDL_DestroyWindow(ppu.window);
    SDL_Quit();

    return 0;
}