CC = g++
CFLAGS = -std=c++11 -g -Wall

# Paths to SDL2 (update these to your actual SDL2 include and lib directories)
SDL2_INCLUDE_DIR = C:\Users\Amaad\Desktop\GBZero\SDL2\include
SDL2_LIB_DIR =C:\Users\Amaad\Desktop\GBZero\SDL2\lib

# SDL2 flags
SDL2_FLAGS = -I$(SDL2_INCLUDE_DIR) -L$(SDL2_LIB_DIR) -lSDL2

# List your source files here
SRC = main.cpp memory.cpp cpu.cpp ppu.cpp opcode_handlers.cpp

# The output executable
OUTPUT = gb_zero

# Compiler commands
$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC) $(SDL2_FLAGS)

# Clean up the generated files
clean:
	rm -f $(OUTPUT) *.o
