CC = g++
CFLAGS = -std=c++11 -g -Wall

# List your source files here
SRC = main.cpp cpu.cpp opcode_handlers.cpp

# The output executable
OUTPUT = gb_zero

# Compiler commands
$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC)

# Clean up the generated files
clean:
	rm -f $(OUTPUT) *.o