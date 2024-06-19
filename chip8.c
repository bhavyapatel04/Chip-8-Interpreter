#include "/opt/homebrew/Cellar/sdl2/2.30.3/include/SDL2/SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

typedef struct Chip8 {
    uint8_t memory[4096];
    uint8_t registers[16];
    uint16_t index;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t keypad[16];
    uint32_t video[64 * 32];
    uint16_t opcode;
} Chip8;


int main() {
    puts("Hello, World!");
}





