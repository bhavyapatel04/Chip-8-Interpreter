#include "/opt/homebrew/Cellar/sdl2/2.30.5/include/SDL2/SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "chip8_def.h"
#include "instructions.h"
#include "chip8_def.h"

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

    // Rest of the code...
}

 




