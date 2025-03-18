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
    cpu.memory.loadROM("roms/cpu_instrs.gb"); 
    cpu.dumpROMHeader();
    ppu.initializeSDL();
    // Resize window based on the scaling factor
    int windowWidth = 160 * SCALE_FACTOR;
    int windowHeight = 144 * SCALE_FACTOR;
    SDL_SetWindowSize(ppu.window, windowWidth, windowHeight);

    bool running = true;
    SDL_Event e;

   

    while (running) {
    
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            // Handle key presses for input (optional)
            // Example: if (e.type == SDL_KEYDOWN) { ... }
        }
    
     
       cpu.executeNextInstruction();
     
    
  
        ppu.renderFrame();
        SDL_Delay(FRAME_DELAY);
    }

    // Clean up and quit SDL2
    SDL_DestroyTexture(ppu.texture);
    SDL_DestroyRenderer(ppu.renderer);
    SDL_DestroyWindow(ppu.window);
    SDL_Quit();

    return 0;
}