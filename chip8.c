#include "/opt/homebrew/Cellar/sdl2/2.30.5/include/SDL2/SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "chip8_def.h"
#include "instructions.h"
#include "chip8_def.h"
#include "helperMethods.h"

#define CPU_CLOCK_DELAY 1000 //1ms delay between each cycle

int main(int argc, char* argv[]) {
    //seed the random number generator
    srand(time(NULL));
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        printf("Usage: ./chip8 path/to/rom\n");
        return 1;
    }

    // Store the path to the ROM in a variable
    char* romPath = argv[1];

    //intialize the chip8 system
    Chip8 user_chip8;
    initialize(&user_chip8);

    //loading the rom
    FILE* rom = fopen(romPath, "rb");
    if (rom == NULL) {
        printf("Failed to open ROM file\n");
        return 1;
    }
    fread(&user_chip8.ram[PROGRAM_START_ADDR], 1, TOTAL_RAM - PROGRAM_START_ADDR, rom);
    fclose(rom);

    
}

 




