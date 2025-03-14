#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include "cpu.h"
#include "memory.h"
#include "ppu.h"

const int SCALE_FACTOR = 3; // Increase or decrease to scale the screen

int main(int argc, char* argv[]) {
    // Initialize memory, PPU, and CPU
    Memory memory;
    PPU ppu(memory); // Pass memory to the PPU constructor
    CPU cpu(memory, ppu);
    // Initialize the CPU
    cpu.reset();
    cpu.memory.loadROM("Dr. Mario (World).gb");

    // Initialize SDL2 and set up graphics
    ppu.initializeSDL();  // Initialize SDL and create a window

    // Resize window based on the scaling factor
    int windowWidth = 160 * SCALE_FACTOR;
    int windowHeight = 144 * SCALE_FACTOR;

    // Resize the SDL window to the new resolution
    SDL_SetWindowSize(ppu.window, windowWidth, windowHeight);

    bool running = true;
    SDL_Event e;

    while (running) {
        // Handle SDL events (e.g., window close)
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
    
        // Execute the next CPU instruction and get the number of cycles it took
        cpu.executeNextInstruction();
        
        // Step the PPU by the number of cycles the CPU just executed
        //ppu.step(cycles);
    
        // Render the graphics to the screen with scaling
        ppu.renderFrame();
    
        // Delay to maintain ~60 FPS
        SDL_Delay(16);  // ~60 FPS (adjust based on your emulator's speed)
    }

    // Clean up and quit SDL2
    SDL_DestroyTexture(ppu.texture);
    SDL_DestroyRenderer(ppu.renderer);
    SDL_DestroyWindow(ppu.window);
    SDL_Quit();

    return 0;
}
